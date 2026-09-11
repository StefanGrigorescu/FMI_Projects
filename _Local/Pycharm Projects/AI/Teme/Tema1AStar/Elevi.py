# A* Message Problem (Problema elevilor)

import queue
import time

from AStar import GraphAStar
from AStar import NodParcurgereAStar


class NodParcurgereElevi(NodParcurgereAStar):
    def __init__(self, id, info, parinte, cost, h, idxLNumeAscultati, timpRamasAscultare, symbol):
        super(NodParcurgereElevi, self).__init__(id, info, parinte, cost, h)
        self.idxLNumeAscultati = idxLNumeAscultati
        self.timpRamasAscultare = timpRamasAscultare
        self.symbol = symbol

    def afisDrum(self):  # returneaza si lungimea drumului
        drumul = ""
        lungimea = 0
        nod = self
        while nod is not None:
            drumul = "{} {}(#{}) ".format(nod.symbol, nod.info, str(nod.id)) + drumul
            nod = nod.parinte
            lungimea += 1

        drumul = "lungimea = {} | costul = {} : ".format(lungimea, self.g) + drumul
        print(drumul)
        return drumul, lungimea

    def __repr__(self):
        sir = super().__repr__()[0:-1]
        sir += "idxLNumeAscultati = {}".format(self.idxLNumeAscultati)
        sir += "timpRamasAscultare = {}".format(self.timpRamasAscultare)

        sir += ")"
        return sir


