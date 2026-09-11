import os.path

import pygame
import sys
import math
from enum import Enum
from os import listdir
from os.path import isfile, join
import random
from functools import total_ordering
import time


def main():
    pygame.init()
    pygame.display.set_caption("Grigorescu Stefan D. [Grupa 231] - Atoms")
    gr = Graph.getInstance()

    reloadChoice = -1
    pathIn = None
    player = 0
    if input("Reload a saved file? [n/y]: ") == 'y':
        savesPath = "saves/"
        files = [f for f in listdir(savesPath) if isfile(join(savesPath, f))]
        for number, file in enumerate(files):
            print(str(number) + ". " + str(os.path.basename(file)))
        reloadChoice = int(input("Choose file number: "))
        pathIn = savesPath + files[reloadChoice]

    if reloadChoice < 0:
        algorithm, player, difficulty, gameMode = drawChoices(gr.display)

        gr.algorithm = Algorithm[algorithm]

        if gameMode == "PvP":
            gr.playerType[0] = gr.playerType[1] = PlayerType.HUMAN
        elif gameMode == "EvE":
            gr.playerType[0] = gr.playerType[1] = PlayerType.NPC
        else:
            gr.playerType[0] = PlayerType(player)
            gr.playerType[1] = PlayerType(1 - player)

        if difficulty == "easy":
            gr.moveDepth = 1
        elif difficulty == "medium":
            gr.moveDepth = 3
        elif difficulty == "hard":
            gr.moveDepth = 5
    else:
        player = gr.reload(pathIn)

    gr.play(player)


