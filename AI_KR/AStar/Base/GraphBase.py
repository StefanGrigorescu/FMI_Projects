from abc import ABC, abstractmethod


class Graph(ABC):  # graful problemei
    def __init__(self, noduri, matriceAdiacenta, start, scopuri):
        self.noduri = noduri
        self.matriceAdiacenta = matriceAdiacenta
        self.nrNoduri = len(matriceAdiacenta)
        self.start = start
        self.scopuri = scopuri

    # va genera succesorii sub forma de noduri in arborele de parcurgere
    @abstractmethod
    def genereazaSuccesori(self, nodCurent, *args):
        pass

    def testeazaScop(self, nodCurent):
        return nodCurent.info in self.scopuri

    @staticmethod
    def gasitScop(nodCurent, nrSolutiiCautate, *args):
        print("Solutie: ", end="")
        drumul, lungimea = nodCurent.afisDrum()
        nrSolutiiCautate -= 1

        if args:
            path = args[0]
            fout = open(path, "a")
            fout.write("Solutie: " + drumul + "\n")
            fout.close()

        return nrSolutiiCautate

    def indiceNod(self, nod):
        return self.noduri.index(nod)

    def __repr__(self):
        sir = ""
        for (k, v) in self.__dict__.items():
            sir += "{} = {}\n".format(k, v)
        return sir