class GraphElevi(GraphAStar):
    deltaCoordonateVecini = ((-1, 0, "^"), (1, 0, "v"), (0, -1, "<"), (0, 1, ">"))

    def __init__(self, noduri, matriceAdiacenta, matricePonderi, start, scopuri, listaH, k, matElevi, dElevi, lNumeAscultati, m):
        super(GraphElevi, self).__init__(noduri, matriceAdiacenta, matricePonderi, start, scopuri, listaH)
        self.k = k          # numarul de banci
        self.matElevi = matElevi
        self.dElevi = dElevi
        self.lNumeAscultati = lNumeAscultati
        self.m = m          # numarul de unitati de timp (echivalente cu o mutare in graful de parcurgere) cat dureaza o ascultare

        self.nrOrdineSolutie = 0
        self.startTime = time.time()
        self.nrMaxNoduri = 0
        self.nrCrtNoduri = 0
        self.nrTotalNoduri = 0

    def aStarOpt(self, nrSolutiiCautate=1, *args):
        posibilSolutie = True
        if args:
            posibilSolutie = self.preAlgoritm(args[0])
        else:
            posibilSolutie = self.preAlgoritm()
        if not posibilSolutie:
            return

        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere) candidate pentru expandare
        lDeschise = [NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, "")]
        dInchise = dict([])  # nodurile expandate

        while lDeschise:  # while lDeschise is not empty
            avemNod = False
            while True:
                nodCurent = lDeschise.pop(0)  # selecteaza nodCurent
                if not (nodCurent.id in dInchise and nodCurent >= dInchise[nodCurent.id]):
                    avemNod = True
                    break
                else:
                    self.nrCrtNoduri -= 1
                if not lDeschise:
                    break
            if not avemNod:
                break

            dInchise[nodCurent.id] = nodCurent
            if self.testeazaScop(nodCurent):
                if args:
                    nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
                else:
                    nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
                if nrSolutiiCautate <= 0:
                    break

            lSuccesori = self.genereazaSuccesori(nodCurent)

            self.actualizeazaContoareNoduri(len(lSuccesori))

            for succesor in lSuccesori:
                pozInserare = self.cautareBinaraPozInserare(lDeschise, len(lDeschise), succesor)
                if pozInserare < len(lDeschise):
                    lDeschise.insert(pozInserare, succesor)
                else:
                    lDeschise.append(succesor)

    def aStarDict(self, nrSolutiiCautate=1, *args):
        pass

    def aStar(self, nrSolutiiCautate=1, *args):
        posibilSolutie = True
        if args:
            posibilSolutie = self.preAlgoritm(args[0])
        else:
            posibilSolutie = self.preAlgoritm()
        if not posibilSolutie:
            return

        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere) candidate pentru expandare
        lDeschise = [NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, "")]
        lInchise = []  # nodurile expandate

        while len(lDeschise) > 0:
            # print("lDeschise: " + str(lDeschise))
            # print("lInchise = " + str(lInchise))

            nodCurent = lDeschise.pop(0)  # selecteaza nodCurent
            self.nrCrtNoduri -= 1
            lInchise.append(nodCurent)
            if self.testeazaScop(nodCurent):
                if args:
                    nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
                else:
                    nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
                if nrSolutiiCautate <= 0:
                    break

            lSuccesori = self.genereazaSuccesori(nodCurent)
            # print("lSuccesori: " + str(lSuccesori))
            for succesor in lSuccesori:
                inDeschise = False
                for nodDeschis in lDeschise:
                    if succesor.info == nodDeschis.info:
                        inDeschise = True
                        if succesor.f >= nodDeschis.f:
                            lSuccesori.remove(succesor)
                        else:
                            lDeschise.remove(nodDeschis)
                        break
                if not inDeschise:
                    for nodInchis in lInchise:
                        if succesor.info == nodInchis.info:
                            if succesor.f >= nodInchis.f:
                                lSuccesori.remove(succesor)
                            else:  # s.f<nodDeschis.f
                                lInchise.remove(nodInchis)
                            break

            self.actualizeazaContoareNoduri(len(lSuccesori))

            for succesor in lSuccesori:
                pozInserare = self.cautareBinaraPozInserare(lDeschise, len(lDeschise), succesor)
                if pozInserare < len(lDeschise):
                    lDeschise.insert(pozInserare, succesor)
                else:
                    lDeschise.append(succesor)

    def idAStar(self, nrSolutiiCautate=1, *args):
        posibilSolutie = True
        if args:
            posibilSolutie = self.preAlgoritm(args[0])
        else:
            posibilSolutie = self.preAlgoritm()
        if not posibilSolutie:
            return

        nodStart = NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, "")
        limita = nodStart.f

        while True:
            # print("Limita de pornire: ", limita)
            if args:
                nrSolutiiCautate, rez = self.construiesteDrum(nodStart, limita, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate, rez = self.construiesteDrum(nodStart, limita, nrSolutiiCautate)
            if rez == "gata":
                break
            if rez == float('inf'):
                # print("Nu mai exista solutii!")
                break
            limita = rez
            # print(">>> Limita noua: ", limita)

    def breadthFirstQ(self, nrSolutiiCautate=1, *args):
        posibilSolutie = True
        if args:
            posibilSolutie = self.preAlgoritm(args[0])
        else:
            posibilSolutie = self.preAlgoritm()
        if not posibilSolutie:
            return

        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere)
        c = queue.SimpleQueue()
        nodCurent = NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, "")
        c.put(nodCurent)

        if self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])

        if nrSolutiiCautate > 0:
            while not c.empty():
                nodCurent = c.get()

                if self.testeazaScop(nodCurent):
                    if args:
                        nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
                    else:
                        nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])

                lSuccesori = self.genereazaSuccesori(nodCurent, nrSolutiiCautate)

                self.actualizeazaContoareNoduri(len(lSuccesori))

                if nrSolutiiCautate <= 0:
                    break
                for succesor in lSuccesori:
                    c.put(succesor)

    def depthFirst(self, nrSolutiiCautate=1, *args):
        # vom simula o stiva prin relatia de parinte a nodului curent
        posibilSolutie = True
        if args:
            posibilSolutie = self.preAlgoritm(args[0])
        else:
            posibilSolutie = self.preAlgoritm()
        if not posibilSolutie:
            return

        if args:
            self.df(NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, ""), nrSolutiiCautate, args[0])
        else:
            self.df(NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, ""), nrSolutiiCautate)

    def depthFirstIterativ(self, nrSolutiiCautate=1, *args):
        posibilSolutie = True
        if args:
            posibilSolutie = self.preAlgoritm(args[0])
        else:
            posibilSolutie = self.preAlgoritm()
        if not posibilSolutie:
            return

        for i in range(1, self.nrNoduri + 1):
            if nrSolutiiCautate <= 0:
                return
            # print("**************\nAdancime maxima: ", i)
            if args:
                nrSolutiiCautate = self.dfi(NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, ""), i,
                                        nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.dfi(NodParcurgereElevi(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start), 0, self.m, ""), i,
                                            nrSolutiiCautate)

    def construiesteDrum(self, nodCurent, limita, nrSolutiiCautate, *args):
        # print("A ajuns la: ", nodCurent)
        if nodCurent.f > limita:
            return nrSolutiiCautate, nodCurent.f
        if nodCurent.f == limita and self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScopIdAStar(nodCurent, limita, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScopIdAStar(nodCurent, limita, nrSolutiiCautate)
            if nrSolutiiCautate <= 0:
                return 0, "gata"
        lSuccesori = self.genereazaSuccesori(nodCurent)

        self.actualizeazaContoareNoduri(len(lSuccesori))

        minim = float('inf')
        for succesor in lSuccesori:
            if args:
                nrSolutiiCautate, rez = self.construiesteDrum(succesor, limita, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate, rez = self.construiesteDrum(succesor, limita, nrSolutiiCautate)
            if rez == "gata":
                return 0, "gata"
            # print("Compara ", rez, " cu ", minim)
            if rez < minim:
                minim = rez
                # print("Noul minim: ", minim)
        return nrSolutiiCautate, minim

    def df(self, nodCurent, nrSolutiiCautate, *args):
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

        self.actualizeazaContoareNoduri(len(lSuccesori))

        for succesor in lSuccesori:
            if nrSolutiiCautate > 0:
                # print("Se expandeaza " + nodCurent.__repr__())
                if args:
                    nrSolutiiCautate = self.df(succesor, nrSolutiiCautate, args[0])
                else:
                    nrSolutiiCautate = self.df(succesor, nrSolutiiCautate)
            else:
                break
        # print("Se intoarce -> ")
        return nrSolutiiCautate

    def dfi(self, nodCurent, adancime, nrSolutiiCautate, *args):
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

            self.actualizeazaContoareNoduri(len(lSuccesori))

            for succesor in lSuccesori:
                if nrSolutiiCautate > 0:
                    nrSolutiiCautate = self.dfi(succesor, adancime - 1, nrSolutiiCautate)
        return nrSolutiiCautate

    def preAlgoritm(self, *args):
        """
        Initializeaza datele solicitate pentru afisare si verifica daca gasirea unei solutii este posibila sau nu.
        :param: args: Contine cel mult un parametru auxiliar ce prezinta calea catre fisierul de scriere.
        :return: True daca gasirea unei solutii este posibila sau False in caz contrar.
        """
        self.nrOrdineSolutie = 0
        self.startTime = time.time()
        self.nrCrtNoduri = self.nrMaxNoduri = self.nrTotalNoduri = 1
        if not self.posibilSolutie():
            print("Nu exista solutie.")
            if args:
                fout = open(args[0], "a")
                fout.write("NU exista solutie.")
            return False
        return True

    def actualizeazaContoareNoduri(self, lungime):
        """
        Actualizeaza nrCrtNoduri, nrMaxNoduri si nrTotal noduri in functie de lungimea listei de succesori.
        :param lungime: Lungimea listei de succesori.
        :return:
        """
        self.nrTotalNoduri += lungime
        self.nrCrtNoduri += lungime
        if self.nrMaxNoduri < self.nrCrtNoduri:
            self.nrMaxNoduri = self.nrCrtNoduri
        self.nrCrtNoduri -= 1   # putem sa scadem nodul curent acum

    def posibilSolutie(self):
        """
        :return: True daca este posibila obtinerea unei solutii din nodul respectiv sau False in caz contrar.
        """
        lastI1 = len(self.matElevi) - 1
        lastI2 = lastI1 - 1
        trecerePosibila = False
        for j in range(0, self.k-1):
            trecerePosibila = False
            numeSt1 = self.matElevi[lastI1][2*j+1].nume
            numeDr1 = self.matElevi[lastI1][2*j+2].nume
            if numeDr1 not in self.dElevi[numeSt1].sNumeSuparati:
                trecerePosibila = True
            elif lastI2 > 0:
                numeSt2 = self.matElevi[lastI2][2*j+1].nume
                numeDr2 = self.matElevi[lastI2][2*j+2].nume
                if numeDr2 not in self.dElevi[numeSt2].sNumeSuparati:
                    trecerePosibila = True
            if not trecerePosibila:
                break
        return trecerePosibila

    def genereazaSuccesori(self, nodCurent, *args):
        listaSuccesori = []

        idxLNumeAscultati = nodCurent.idxLNumeAscultati
        timpRamasAscultare = nodCurent.timpRamasAscultare - 1
        if timpRamasAscultare == 0:
            timpRamasAscultare = self.m
            idxLNumeAscultati += 1

        idxLinieNodCurent = self.dElevi[nodCurent.info].idxLinie
        idxColoanaNodCurent = self.dElevi[nodCurent.info].idxColoana

        for (i, j, symbol) in self.deltaCoordonateVecini:
            i += idxLinieNodCurent
            j += idxColoanaNodCurent
            if i not in range(0, len(self.matElevi)):
                continue
            if j not in range(0, self.k*2):
                continue
            idVecin = self.matElevi[i][j].idxLNumeElevi

            if self.blocatDeAscultare(i, j, nodCurent.idxLNumeAscultati):
                continue

            if self.getBanca(idxColoanaNodCurent) != self.getBanca(j):
                symbol = symbol+symbol

            if self.matriceAdiacenta[nodCurent.id][idVecin] == 1 and not nodCurent.contineInDrum(self.noduri[idVecin]):
                nodNou = NodParcurgereElevi(idVecin, self.noduri[idVecin], nodCurent, nodCurent.g + self.matricePonderi[nodCurent.id][idVecin], self.calculeazaH(self.noduri[idVecin]), idxLNumeAscultati, timpRamasAscultare, symbol)
                listaSuccesori.append(nodNou)
        return listaSuccesori

    def gasitScop(self, nodCurent, nrSolutiiCautate, *args):
        endTime = time.time() - self.startTime
        print("Solutie #{}: timp total = {} | nrMaxNoduri in memorie = {} | nrTotalNoduri in memorie = {} | ".format(self.nrOrdineSolutie, endTime, self.nrMaxNoduri, self.nrTotalNoduri), end="")

        drumul, lungimea = nodCurent.afisDrum()
        nrSolutiiCautate -= 1

        if args:
            path = args[0]
            fout = open(path, "a")
            fout.write("Solutie #{}: timp total = {} | nrMaxNoduri in memorie = {} | nrTotalNoduri in memorie = {} | ".format(self.nrOrdineSolutie, endTime, self.nrMaxNoduri, self.nrTotalNoduri) + drumul + "\n")
            fout.close()

        self.nrOrdineSolutie += 1
        return nrSolutiiCautate

    def blocatDeAscultare(self, i, j, idxLNumeAscultati):
        """
        Returneaza True daca pozitia respectiva din matricea de elevi este idisponibila din cauza ascultarii in vecinatate.
        :param i: coordonata pe linii a pozitiei de verificat.
        :param j: coordonata pe coloana a pozitiei de verificat.
        :param idxLNumeAscultati: indexul corespunzator elevului din lNumeAscultati ascultat in prezent.
        :return: True daca pozitia respectiva din matricea de elevi este idisponibila.
        """
        if idxLNumeAscultati >= len(self.lNumeAscultati):
            return False

        bancaVecin = self.getBanca(j)

        elevAscultat = self.dElevi[self.lNumeAscultati[idxLNumeAscultati]]
        iElevAscultat = elevAscultat.idxLinie
        jElevAscultat = elevAscultat.idxColoana
        bancaElevAscultat = self.getBanca(jElevAscultat)
        # pe aceeasi linie si in aceeasi banca/ banci alaturate    sau    in aceeasi banca si pe linii alaturate
        return (i - iElevAscultat == 0 and abs(bancaVecin - bancaElevAscultat) <= 1) or (
                bancaVecin - bancaElevAscultat == 0 and abs(i - iElevAscultat) == 1)

    @staticmethod
    def getBanca(idxColoana):
        """
        Returneaza indexul (pe coloane) al bancii in functie de indexul pe coloane al elevului.
        :param idxColoana: indexul pe coloane al elevului.
        :return: indexul (pe coloane) al bancii
        """
        return idxColoana // 2

    def indiceNod(self, nod):
        return self.dElevi[nod].idxLNumeElevi


class Elev:
    def __init__(self, nume, idxLinie, idxColoana, idxLNumeElevi=0):
        """

        :param nume: Numele elevului.
        :param idxLinie: Indexul elevului pe linie in matricea ce reprezinta asezarea elevilor in clasa.
        :param idxColoana: Indexul elevului pe coloana in matricea ce reprezinta asezarea elevilor in clasa.
        :param idxLNumeElevi: Indexul elevului in lista numelor elevilor (cu aparitie unica).
        """
        self.nume = nume
        self.idxLinie = idxLinie
        self.idxColoana = idxColoana
        self.idxLNumeElevi = idxLNumeElevi
        self.sNumeSuparati = set([])

    def __repr__(self):
        sir = ""
        sir += self.nume + "("
        # sir += "idxLinie = {}, ".format(self.idxLinie)
        # sir += "idxColoana = {}, ".format(self.idxColoana)
        sir += "idxLNumeElevi = {}, ".format(self.idxLNumeElevi)
        # sir += "len(sNumeSuparati) = {} ".format(len(self.sNumeSuparati))
        sir += ")"
        return sir