class Graph:
    instance = None
    display = None
    linesNo = 7
    columnsNo = 7
    totalPiecesOfPlayer = 11
    selectedPiece = None
    lDColors = []
    lDColorsIdx = 0

    def __init__(self, scaleCount=100, translationCount=80, pointRadius=10, pieceRadius=40):
        """
        Constructorul. Nu ar trebui apelat, ci sa se foloseasca metoda getInstance a clasei, pentru a realiza design patternul de singleton.
        :param scaleCount: Scalarea.
        :param translationCount: Tranzitia.
        :param pointRadius: Raza unui punct neocupat de pe tabla de joc.
        :param pieceRadius: Raza unei piese.
        """
        Graph.display = pygame.display.set_mode(size=(900, 900))

        self.scaleCount = scaleCount
        self.translationCount = translationCount
        self.pointRadius = pointRadius
        self.pieceRadius = pieceRadius
        Images.rescale(2 * pieceRadius)

        self.nodes = []
        dColors = dict([])
        for y in range(Graph.linesNo + 1):
            for x in range(Graph.columnsNo + 1):
                newNode = Node(x, y, Node.getNextId())
                self.nodes.append(newNode)
                dColors[newNode.id] = None

        Graph.lDColors.append(dColors)

        for node in self.nodes:
            node.x = self.translationCount + self.scaleCount * node.columnIdx
            node.y = self.translationCount + self.scaleCount * node.lineIdx

        self.piecesPlaced = [0, 0]
        self.playerType = [PlayerType(0), PlayerType(0)]
        self.moveDepth = 1
        self.algorithm = Algorithm.ALPHABETA
        self.minNPCThinkTime = float('inf')
        self.maxNPCThinkTime = 0

        self.edges = self.initEdges()

    def play(self, player=0):
        """
        Runs the game.
        :param player: The player to act next.
        :return:
        """
        self.drawPlayScreen()

        while True:
            if self.playerType[player] == PlayerType.NPC:
                startTime = time.time()

                if self.piecesPlaced[1] < self.totalPiecesOfPlayer:     # still in Placing Phase
                    node = self.getBestPlacement(player)
                    if not node:
                        print("The {} NPC found no solution and aborted the match!".format(Color(player).name))
                        self.declareWinner(1 - player)

                    player = self.appendGameState(node, player, GamePhase.PLACE)
                else:
                    node, Graph.selectedPiece, moveScore = None, None, None
                    if self.algorithm == Algorithm.ALPHABETA:
                        node, Graph.selectedPiece, moveScore, alpha, beta = self.alphaBeta(player)
                    else:
                        node, Graph.selectedPiece, moveScore = self.minMax(player)
                    print("The {} algorithm estimated the move score: {}".format(self.algorithm.name, moveScore))
                    if not (node and Graph.selectedPiece and moveScore):
                        print("The {} NPC found no solution and aborted the match!".format(Color(player).name))
                        self.declareWinner(1 - player)

                    player = self.appendGameState(node, player, GamePhase.MOVE)

                endTime = time.time()
                duration = endTime - startTime
                if self.minNPCThinkTime > duration:
                    self.minNPCThinkTime = duration
                if self.maxNPCThinkTime < duration:
                    self.maxNPCThinkTime = duration

                self.drawPlayScreen()
                print("{} player's turn lasted for: {}".format(Color(1 - player).name, duration))
                print("Turn: " + Color(player).name)

            else:       # Human player
                startTime = time.time()

                for ev in pygame.event.get():
                    if ev.type == pygame.QUIT:
                        self.declareWinner(None)

                    if self.piecesPlaced[1] == Graph.totalPiecesOfPlayer and not self.canMoveAnyPiece(
                            player):  # last piece was placed and the current player can not move
                        self.declareWinner(1 - player)

                    if ev.type == pygame.KEYDOWN:
                        if ev.key == pygame.K_u:
                            player = self.undo(player)
                            self.drawPlayScreen()
                        elif ev.key == pygame.K_r:
                            self.reset()
                            player = 0
                            self.drawPlayScreen()
                        elif ev.key == pygame.K_s:
                            self.save(player)
                        print("Turn: " + Color(player).name)
                        break

                    elif ev.type == pygame.MOUSEBUTTONDOWN:
                        pos = pygame.mouse.get_pos()

                        for node in self.nodes:
                            if euclidDist(pos, node.getCoords()) <= self.pieceRadius:
                                if node.getColor() is None:  # daca am apasat pe un node liber
                                    if Graph.selectedPiece:  # daca avem o piesa selectata
                                        if self.adjacent(node, Graph.selectedPiece) and self.validMove(Graph.selectedPiece,
                                                                                                       node):
                                            player = self.appendGameState(node, player, GamePhase.MOVE)
                                        else:  # avem piesa selectata, dar pozitia aleasa pentru mutare nu e valida
                                            continue  # nu mai reafisam ecranul daca nu s-a facut nicio modificare
                                    elif self.validPlacement(node, player):  # daca asezarea este valida
                                        player = self.appendGameState(node, player, GamePhase.PLACE)
                                    else:
                                        continue  # nu mai reafisam ecranul daca asezarea nu a fost valida

                                elif node.getColor() == Color(
                                        player):  # daca am apasat pe o piesa a jucatorului care e la rand
                                    if Graph.selectedPiece == node:  # daca piesa era cea selectata
                                        Graph.selectedPiece = None  # o deselectam
                                    else:
                                        Graph.selectedPiece = node  # o selectam
                                else:  # piesa aleasa nu este de culoarea jucatorului care e la rand
                                    continue  # nu mai reafisam ecranul daca nu s-a facut nicio modificare

                                endTime = time.time()
                                self.drawPlayScreen()
                                print("{} player's turn lasted for: {}".format(Color(1 - player).name, endTime - startTime))
                                print("Turn: " + Color(player).name)
                                break

    def undo(self, player):
        """
        Undoes the last move.
        :param player: The player to act next.
        :return: The player to act next (considering the undo).
        """
        if Graph.lDColorsIdx:
            Graph.lDColorsIdx -= 1
            Graph.lDColors = Graph.lDColors[:-1]
            Graph.selectedPiece = None
            if Graph.lDColorsIdx < 22:
                self.piecesPlaced[Graph.lDColorsIdx % 2] -= 1
            player = 1 - player
        return player

    def save(self, player):
        """
        Saves the progress of the current game in the saves directory.
        :param player: The player to act next.
        :return:
        """
        pathOut = input("Input file name for the save: ")
        pathOut = "saves/" + pathOut + ".txt"
        fout = open(pathOut, 'a+')

        if Graph.selectedPiece:
            fout.write(str(Graph.selectedPiece.id) + "\n")
        else:
            fout.write(str(None) + "\n")

        fout.write(str(Graph.lDColorsIdx) + "\n")
        for d in Graph.lDColors:
            s = ""
            for k, v in d.items():
                if v:
                    s += str(k) + " " + str(v.name) + ";"
                else:
                    s += str(k) + " " + str(None) + ";"
            fout.write(s + "\n")
        fout.write(str(self.piecesPlaced[0]) + " " + str(self.piecesPlaced[1]) + "\n")
        fout.write(str(self.playerType[0].name) + " " + str(self.playerType[1].name) + "\n")
        fout.write(str(self.moveDepth) + "\n")
        fout.write(str(self.algorithm.name) + "\n")
        fout.write(str(self.minNPCThinkTime) + "\n")
        fout.write(str(self.maxNPCThinkTime) + "\n")
        fout.write(str(player))
        fout.close()

    def reload(self, pathIn):
        """
        Reloads a the game progress from a save file.
        :param pathIn: The path of the save file.
        :return: The player to act next.
        """
        fin = open(pathIn, 'r')
        line = fin.readline().strip("\n")
        if line != str(None):
            Graph.selectedPiece = self.nodes[int(line)]
        Graph.lDColorsIdx = int(fin.readline().strip("\n"))
        for i in range(Graph.lDColorsIdx + 1):
            line = fin.readline().strip("\n").split(";")
            d = dict([])
            for pair in line:
                if pair == '':
                    continue
                pair = pair.split(" ")
                k = int(pair[0])
                v = pair[1]
                if v != str(None):
                    d[k] = Color[v]
                else:
                    d[k] = None
            Graph.lDColors.append(d)
        line = fin.readline().strip("\n").split(" ")
        self.piecesPlaced[0] = int(line[0])
        self.piecesPlaced[1] = int(line[1])
        line = fin.readline().strip("\n").split(" ")
        self.playerType[0] = PlayerType[line[0]]
        self.playerType[1] = PlayerType[line[1]]
        self.moveDepth = int(fin.readline().strip("\n"))
        self.algorithm = Algorithm[fin.readline().strip("\n")]
        self.minNPCThinkTime = int(fin.readline().strip("\n"))
        self.maxNPCThinkTime = int(fin.readline().strip("\n"))
        player = int(fin.readline().strip("\n"))
        fin.close()

        return player

    def reset(self):
        """
        Resets the current game to its starting state.
        :return:
        """
        Graph.selectedPiece = None
        Graph.lDColors = []
        Graph.lDColorsIdx = 0

        dColors = dict([])
        for node in self.nodes:
            dColors[node.id] = None

        Graph.lDColors.append(dColors)

        self.piecesPlaced = [0, 0]

    def canMoveAnyPiece(self, player):
        """
        Checks wether the game ended by no move possibility for the current player or not.
        :param player: The player to move a piece next.
        :return: True if the player still has available moves to play and False otherwise.
        """
        for node in self.nodes:
            if node.getColor() == Color(player) and len(self.getValidMoves(node)) > 0:
                return True
        return False

    def alphaBeta(self, player, depth=0, scoreMethod=None, alpha=-500, beta=500):
        """
        Gets the best piece move for the given player.
        :param player: The player to move a piece next.
        :param depth: The depth of the algorithm.
        :param scoreMethod: The chosen method to calculate the score of a move. If default or None, totalValidMoves will be used instead.
        :param alpha: The lower bound score of the algorithm.
        :param beta: The upper bound score of the algorithm.
        :return: The best node to move a piece onto and the best node to be moved, for the given player, and also a score of the move.
        """
        if self.piecesPlaced[0] < Graph.totalPiecesOfPlayer or self.piecesPlaced[1] < Graph.totalPiecesOfPlayer:
            # not all pieces were placed
            return None, None, None, alpha, beta
        if alpha > beta:    # invalid interval
            return None, None, None, alpha, beta

        if not scoreMethod:
            scoreMethod = ScoreMethod.totalValidMoves
        lScoreStats = self.getLScoreStats(player, scoreMethod)

        if depth >= self.moveDepth:     # max depth reached
            if not lScoreStats:
                return None, None, None, alpha, beta
            return self.nodes[lScoreStats[len(lScoreStats) - 1].destNodeId], self.nodes[lScoreStats[len(lScoreStats) - 1].sourceNodeId], lScoreStats[len(lScoreStats) - 1].score[player], alpha, beta
            # the id of the best Node for the current player to place a piece

        crtScore = None
        destNode = None
        sourceNode = None

        if depth % 2 == 0:      # the player who initiated the search, who tries to minimize the score
            crtScore = float('inf')

            for crtScoreStats in lScoreStats:
                Graph.selectedPiece = self.nodes[crtScoreStats.sourceNodeId]
                player = self.appendGameState(self.nodes[crtScoreStats.destNodeId], player, GamePhase.MOVE)
                returnedDestNode, returnedSourceNode, returnedScore, alpha, beta = self.alphaBeta(player, depth + 1, scoreMethod, alpha, beta)

                if returnedScore is not None and returnedDestNode and returnedSourceNode:
                    if crtScore > returnedScore:
                        crtScore = returnedScore
                        destNode = returnedDestNode
                        sourceNode = returnedSourceNode

                    if beta > crtScore:
                        beta = crtScore
                        if alpha >= beta:
                            break

                player = self.undo(player)
        else:                   # the opponent of the player who initiated the search, who tries to maximize the score
            crtScore = float('-inf')

            for crtScoreStats in lScoreStats:
                Graph.selectedPiece = self.nodes[crtScoreStats.sourceNodeId]
                player = self.appendGameState(self.nodes[crtScoreStats.destNodeId], player, GamePhase.MOVE)
                returnedDestNode, returnedSourceNode, returnedScore, alpha, beta = self.alphaBeta(player, depth + 1, scoreMethod,
                                                                                     alpha, beta)
                if returnedScore is not None and returnedDestNode and returnedSourceNode:
                    if crtScore < returnedScore:
                        crtScore = returnedScore
                        destNode = returnedDestNode
                        sourceNode = returnedSourceNode

                    if alpha < crtScore:
                        alpha = crtScore
                        if alpha >= beta:
                            break

                player = self.undo(player)

        return destNode, sourceNode, crtScore, alpha, beta

    def minMax(self, player, depth=0, scoreMethod=None):
        """
        Gets the best piece move for the given player.
        :param player: The player to move a piece next.
        :param depth: The depth of the algorithm.
        :param scoreMethod: The chosen method to calculate the score of a move. If default or None, totalValidMoves will be used instead.
        :return: The best node to move a piece onto and the best node to be moved, for the given player, and also a score of the move.
        """
        if self.piecesPlaced[0] < Graph.totalPiecesOfPlayer or self.piecesPlaced[1] < Graph.totalPiecesOfPlayer:
            # not all pieces were placed
            return None, None, None

        if not scoreMethod:
            scoreMethod = ScoreMethod.totalValidMoves
        lScoreStats = self.getLScoreStats(player, scoreMethod)

        if depth >= self.moveDepth:     # max depth reached
            if not lScoreStats:
                return None, None, None
            return self.nodes[lScoreStats[len(lScoreStats) - 1].destNodeId], self.nodes[lScoreStats[len(lScoreStats) - 1].sourceNodeId], lScoreStats[len(lScoreStats) - 1].score[player]
            # the id of the best Node for the current player to place a piece

        crtScore = None
        destNode = None
        sourceNode = None

        if depth % 2 == 0:      # the player who initiated the search, who tries to minimize the score
            crtScore = float('inf')

            for crtScoreStats in lScoreStats:
                Graph.selectedPiece = self.nodes[crtScoreStats.sourceNodeId]
                player = self.appendGameState(self.nodes[crtScoreStats.destNodeId], player, GamePhase.MOVE)
                returnedDestNode, returnedSourceNode, returnedScore = self.minMax(player, depth + 1, scoreMethod)

                if returnedScore is not None and returnedDestNode and returnedSourceNode:
                    if crtScore > returnedScore:
                        crtScore = returnedScore
                        destNode = returnedDestNode
                        sourceNode = returnedSourceNode

                player = self.undo(player)
        else:                   # the opponent of the player who initiated the search, who tries to maximize the score
            crtScore = float('-inf')

            for crtScoreStats in lScoreStats:
                Graph.selectedPiece = self.nodes[crtScoreStats.sourceNodeId]
                player = self.appendGameState(self.nodes[crtScoreStats.destNodeId], player, GamePhase.MOVE)
                returnedDestNode, returnedSourceNode, returnedScore = self.minMax(player, depth + 1, scoreMethod)

                if returnedScore is not None and returnedDestNode and returnedSourceNode:
                    if crtScore < returnedScore:
                        crtScore = returnedScore
                        destNode = returnedDestNode
                        sourceNode = returnedSourceNode

                player = self.undo(player)

        return destNode, sourceNode, crtScore

    def getBestPlacement(self, player):
        """
        Gets the best piece placement for the given player.
        :param player: The player to place a piece next.
        :return: The the best node to place a piece onto for the given player or None if unavailable.
        """
        if player:
            if self.piecesPlaced[1] == Graph.totalPiecesOfPlayer:  # all black pieces were placed
                return None
        elif self.piecesPlaced[0] == Graph.totalPiecesOfPlayer:  # all white pieces were placed
            return None
        if self.piecesPlaced[0] == 0:  # no piece was placed
            return self.nodes[random.randint(0, len(self.nodes) - 1)]

        lScoreStats = []
        for node in self.nodes:
            if not node.getColor():
                valence, stable = self.getValence(node)
                if valence > 0:
                    player = self.appendGameState(node, player, GamePhase.PLACE)

                    crtScoreStats = ScoreStats(node.id, None,
                                               self.getScore(ScoreMethod.totalStablePieces), 1 - player, GamePhase.PLACE, extraScore=self.getScore(ScoreMethod.totalValidMoves))

                    insertPos = binarySearchInsertPos(lScoreStats, crtScoreStats)
                    if insertPos < len(lScoreStats):
                        lScoreStats.insert(insertPos, crtScoreStats)
                    else:
                        lScoreStats.append(crtScoreStats)

                    player = self.undo(player)

        if not lScoreStats:
            return None
        return self.nodes[lScoreStats[len(lScoreStats) - 1].destNodeId]     # the best Node for the current player to place a piece

    def getLScoreStats(self, player, scoreMethod):
        """
        Gets the list of ScoreStats for the child game states of the current game state, in the move phase.
        :param player: The player to move next.
        :param scoreMethod: The chosen method to calculate the score of a move.
        :return: The list of ScoreStats for the child game states of the current game state.
        """
        lScoreStats = []
        for node in self.nodes:
            if node.getColor() == Color(player):
                Graph.selectedPiece = node
                validMoves = self.getValidMoves(node)
                for neighbor in validMoves:
                    Graph.selectedPiece = node
                    player = self.appendGameState(neighbor, player, GamePhase.MOVE)

                    crtScoreStats = ScoreStats(neighbor.id, node.id,
                                               self.getScore(scoreMethod), 1 - player, GamePhase.PLACE)

                    if crtScoreStats.getScoreRatio(1 - player) > 0 or not len(lScoreStats):   # if no 0 scores encountered for current player or lScoreStats is empty, insert crtScoreStats
                        insertPos = binarySearchInsertPos(lScoreStats, crtScoreStats)   # rearrangement of successors for the algorithm
                        if insertPos < len(lScoreStats):
                            lScoreStats.insert(insertPos, crtScoreStats)
                        else:
                            lScoreStats.append(crtScoreStats)

                    player = self.undo(player)
        return lScoreStats

    def appendGameState(self, node, player, gamePhase):
        """
        Copies the last game state dict, appends the copy to the game states list and modifies it by applying a piece placing or move.
        :param node: The node a piece will be placed or moved onto.
        :param player: The player who acts next.
        :param gamePhase: The phase of the game, wether it is still in the placing phase or the moving phase.
        :return: The player who acts next (considering the action of the given player was made).
        """
        newGameState = dict([])
        for k, v in Graph.lDColors[Graph.lDColorsIdx].items():
            newGameState[k] = v
        Graph.lDColors.append(newGameState)
        Graph.lDColorsIdx += 1

        if gamePhase == GamePhase.PLACE:
            node.setColor(Color(player))
            self.piecesPlaced[player] += 1
        else:
            node.setColor(Graph.selectedPiece.getColor())
            Graph.selectedPiece.setColor(None)
            Graph.selectedPiece = None

            self.removeUnstablePieces()

        return 1 - player

    def removeUnstablePieces(self):
        """
        Sets all unstable node's (that don not have adjacent nodes of opposite color) color to None, removing them from the game.
        :return:
        """
        for node in self.nodes:
            if not self.getValence(node)[1]:
                node.setColor(None)

    def getScore(self, method):
        """
        Gets the score considering the current game state as a tuple of scores for both players. A ratio may be calculated, considering the independent results too.
        :param method: The value of the ScoreMethod enum to determine the score calculation method.
        :return: The score as a tuple, on the first position being the score of the first player and on the second of the second player.
        """
        if method == ScoreMethod.totalValidMoves:
            totalValidMoves = [0, 0]
            for node in self.nodes:
                if node.getColor():
                    totalValidMoves[node.getColor().value] += len(self.getValidMoves(node))
            return totalValidMoves

        elif method == ScoreMethod.remainingPieces:
            remainingPieces = [0, 0]
            for node in self.nodes:
                if node.getColor():
                    remainingPieces[node.getColor().value] += 1
            return remainingPieces

        elif method == ScoreMethod.stabilitiesSum:
            stabilitiesSum = [0, 0]
            for node in self.nodes:
                if node.getColor():
                    stabilitiesSum[node.getColor().value] += self.getValence(node)[1]
            return stabilitiesSum

        elif method == ScoreMethod.totalStablePieces:
            totalStablePieces = [0, 0]
            for node in self.nodes:
                if node.getColor() and self.getValence(node)[1]:
                    totalStablePieces[node.getColor().value] += 1
            return totalStablePieces

    def getValidMoves(self, node):
        """
        Gets all the valid moves of the given node.
        :param node: The node to check valid moves for.
        :return: A list of all the adjacent nodes the piece can be moved to.
        """
        validMoves = []
        if node:
            idDifs = (
                -self.columnsNo - 2, -self.columnsNo - 1, -self.columnsNo, -1, +1, +self.columnsNo, +self.columnsNo + 1,
                +self.columnsNo + 2)
            for idDif in idDifs:
                neighborId = node.id + idDif
                if neighborId in range(len(self.nodes)):
                    neighbor = self.nodes[neighborId]
                    if self.adjacent(node, neighbor) and not neighbor.getColor():  # a free neighbor node
                        if self.validMove(node, neighbor):
                            validMoves.append(neighbor)
        return validMoves

    def validMove(self, node, neighbor):
        """
        Checks if the move from node to neighbor is valid.
        :param node: The node to move a piece from.
        :param neighbor: The node to move a piece onto.
        :return: False if there are still pieces to be placed for the current player or the valence of neighbor is not equal to the valence of node and True otherwise.
        """
        return (1 + self.getValence(node)[0]) == self.getValence(neighbor)[
            0]  # consider that one of node's neighbors is selecetedPiece

    def validPlacement(self, node, player):
        """
        Checks if the placement is valid or not.
        :param node: The node to place the piece onto.
        :param player: The player placing the piece.
        :return: True if it is the first piece placement or there are still pieces to be placed for the current player and the node's valence is strictly positive and false otherwise.
        """
        if player:
            if self.piecesPlaced[1] == Graph.totalPiecesOfPlayer:  # all black pieces were placed
                return False
        elif self.piecesPlaced[0] == Graph.totalPiecesOfPlayer:  # all white pieces were placed
            return False
        if self.piecesPlaced[0] == 0:  # no piece was placed
            return True

        return self.getValence(node)[0] > 0

    def getValence(self, node):
        """
        Returns the valence of node and whether it is stable or not.
        :param node: The node to calculate valence and stability for.
        :return: The valence of the node, representing a natural number between 0 and 8 for how many adjacent occupied by pieces nodes, and its stability, representing the number of neighbors of the opposite color.
        """
        valence = 0
        stability = 0
        idDifs = (
            -self.columnsNo - 2, -self.columnsNo - 1, -self.columnsNo, -1, +1, +self.columnsNo, +self.columnsNo + 1,
            +self.columnsNo + 2)
        for idDif in idDifs:
            neighborId = node.id + idDif
            if neighborId in range(len(self.nodes)):
                neighbor = self.nodes[neighborId]
                if self.adjacent(node, neighbor) and neighbor.getColor():  # an occupied neighbor node
                    valence += 1
                    if neighbor.getColor() != node.getColor():
                        stability += 1
        return valence, stability

    def drawPlayScreen(self, displayColor=(255, 255, 255), linesColor=(0, 0, 0), validMovesColor=(220, 247, 129),
                       winner=None):
        print(self)
        print("!-----------------H------------------!")
        print("Selected Piece: {}".format(Graph.selectedPiece))
        Graph.display.fill(displayColor)

        for edge in self.edges:
            p0 = self.nodes[edge.node0.id]
            p1 = self.nodes[edge.node1.id]
            if p0.x != p1.x and p0.y != p1.y:  # to not draw the diagonal edges
                continue
            pygame.draw.line(surface=Graph.display, color=linesColor, start_pos=p0.getCoords(), end_pos=p1.getCoords(),
                             width=5)

        validMoves = self.getValidMoves(Graph.selectedPiece)
        for node in self.nodes:
            color = linesColor
            radius = self.pointRadius
            if node in validMoves:
                color = validMovesColor
                radius *= 2.5

            pygame.draw.circle(surface=Graph.display, color=color, center=node.getCoords(), radius=radius,
                               width=0)  # width=0 face un cerc plin
            if node.getColor() == Color(0):
                if winner != 0:
                    Graph.display.blit(Images.whitePiece, (node.x - self.pieceRadius, node.y - self.pieceRadius))
                else:
                    Graph.display.blit(Images.winnerWhitePiece, (node.x - self.pieceRadius, node.y - self.pieceRadius))
            elif node.getColor() == Color(1):
                if winner != 1:
                    Graph.display.blit(Images.blackPiece, (node.x - self.pieceRadius, node.y - self.pieceRadius))
                else:
                    Graph.display.blit(Images.winnerBlackPiece, (node.x - self.pieceRadius, node.y - self.pieceRadius))

        if Graph.selectedPiece:
            if Graph.selectedPiece.getColor() == Color(0):
                Graph.display.blit(Images.selectedWhitePiece,
                                   (Graph.selectedPiece.x - self.pieceRadius, Graph.selectedPiece.y - self.pieceRadius))
            else:
                Graph.display.blit(Images.selectedBlackPiece,
                                   (Graph.selectedPiece.x - self.pieceRadius, Graph.selectedPiece.y - self.pieceRadius))
        pygame.display.update()

    def adjacent(self, n0, n1):
        """
        Checks wheter n0 and n1 are adjacent in the graph.
        :param n0: The first node.
        :param n1: The second node.
        :return: True if there exists an edge between node and n1 in the graph or False otherwise.
        """
        return (n0, n1) in [(edge.node0, edge.node1) for edge in self.edges]

    @classmethod
    def getInstance(cls):
        """
        Gets the singleton instance of the graph class. If it is None, it calls the __init__ method.
        :return: The singleton instance of the graph class.
        """
        if cls.instance:
            return cls.instance
        else:
            cls.instance = Graph()
            return cls.instance

    def initEdges(self):
        """
        Initializes and returns the edges of the graph.
        :return: The list of edges.
        """
        idDifs = (-self.columnsNo - 2, -self.columnsNo - 1, -self.columnsNo, -1)
        edges = []
        for node0 in self.nodes:
            if node0.lineIdx in range(1, Graph.linesNo) and node0.columnIdx in range(1, Graph.columnsNo):
                for idDif in idDifs:
                    node1 = self.nodes[node0.id + idDif]
                    edges += [Edge(node0, node1), Edge(node1, node0)]
            else:
                if node0.columnIdx != 0:
                    node1 = self.nodes[node0.id - 1]
                    edges += [Edge(node0, node1), Edge(node1, node0)]
                    if node0.lineIdx != 0:
                        node1 = self.nodes[node0.id - self.columnsNo - 2]
                        edges += [Edge(node0, node1), Edge(node1, node0)]
                if node0.lineIdx != 0:
                    node1 = self.nodes[node0.id - self.columnsNo - 1]
                    edges += [Edge(node0, node1), Edge(node1, node0)]
                    if node1.columnIdx < self.columnsNo:
                        node1 = self.nodes[node0.id - self.columnsNo]
                        edges += [Edge(node0, node1), Edge(node1, node0)]
        return edges

    def declareWinner(self, player=None):
        """
        Prints the winner in console, draws the winner screen and waits for the game to end.
        :param player: The winning player or None if the game was suspended.
        :return:
        """
        print("\nNPC min thinking time = {} | NPC max thinking time = {}.".format(self.minNPCThinkTime, self.maxNPCThinkTime))
        if player:
            print("Player {} [{}] has won!".format(player, Color(player).name))
            self.drawPlayScreen(winner=player)
            while True:
                for ev in pygame.event.get():
                    if ev.type == pygame.QUIT:
                        pygame.quit()
                        sys.exit()
        else:
            print("Game closed.")
            pygame.quit()
            sys.exit()

    def __repr__(self):
        s = "nodes:"
        for nod in self.nodes:
            s += "\n" + nod.__repr__()

        print("edges:")
        for edge in self.edges:
            s += "\n" + edge.__repr__()
        return s


