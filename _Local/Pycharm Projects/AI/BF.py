from queue import SimpleQueue
from Base import GraphBase as gBase, NodParcurgereBase as npBase


class GraphBF(gBase.Graph):
    def __init__(self, noduri, matriceAdiacenta, start, scopuri):
        super(GraphBF, self).__init__(noduri, matriceAdiacenta, start, scopuri)

    def breadthFirst(self, nrSolutiiCautate=1, *args):
        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere)
        c = [npBase.NodParcurgere(self.indiceNod(self.start), self.start, None)]

        nodCurent = c[0]
        if self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])

        if nrSolutiiCautate > 0:
            while len(c) > 0:
                nodCurent = c.pop(0)

                lSuccesori, nrSolutiiCautate = self.genereazaSuccesori(nodCurent, nrSolutiiCautate)

                if nrSolutiiCautate <= 0:
                    break
                c.extend(lSuccesori)

    def breadthFirstQ(self, nrSolutiiCautate=1, *args):
        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere)
        c = SimpleQueue()
        nodCurent = npBase.NodParcurgere(self.indiceNod(self.start), self.start, None)
        c.put(nodCurent)
        if self.testeazaScop(nodCurent):
            if args:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])
            else:
                nrSolutiiCautate = self.gasitScop(nodCurent, nrSolutiiCautate, args[0])

        if nrSolutiiCautate > 0:
            while not c.empty():
                nodCurent = c.get()

                lSuccesori, nrSolutiiCautate = self.genereazaSuccesori(nodCurent, nrSolutiiCautate)

                if nrSolutiiCautate <= 0:
                    break
                for succesor in lSuccesori:
                    c.put(succesor)

    def accesibilBFQ(self):
        nrSolutiiCautate = 1
        # in coada vom avea doar noduri de tip NodParcurgere (nodurile din arborele de parcurgere)
        c = SimpleQueue()
        nodCurent = npBase.NodParcurgere(self.indiceNod(self.start), self.start, None)
        c.put(nodCurent)
        if self.testeazaScop(nodCurent):
            return True

        while not c.empty():
            nodCurent = c.get()

            lSuccesori, nrSolutiiCautate = self.genereazaSuccesori(nodCurent, nrSolutiiCautate)

            if nrSolutiiCautate <= 0:
                return True
            for succesor in lSuccesori:
                c.put(succesor)

        return False

    def genereazaSuccesori(self, nodCurent, *args):
        nrSolutiiCautate = args[0]
        lSuccesori = []
        for i in range(self.nrNoduri):
            if self.matriceAdiacenta[nodCurent.id][i] == 1 and not nodCurent.contineInDrum(self.noduri[i]):
                nodNou = npBase.NodParcurgere(i, self.noduri[i], nodCurent)
                lSuccesori.append(nodNou)
                if self.testeazaScop(nodNou):
                    nrSolutiiCautate = self.gasitScop(nodNou, nrSolutiiCautate)
                    if nrSolutiiCautate <= 0:
                        break
        return lSuccesori, nrSolutiiCautate

