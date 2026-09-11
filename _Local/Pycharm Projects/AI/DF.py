import queue

from Base import GraphBase as gBase, NodParcurgereBase as npBase


class GraphDF(gBase.Graph):
    def __init__(self, noduri, matriceAdiacenta, start, scopuri):
        super(GraphDF, self).__init__(noduri, matriceAdiacenta, start, scopuri)

    def depthFirst(self, nrSolutiiCautate=1, *args):
        # vom simula o stiva prin relatia de parinte a nodului curent
        if args:
            self.df(npBase.NodParcurgere(self.indiceNod(self.start), self.start, None), nrSolutiiCautate, args[0])
        else:
            self.df(npBase.NodParcurgere(self.indiceNod(self.start), self.start, None), nrSolutiiCautate)

    def df(self, nodCurent: npBase.NodParcurgere, nrSolutiiCautate, *args):
        if nrSolutiiCautate <= 0:  # testul acesta s-ar valida doar daca in apelul initial avem df(start,if nrSolutiiCautate=0)
            return nrSolutiiCautate
        # print("Stiva actuala: " + "->".join(nodCurent.obtineDrum()))
        if self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
            if nrSolutiiCautate <= 0:
                return nrSolutiiCautate

        lSuccesori = self.genereazaSuccesori(nodCurent)
        for succesor in lSuccesori:
            if nrSolutiiCautate > 0:
                # print("Se expandeaza " + nodCurent.__repr__())
                nrSolutiiCautate = self.df(succesor, nrSolutiiCautate)
            else:
                break
        # print("Se intoarce -> ")
        return nrSolutiiCautate

    def depthFirstIterativ(self, nrSolutiiCautate=1, *args):
        for i in range(1, self.nrNoduri + 1):
            if nrSolutiiCautate <= 0:
                return
            # print("**************\nAdancime maxima: ", i)
            if args:
                nrSolutiiCautate = self.dfi(npBase.NodParcurgere(self.indiceNod(self.start), self.start, None), i,
                                        nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.dfi(npBase.NodParcurgere(self.indiceNod(self.start), self.start, None), i,
                                            nrSolutiiCautate)

    def dfi(self, nodCurent: npBase.NodParcurgere, adancime, nrSolutiiCautate, *args):
        # print("Stiva actuala: " + "->".join(nodCurent.obtineDrum()))
        if adancime == 1 and self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
            if nrSolutiiCautate <= 0:
                return nrSolutiiCautate

        if adancime > 1:
            lSuccesori = self.genereazaSuccesori(nodCurent)
            for succesor in lSuccesori:
                if nrSolutiiCautate > 0:
                    nrSolutiiCautate = self.dfi(succesor, adancime - 1, nrSolutiiCautate)
        return nrSolutiiCautate

    def depthFirstLIFO(self, nrSolutiiCautate=1, *args):
        nodCurent = npBase.NodParcurgere(self.indiceNod(self.start), self.start, None)
        if self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
            if nrSolutiiCautate <= 0:
                return nrSolutiiCautate

        viz = [False]*self.nrNoduri
        viz[nodCurent.id] = True
        stiva = queue.LifoQueue()
        stiva.put(nodCurent)

        while(not stiva.empty()):
            # print("Stiva actuala: " + "->".join(nodCurent.obtineDrum()))
            nodCurent = stiva.get()     # actualizam nodul curent
            stiva.put(nodCurent)        # si il reasezam pe stiva
            if self.testeazaScop(nodCurent):
                if args:
                    nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
                else:
                    nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
                if nrSolutiiCautate <= 0:
                    return nrSolutiiCautate

            lSuccesori = self.genereazaSuccesori(nodCurent)
            for succesor in lSuccesori:
                if not viz[succesor.id]:
                    viz[succesor.id] = True
                    stiva.put(succesor)
                    break               # luam primul succesor nevizitat si il urcam pe stiva
            varf = stiva.get()          # verificam varful stivei
            if varf != nodCurent:       # daca e difierit de nodCurent, inseamna ca a avut un succesor nevizitat
                stiva.put(varf)         # si il reasezam pe stiva

    def genereazaSuccesori(self, nodCurent, *args):
        lSuccesori = []
        for i in range(self.nrNoduri):
            if self.matriceAdiacenta[nodCurent.id][i] == 1 and not nodCurent.contineInDrum(self.noduri[i]):
                nodNou = npBase.NodParcurgere(i, self.noduri[i], nodCurent)
                lSuccesori.append(nodNou)
        return lSuccesori