class Edge:
    def __init__(self, node0, node1):
        self.node0 = node0
        self.node1 = node1

    def __repr__(self):
        s = self.node0.__repr__() + " - " + self.node1.__repr__()
        return s


def euclidDist(p0, p1):
    """
    Returns the euclidian distance between two screen points.
    :param p0: The first point as a touple of screen coordinates (abscissa first).
    :param p1: The second point as a touple of screen coordinates (abscissa first).
    :return: The euclidian distance between p0 and p1.
    """
    (x0, y0) = p0
    (x1, y1) = p1
    return math.sqrt((x0 - x1) ** 2 + (y0 - y1) ** 2)


class Node:
    idCount = 0

    def __init__(self, columnIdx: int, lineIdx: int, id: int):
        self.columnIdx = columnIdx
        self.lineIdx = lineIdx
        self.id = id
        self.x = None
        self.y = None

    def getCoords(self):
        """
        Gets the screen coordinates of the Node.
        :return: A touple of x and y screen coordinates of the Node.
        """
        return self.x, self.y

    def getColor(self):
        """
        Gets the color of the node.
        :return: None or a color from Color enum.
        """
        return Graph.lDColors[Graph.lDColorsIdx][self.id]

    def setColor(self, color):
        """
        Sets the color of the node.
        :param color: None or a color from Color enum.
        :return:
        """
        Graph.lDColors[Graph.lDColorsIdx][self.id] = color

    @classmethod
    def getNextId(cls):
        """
        Increments the static idCount and returns its former value.
        :return: The next id to be assigned.
        """
        cls.idCount += 1
        return cls.idCount - 1

    def __repr__(self):
        colorSymbol = None if not self.getColor() else self.getColor().name
        s = "#{}[{}]: ({}, {})".format(self.id, colorSymbol, self.columnIdx, self.lineIdx)
        return s


