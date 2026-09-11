import math

from Elevi import GraphElevi
from Elevi import Elev
import BF


class GraphEleviBuilder:
    strSeparators = ": \n"
    euristici = ("ADMSIBILA1", "ADMISIBILA2", "NEADMISIBILA", "BANALA")

    @classmethod
    def getGraphElevi(cls, path, euristica="ADMISIBILA1"):
        """
        Returneaza graful de tip GraphElevi in functie de configurarile precizate in fisierul de intrare de la calea din pathIn.
        :param path: Calea fisierului de intrare.
        :param euristica: Euristica folosita.
        :return: Graful de tip GraphElevi initializat.
        """
        k, matElevi, dElevi, lNumeAscultati, m, lNumeElevi, numeSursa, numeDestinatie = cls.parseInputFile(path)

        nrElevi = len(lNumeElevi)
        matriceAdiacenta = [[0] * nrElevi for i in range(nrElevi)]
        matricePonderi = [[0] * nrElevi for i in range(nrElevi)]

        strLiber = "liber"
        costAceeasiBanca = 0; costBanciConsecutive = 1; costColoaneAlaturateBanci = 2
        # matricile
        for idxLNumeElevi in range(0, nrElevi):
            elev = dElevi[lNumeElevi[idxLNumeElevi]]
            i = elev.idxLinie
            j = elev.idxColoana
            if i > 0:
                vecin = matElevi[i - 1][j]  # vecinul de deasupra
                if vecin.nume != strLiber and vecin.nume not in elev.sNumeSuparati:
                    cls.seteazaVecin(matriceAdiacenta, matricePonderi, dElevi, elev, vecin, costBanciConsecutive)

            if j > 0:
                vecin = matElevi[i][j - 1]  # vecinul de la stanga
                if vecin.nume != strLiber and vecin.nume not in elev.sNumeSuparati:
                    if j % 2 != 0:  # vecin de la stanga (din aceeasi banca)
                        cls.seteazaVecin(matriceAdiacenta, matricePonderi, dElevi, elev, vecin, costAceeasiBanca)
                    elif i >= len(matElevi) - 2:  # vecin de la stanga (din banca vecina, in ultimele 2 banci)
                        cls.seteazaVecin(matriceAdiacenta, matricePonderi, dElevi, elev, vecin,
                                         costColoaneAlaturateBanci)

        # euristica (listaH)
        euristica = euristica.upper()
        nrElevi = len(lNumeElevi)
        listaH = [0]*nrElevi
        idxLinieDestinatie = dElevi[numeDestinatie].idxLinie
        idxColoanaDestinatie = dElevi[numeDestinatie].idxColoana
        idxLinieTrecere = len(matElevi) - 2     # indexul pe linie al penultimei banci

        if euristica == "ADMISIBILA1":
            for i in range(0, nrElevi):
                elev = dElevi[lNumeElevi[i]]
                listaH[i] = abs(idxLinieTrecere - elev.idxLinie) + abs(idxColoanaDestinatie - elev.idxColoana) + abs(idxLinieDestinatie - idxLinieTrecere)
        elif euristica == "ADMISIBILA2":
            for i in range(0, nrElevi):
                elev = dElevi[lNumeElevi[i]]
                listaH[i] = math.sqrt(pow((idxLinieDestinatie - elev.idxLinie), 2) + pow((idxColoanaDestinatie - elev.idxColoana), 2))
        elif euristica == "NEADMISIBILA":
            for i in range(0, nrElevi):
                elev = dElevi[lNumeElevi[i]]
                listaH[i] = abs(idxLinieTrecere - elev.idxLinie) + abs(idxColoanaDestinatie - elev.idxColoana) + abs(idxLinieDestinatie - idxLinieTrecere)
                listaH[i] *= 3
        elif euristica == "BANALA":
            grBf = BF.GraphBF(lNumeElevi, matriceAdiacenta, dElevi[lNumeElevi[0]], [numeDestinatie])
            for i in range(0, nrElevi):
                grBf.start = dElevi[lNumeElevi[i]]
                listaH[i] = 0 if grBf.accesibilBFQ() else 1

        listaH[dElevi[numeSursa].idxLNumeElevi] = 0
        listaH[dElevi[numeDestinatie].idxLNumeElevi] = 0

        gr = GraphElevi(lNumeElevi, matriceAdiacenta, matricePonderi, numeSursa, [numeDestinatie], listaH, k, matElevi, dElevi, lNumeAscultati, m)
        return gr

    @staticmethod
    def seteazaVecin(matriceAdiacenta, matricePonderi, dElevi, elev, vecin, cost):
        """
        Seteaza 1 in matricea de adiacenta si cost in matricea de ponderi pentru elev si vecin.
        :param matriceAdiacenta: Matricea de adiacenta a grafului.
        :param matricePonderi: Matricea de ponderi a grafului.
        :param dElevi: Dictionar de elevi ce are drept cheie numele elevului si valoare elevul respectiv.
        :param elev: Elevul in raport cu care se verifica adiacenta si ponderea.
        :param vecin: Vecinul elevului fata de care se verifica adiacenta si ponderea.
        :param cost: Ponderea drumului dintre acesti elevi.
        :return:
        """
        idxElev = dElevi[elev.nume].idxLNumeElevi
        idxVecin = dElevi[vecin.nume].idxLNumeElevi

        matriceAdiacenta[idxElev][idxVecin] = matriceAdiacenta[idxVecin][idxElev] = 1
        matricePonderi[idxElev][idxVecin] = matricePonderi[idxVecin][idxElev] = cost

    @classmethod
    def parseInputFile(cls, path: str):
        """
        Analizeaza fisierul de input si returneaza datele problemei extrase.
        :param path: Calea fisierului de intrare.
        :return: Numarul k descris de problema, matricea elevilor, dictionar cu toti elevii, lista elevilor asculatti, numarul m descris de problema, lista numeleor elevilor, numele elevului sursa si numele elevului destinatie.
        """
        file = open(path, "r")
        matElevi = []
        dElevi = dict([])
        lNumeElevi = []

        linie = getNextLine(file, cls.strSeparators)
        k = len(linie)//2
        linie = cls.readMatElevi(file, linie, matElevi, dElevi, lNumeElevi)

        linie = cls.readSuparati(file, linie, dElevi)

        m = int(linie[0])
        lNumeAscultati = []
        linie, idxUrmatorulNume = cls.readAscultati(file, linie, lNumeAscultati)

        numeSursa = linie[idxUrmatorulNume]
        numeDestinatie = linie[idxUrmatorulNume + 1]

        file.close()
        return k, matElevi, dElevi, lNumeAscultati, m, lNumeElevi, numeSursa, numeDestinatie

    @classmethod
    def readMatElevi(cls, file, linie, matElevi, dElevi, lNumeElevi):
        """
        Se citeste matricea de elevi linie cu linie, elevii noi sunt retinuti in dElevi, numele lor este introdus in lNumeElevi si se construieste si matElevi.
        :param file: File descriptorul ce indica unde am ramas in fisierul de intrare.
        :param linie: Ultima linie citita inainte de executarea functiei.
        :param matElevi: Matricea elevilor.
        :param dElevi: Dictionarul elevilor.
        :param lNumeElevi: Lista numelor tuturor elevilor.
        :return: Ultima linie citita dupa executarea functiei.
        """
        strSuparati = "suparati"
        strLiber = "liber"
        idxLNumeElevi = 0
        idxLinie = 0
        while linie:
            if len(linie) > 0 and linie[0] == strSuparati:
                break
            idxColoana = 0
            lElevi = []

            for nume in linie:
                elevNou = Elev(nume, idxLinie, idxColoana, idxLNumeElevi)
                lElevi.append(elevNou)
                if nume != strLiber:
                    dElevi[nume] = elevNou
                    lNumeElevi.append(nume)
                    idxLNumeElevi += 1
                idxColoana += 1
            matElevi.append(lElevi)

            idxLinie += 1
            linie = getNextLine(file, cls.strSeparators)

        linie = getNextLine(file, cls.strSeparators)
        return linie

    @classmethod
    def readSuparati(cls, file, linie, dElevi):
        """
        Se construiesc seturile de nume de elevi suparati.
        :param file: File descriptorul ce indica unde am ramas in fisierul de intrare.
        :param linie: Ultima linie citita inainte de executarea functiei.
        :param dElevi: Dictionarul elevilor.
        :return: Ultima linie citita dupa executarea functiei.
        """
        strAscultati = "ascultati"
        while linie:
            if len(linie) > 0 and linie[0] == strAscultati:
                break
            nume1 = linie[0]
            nume2 = linie[1]
            dElevi[nume1].sNumeSuparati.add(nume2)
            dElevi[nume2].sNumeSuparati.add(nume1)

            linie = getNextLine(file, cls.strSeparators)

        linie = getNextLine(file, cls.strSeparators)
        return linie

    @classmethod
    def readAscultati(cls, file, linie, lNumeAscultati):
        """
        Se construieste lNumeAscultati.
        :param file: File descriptorul ce indica unde am ramas in fisierul de intrare.
        :param linie: Ultima linie citita inainte de executarea functiei.
        :param lNumeAscultati: Lista numelor elevilor ordonate in ordinea in care vor fi ascultati.
        :return: Ultima linie citita dupa executarea functiei si indexul pe linie al urmatorului nume ce va fi citit (validarea inputului).
        """
        strMesaj = "mesaj"
        for i in range(1, len(linie)):
            lNumeAscultati.append(linie[i])
        linie = getNextLine(file, cls.strSeparators)
        while linie:
            if len(linie) > 0 and linie[0] == strMesaj:
                break
            for nume in linie:
                lNumeAscultati.append(nume)

            linie = getNextLine(file, cls.strSeparators)

        idxUrmatorulNume = 1
        if len(linie) == 1:     # daca e doar stringul "mesaj:" pe linia curenta
            linie = getNextLine(file, cls.strSeparators)    # citim urmatoarea linie
            idxUrmatorulNume = 0
        return linie, idxUrmatorulNume


def getNextLine(file, strSeparators):
    """
    Citeste urmatoarea linie, o imparte pe cuvinte, elimina caracterele despartitoare din strSeparators si returneaza lista de stringuri obtinuta.
    :param file: File descriptorul ce indica unde am ramas in fisierul de intrare.
    :param strSeparators: Contine caracterele despartitoare ce vor fi eliminate la parsarea liniei.
    :return: Lista de stringuri obtinuta.
    """
    lStr = [file.readline().strip(strSeparators)]
    for separator in strSeparators:
        lStrAux = []
        for elem in lStr:
            newElems = elem.split(sep=separator)
            for newElem in newElems:
                if newElem != '' and newElem != "->":
                    lStrAux.append(newElem)
        lStr = lStrAux
    return lStr