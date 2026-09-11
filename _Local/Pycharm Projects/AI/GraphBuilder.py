import random
import BF
import DF
import AStar


class GraphBuilder:
    graphTypes = ["GRAPHBF", "GRAPHDF", "GRAPHASTAR"]

    @classmethod
    def getGraph(cls, nrNoduri: int, nrMuchii: int, nrScopuri: int = 1, type: str = "GraphBF", orientat: bool = False, listaH=None, minPondere: int = 0, maxPondere: int = 100000):
        type = type.upper()
        if type not in cls.graphTypes:
            raise TypeError("Specified Graph Type not allowed. Available types are " + ", ".join(cls.graphTypes))

        noduri = cls.getListaNoduri(nrNoduri)
        start = cls.getIdxNodStart(nrNoduri)
        scopuri = cls.getListaScopuri(nrNoduri, nrScopuri)

        matriceAdiacenta, matricePonderi = MatricesBuilder.getMatrici(nrNoduri, nrMuchii, type, orientat, minPondere, maxPondere)

        if listaH is None:
            listaH = [0]*nrNoduri

        if type == "GRAPHBF":
            return BF.GraphBF(noduri, matriceAdiacenta, start, scopuri)
        elif type == "GRAPHDF":
            return DF.GraphDF(noduri, matriceAdiacenta, start, scopuri)
        elif type == "GRAPHASTAR":
            return AStar.GraphAStar(noduri, matriceAdiacenta, matricePonderi, start, scopuri, listaH)
        else:
            return None

    @staticmethod
    def getListaNoduri(nrNoduri: int):
        """
        Creeaza o lista de noduri indexate de la 1 la nrNoduri-1 si returneaza o referinta la aceasta.
        :param nrNoduri: numarul de noduri al grafului.
        :return: o referinta la lista de noduri.
        """
        noduri = []
        for i in range(0, nrNoduri):
            noduri.append(str(i))
        return noduri

    @staticmethod
    def getIdxNodStart(nrNoduri: int):
        """
        Returneaza o valoare random in intervalul [0, nrNoduri) pe post de index al nodului de start.
        :param nrNoduri: numarul de noduri.
        :return: indexul nodului de start.
        """
        return str(random.randint(0, nrNoduri))

    @staticmethod
    def getListaScopuri(nrNoduri: int, nrScopuri: int = 1):
        """
        Creeaza o lista de scopuri, o poplueaza cu valori random si returneaza o referinta la aceasta.
        :param nrNoduri: numarul de noduri al grafului.
        :param nrScopuri: numarul de scopuri solicitate.
        :return: o referinta la lista de scopuri.
        """

        scopuri = []
        while nrScopuri:
            scop = str(random.randint(0, nrNoduri))
            if scop not in scopuri:
                scopuri.append(scop)
                nrScopuri -= 1
        return scopuri


class MatricesBuilder:
    @classmethod
    def getMatrici(cls, nrNoduri: int, nrMuchii: int, type: str = "GraphBF", orientat: bool = False, minPondere: int = 0, maxPondere: int = 100000):
        """
        In euristica aceasta, getMatrice genereaza matricea de adiacenta si cea de ponderi cu un numar de muchii <= nrMuchii,
        in vederea conservarii eficientei de timp.
        :param nrNoduri: numarul de noduri din graf.
        :param nrMuchii: numarul de muchii solicitate pentru adaugare in graf.
        :param type: tipul grafului.
        :param orientat: (bool) este adevarat daca graful este orientat sau nu.
        :param minPondere: ponderea minima acceptata in matricea de ponderi.
        :param maxPondere: ponderea maxima acceptata in matricea de ponderi.
        :return: un tuplu ce contine pe pozitia 0 referinta catre matricea de adiacenta si pe pozitia 1 referinta catre matricea de ponderi.
        """

        nrMuchii = cls.getNrMuchii(nrNoduri, nrMuchii, orientat)

        matriceAdiacenta, matricePonderi = cls.initMatrici(nrNoduri, type)

        if not orientat:
            for i in range(1, nrNoduri):
                for j in range(0, i):
                    if matriceAdiacenta[i][j] == 0 and random.randint(0, 10) > 5:
                        matriceAdiacenta[i][j] = matriceAdiacenta[j][i] = 1
                        if type == "GRAPHASTAR":
                            matricePonderi[i][j] = matricePonderi[j][i] = random.randint(minPondere, maxPondere)
                        nrMuchii -= 1
                        if nrMuchii <= 0:
                            break
        else:
            for i in range(1, nrNoduri):
                for j in range(0, nrNoduri):
                    if matriceAdiacenta[i][j] == 0 and random.randint(0, 10) > 5:
                        matriceAdiacenta[i][j] = 1
                        matricePonderi[i][j] = random.randint(minPondere, maxPondere)
                        nrMuchii -= 1
                        if nrMuchii <= 0:
                            break
        # este probabil ca nrMuchii sa nu ajunga <= 0 pana sa se termine oricare dinre forurile de deasupra, de aceea dam return aici, la final:
        return matriceAdiacenta, matricePonderi

    @staticmethod
    def getNrMuchii(nrNoduri: int, nrMuchii: int, orientat: bool):
        """
        Returneaza numarul de muchii in functie de valoarea data apelant si numarul maxim de muchii posibil pentru graful respectiv.
        :param nrNoduri: numarul de noduri al grafului.
        :param nrMuchii: numarul de muchii dat de apelant.
        :param orientat: (bool) este adevarat daca graful este orientat.
        :return: minimul dintre nrMuchii si numarul maxim de muchii posibil pentru graful respectiv.
        """
        if not orientat:
            return min(nrMuchii, (nrNoduri - 1) // 2 * nrNoduri)
        else:
            return min(nrMuchii, (nrNoduri - 1) * nrNoduri)

    @staticmethod
    def initMatrici(nrNoduri: int, type: str):
        """
        Returneaza valori initiale pentru matricea de adiacenta si cea de ponderi, in functie de numarul de noduri si de tipul grafului.
        :param nrNoduri: numarul de noduri.
        :param type: tipul grafului.
        :return: un tuplu ce contine pe pozitia 0 referinta catre matricea de adiacenta si pe pozitia 1 referinta catre matricea de ponderi.
        """
        matriceAdiacenta = [[0]*nrNoduri]*nrNoduri
        matricePonderi = None
        if type == "GRAPHASTAR":
            matricePonderi = [[0]*nrNoduri]*nrNoduri
        return matriceAdiacenta, matricePonderi