def binarySearchInsertPos(array, crtScoreStats):
    """ Finds the position where crtScoreStats should be inserted.
        :param: array: Array of ScoreStats ascending ordered by ScoreStats.__lt__ and ScoreStats.__eq__.
        :param: crtScoreStats: The ScoreStats object to be inserted.
        :return: The position where crtScoreStats should be inserted."""
    arrSize = len(array)
    if arrSize == 0 or array[arrSize - 1] <= crtScoreStats:
        return arrSize  # in this case, an append is needed
    if array[0] > crtScoreStats:
        return 0

    power2 = 1
    while power2 < arrSize:  # the least power of 2 that is >= arrSize
        power2 <<= 1

    infimumIdx = 0
    while power2:
        if infimumIdx + power2 < arrSize and array[infimumIdx + power2] <= crtScoreStats:
            infimumIdx += power2
        power2 >>= 1

    return 1 + infimumIdx


@total_ordering
class ScoreStats:
    def __init__(self, destNodeId, sourceNodeId, score, player, gamePhase, extraScore=None):
        """
        Object containing the scores of a game state as tuples that contain on position 0 first player's score and on second position second player's score.
        :param destNodeId: The id of the destination node of a piece placement or move.
        :param sourceNodeId: The id of the node being moved to an adjacent valid node. May be None if gamePhase is GamePhase.PLACE.
        :param score: The tuple of scores for both players.
        :param player: The player to act next.
        :param gamePhase: The phase of the game, wether it is still in the placing phase or the moving phase.
        :param score: The tuple of extra scores for both players. It is used for more euristics at once to compare two ScoreStats mainly.
        """
        self.destNodeId = destNodeId
        self.sourceNodeId = sourceNodeId
        self.score = score
        self.player = player
        self.gamePhase = gamePhase
        self.extraScore = extraScore

    def __lt__(self, other):
        """
        Compares the given score stats and returns True if scores1 is less than scores2 overall and False otherwise.
        :param self: The first ScoreStats object.
        :param other: The second ScoreStats object.
        :param player: The player to act next.
        :param gamePhase: The phase of the game, wether it is still in the placing phase or the moving phase.
        :return: True if scores1 is less than scores2 overall and False otherwise.
        """
        # Ratios comparison first
        if not self.getScoreRatio(self.player) == other.getScoreRatio(self.player):
            return self.getScoreRatio(self.player) < other.getScoreRatio(self.player)

        # Opponent scores comparison now (the comparison operator is reversed from the previous cases)
        if not self.score[1 - self.player] == other.score[1 - self.player]:
            return self.score[1 - self.player] > other.score[1 - self.player]

        if self.extraScore and other.extraScore:
            if not self.getExtraScoreRatio(self.player) == other.getExtraScoreRatio(self.player):
                return self.getExtraScoreRatio(self.player) < other.getExtraScoreRatio(self.player)

            if not self.extraScore[self.player] == other.extraScore[self.player]:
                return self.extraScore[self.player] < other.extraScore[self.player]

        return False

    def __eq__(self, other):
        return self.getScoreRatio(self.player) == other.getScoreRatio(self.player) \
            and self.score[1 - self.player] == other.score[1 - self.player] \
            and (not self.extraScore or not other.extraScore or (self.getExtraScoreRatio(self.player) == other.getExtraScoreRatio(self.player) and self.extraScore[self.player] == other.extraScore[self.player]))

    def getScoreRatio(self, player):
        """
        Gets the ratio of the score for the given player.
        :param player: The given player.
        :return: The ratio of the score for the given player as the difference between the player's score and the opponent's score.
        """
        return self.score[player] - self.score[1 - player]

    def getExtraScoreRatio(self, player):
        """
        Gets the ratio of the extraScore for the given player.
        :param player: The given player.
        :return: The ratio of the extraScore for the given player as the difference between the player's score and the opponent's score.
        """
        return self.extraScore[player] - self.extraScore[1 - player]


