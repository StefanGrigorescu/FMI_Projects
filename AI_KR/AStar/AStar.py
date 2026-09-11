import queue
from functools import total_ordering
from Base import GraphBase as gBase, NodParcurgereBase as npBase


@total_ordering
class NodParcurgereAStar(npBase.NodParcurgere):
    graf = None

    def __init__(self, id, info, parinte, cost, h):
        super(NodParcurgereAStar, self).__init__(id, info, parinte)
        self.g = cost
        self.h = h
        self.f = self.g + self.h

    def __eq__(self, other):
        if self.f == other.f and self.g == other.g:
            return True
        return False

    def __lt__(self, other):
        if self.f < other.f:
            return True
        elif self.f == other.f and self.g > other.g:
            return True
        return False

    def __repr__(self):
        sir = super().__repr__()[0:-1]
        sir += " g:{}".format(self.g)
        sir += " h:{}".format(self.h)

        sir += " f:{})".format(self.f)
        return sir


class GraphAStar(gBase.Graph):
    def __init__(self, noduri, matriceAdiacenta, matricePonderi, start, scopuri, listaH):
        super(GraphAStar, self).__init__(noduri, matriceAdiacenta, start, scopuri)
        self.matricePonderi = matricePonderi
        self.listaH = listaH

    def aStarOpt(self, nrSolutiiCautate=1, *args):
        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere) candidate pentru expandare
        lDeschise = [NodParcurgereAStar(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start))]
        dInchise = dict([])  # nodurile expandate

        while lDeschise:  # while lDeschise is not empty
            avemNod = False
            while True:
                nodCurent = lDeschise.pop(0)  # selecteaza nodCurent
                if not (nodCurent.id in dInchise and nodCurent >= dInchise[nodCurent.id]):
                    avemNod = True
                    break
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
            for succesor in lSuccesori:
                pozInserare = self.cautareBinaraPozInserare(lDeschise, len(lDeschise), succesor)
                if pozInserare < len(lDeschise):
                    lDeschise.insert(pozInserare, succesor)
                else:
                    lDeschise.append(succesor)

    def aStarDict(self, nrSolutiiCautate=1, *args):
        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere) candidate pentru expandare
        lDeschise = [NodParcurgereAStar(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start))]
        dInchise = dict([])  # nodurile expandate

        while len(lDeschise) > 0:
            # print("lDeschise: " + str(lDeschise))
            # print("lInchise = " + str(lInchise))

            nodCurent = lDeschise.pop(0)  # selecteaza nodCurent
            print("nodCurent: " + str(nodCurent.id))
            dInchise[nodCurent.id] = nodCurent
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
                    for k, v in dInchise.items():
                        if succesor.info == v.info:
                            if succesor.f >= v.f:
                                lSuccesori.remove(succesor)
                            else:  # s.f<nodDeschis.f
                                del dInchise[k]
                            break

            for succesor in lSuccesori:
                pozInserare = self.cautareBinaraPozInserare(lDeschise, len(lDeschise), succesor)
                if pozInserare < len(lDeschise):
                    lDeschise.insert(pozInserare, succesor)
                else:
                    lDeschise.append(succesor)

    def aStar(self, nrSolutiiCautate=1, *args):
        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere) candidate pentru expandare
        lDeschise = [NodParcurgereAStar(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start))]
        lInchise = []  # nodurile expandate

        while len(lDeschise) > 0:
            # print("lDeschise: " + str(lDeschise))
            # print("lInchise = " + str(lInchise))

            nodCurent = lDeschise.pop(0)  # selecteaza nodCurent
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

            for succesor in lSuccesori:
                pozInserare = self.cautareBinaraPozInserare(lDeschise, len(lDeschise), succesor)
                if pozInserare < len(lDeschise):
                    lDeschise.insert(pozInserare, succesor)
                else:
                    lDeschise.append(succesor)

    def idAStar(self, nrSolutiiCautate=1, *args):
        nodStart = NodParcurgereAStar(self.indiceNod(self.start), self.start, None, 0, self.calculeazaH(self.start))
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

    def genereazaSuccesori(self, nodCurent, *args):
        listaSuccesori = []
        for i in range(self.nrNoduri):
            if self.matriceAdiacenta[nodCurent.id][i] == 1 and not nodCurent.contineInDrum(self.noduri[i]):
                nodNou = NodParcurgereAStar(i, self.noduri[i], nodCurent,
                                            nodCurent.g + self.matricePonderi[nodCurent.id][i],
                                            self.calculeazaH(self.noduri[i]))
                listaSuccesori.append(nodNou)
        return listaSuccesori

    def calculeazaH(self, infoNod):
        return self.listaH[self.indiceNod(infoNod)]

    @staticmethod
    def cautareBinaraPozInserare(array, dim: int, nodTinta: NodParcurgereAStar):
        """:parameter: array: array de noduri, ordonat crescator dupa f apoi g, unde cautam un loc.
        :param: dim: dimensiunea array-ului.
        :param: nodTinta: nodul de inserat.
        Obs: compararea pentru NodParcurgere a fost supraincarcata pentru aceasta metoda.
        :returns: pozitia pe care trebuie inserata tinta."""

        if dim == 0 or array[dim - 1] <= nodTinta:
            return dim  # in acest caz, este necesar un append
        if array[0] > nodTinta:
            return 0

        putere = 1
        while putere < dim:  # obtinem cea mai mica putere a lui 2 care este >= dim
            putere <<= 1

        indexInfimum = 0
        while putere:
            if indexInfimum + putere < dim and array[indexInfimum + putere] <= nodTinta:
                indexInfimum += putere
            putere >>= 1

        return 1 + indexInfimum

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
        lSuccesori = self.genereazaSuccesori(nodCurent, )
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

    def gasitScopIdAStar(self, nodCurent, limita, nrSolutiiCautate, *args):
        if args:
            nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
        else:
            nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate)
        print(limita)
        print("\n----------------\n")
        return nrSolutiiCautate