# ############ display initial ####################### #
def drawChoices(display):
    """
    Displays the initial game options on display screen for the current graph and starts the game.
    :param display: The display screen.
    :param crtGraph: The current graph to start the game from.
    :return: The chosen algorithm, player, difficulty and game mode.
    """
    algBtnGroup = ButtonsGroup(
        top=30,
        left=30,
        lButtons=[
            Button(display=display, width=80, height=30, text="minimax", value=Algorithm.MINMAX.name),
            Button(display=display, width=80, height=30, text="alphabeta", value=Algorithm.ALPHABETA.name)
        ],
        selectedIdx=1)
    playerBtnGroup = ButtonsGroup(
        top=100,
        left=30,
        lButtons=[
            Button(display=display, width=50, height=30, text=Color(0).name.lower(), value='0'),
            Button(display=display, width=50, height=30, text=Color(1).name.lower(), value='0')
        ],
        selectedIdx=0)
    difficultyBtnGroup = ButtonsGroup(
        top=170,
        left=30,
        lButtons=[
            Button(display=display, width=60, height=30, text="easy", value="easy"),
            Button(display=display, width=75, height=30, text="medium", value="medium"),
            Button(display=display, width=60, height=30, text="hard", value="hard")
        ],
        selectedIdx=0)
    gameModeBtnGroup = ButtonsGroup(
        top=240,
        left=30,
        lButtons=[
            Button(display=display, width=60, height=30, text="PvP", value="PvP"),
            Button(display=display, width=75, height=30, text="PvE", value="PvE"),
            Button(display=display, width=60, height=30, text="EvE", value="EvE")
        ],
        selectedIdx=1)
    ok = Button(display=display, top=310, left=30, width=40, height=30, text="ok", backgroundColor=(155, 0, 55))
    algBtnGroup.draw()
    playerBtnGroup.draw()
    difficultyBtnGroup.draw()
    gameModeBtnGroup.draw()
    ok.draw()
    while True:
        for ev in pygame.event.get():
            if ev.type == pygame.QUIT:
                pygame.quit()
                sys.exit()

            elif ev.type == pygame.MOUSEBUTTONDOWN:
                pos = pygame.mouse.get_pos()
                if not algBtnGroup.selectByCoords(pos):
                    if not playerBtnGroup.selectByCoords(pos):
                        if not difficultyBtnGroup.selectByCoords(pos):
                            if not gameModeBtnGroup.selectByCoords(pos):
                                if algBtnGroup.anySelected() and playerBtnGroup.anySelected() and difficultyBtnGroup.anySelected() and gameModeBtnGroup.anySelected():
                                    if ok.selectByCoords(pos):
                                        display.fill((0, 0, 0))  # stergere display
                                        return algBtnGroup.getValue(), int(
                                            playerBtnGroup.getValue()), difficultyBtnGroup.getValue(), gameModeBtnGroup.getValue()
        pygame.display.update()


class ButtonsGroup:
    def __init__(self, lButtons=[], selectedIdx=0, buttonsSpacing=10, left=0, top=0):
        self.lButtons = lButtons
        self.selectedIdx = selectedIdx
        self.lButtons[self.selectedIdx].selected = True
        self.top = top
        self.left = left
        crtLeft = self.left
        for button in self.lButtons:
            button.top = self.top
            button.left = crtLeft
            button.updateRectangle()
            crtLeft += (buttonsSpacing + button.width)

    def selectByCoords(self, coords):
        oldSelected = self.anySelected()
        for buttonIdx, button in enumerate(self.lButtons):
            if button.selectByCoords(coords):
                newSelected = not oldSelected if self.selectedIdx == buttonIdx else False
                self.lButtons[self.selectedIdx].select(newSelected)
                self.selectedIdx = buttonIdx
                return True
        return False

    def draw(self):
        # atentie, nu face wrap
        for b in self.lButtons:
            b.draw()

    def getValue(self):
        return self.lButtons[self.selectedIdx].value

    def anySelected(self):
        """
        Checks whether there is any button selected in the group.
        :return: True if the button at selectedIdx is selected or else otherwise.
        """
        return self.lButtons[self.selectedIdx].selected


class Button:
    def __init__(self, display=None, left=0, top=0, width=0, height=0, backgroundColor=(53, 80, 115),
                 selectedBackgroundColor=(89, 134, 194), text="", font="arial", fontSize=16, textColor=(255, 255, 255),
                 value=""):
        self.left = left
        self.top = top
        self.display = display
        self.backgroundColor = backgroundColor
        self.selectedBackgroundColor = selectedBackgroundColor
        self.text = text
        self.font = font
        self.width = width
        self.height = height
        self.selected = False
        self.fontSize = fontSize
        self.textColor = textColor
        # creez obiectul font
        fontObj = pygame.font.SysFont(self.font, self.fontSize)
        self.renderedText = fontObj.render(self.text, True, self.textColor)
        self.rectangle = pygame.Rect(left, top, width, height)
        # aici centram textul
        self.textRectangle = self.renderedText.get_rect(center=self.rectangle.center)
        self.value = value

    def select(self, selected):
        self.selected = selected
        self.draw()

    def selectByCoords(self, coords):
        if self.rectangle.collidepoint(coords):
            self.select(True)
            return True
        return False

    def updateRectangle(self):
        self.rectangle.left = self.left
        self.rectangle.top = self.top
        self.textRectangle = self.renderedText.get_rect(center=self.rectangle.center)

    def draw(self):
        backgroundColor = self.selectedBackgroundColor if self.selected else self.backgroundColor
        pygame.draw.rect(self.display, backgroundColor, self.rectangle)
        self.display.blit(self.renderedText, self.textRectangle)


class Images:
    whitePiece = pygame.image.load('checkerWhite.png')
    blackPiece = pygame.image.load('checkerBlack.png')
    selectedWhitePiece = pygame.image.load('checkerWhiteSelected.png')
    selectedBlackPiece = pygame.image.load('checkerBlackSelected.png')
    winnerWhitePiece = pygame.image.load('checkerWhiteWinner.png')
    winnerBlackPiece = pygame.image.load('checkerBlackWinner.png')

    @classmethod
    def rescale(cls, pieceRadius):
        """
        Rescales the piece images by the given pieceRadius.
        :param pieceRadius: The radius to rescale the images by.
        :return:
        """
        cls.whitePiece = pygame.transform.scale(cls.whitePiece, (pieceRadius, pieceRadius))
        cls.blackPiece = pygame.transform.scale(cls.blackPiece, (pieceRadius, pieceRadius))
        cls.selectedWhitePiece = pygame.transform.scale(cls.selectedWhitePiece, (pieceRadius, pieceRadius))
        cls.selectedBlackPiece = pygame.transform.scale(cls.selectedBlackPiece, (pieceRadius, pieceRadius))
        cls.winnerWhitePiece = pygame.transform.scale(cls.winnerWhitePiece, (pieceRadius, pieceRadius))
        cls.winnerBlackPiece = pygame.transform.scale(cls.winnerBlackPiece, (pieceRadius, pieceRadius))


class ScoreMethod(Enum):
    """
    The methods used to calculate the score of a game state.
    """
    totalValidMoves = 0
    remainingPieces = 1
    stabilitiesSum = 2
    totalStablePieces = 3


class PlayerType(Enum):
    """
    The player type.
    """
    HUMAN = 0
    NPC = 1


class Color(Enum):
    """
    The colors of the players (0 as first player and 1 as second player).
    """
    WHITE = 0
    BLACK = 1


class Algorithm(Enum):
    """
    The algorithm to calculate next moves.
    """
    MINMAX = 0
    ALPHABETA = 1


class GamePhase(Enum):
    """
    The phase of the game, wether it is still in the placing phase or the moving phase.
    """
    PLACE = 0
    MOVE = 1


if __name__ == "__main__":
    main()
