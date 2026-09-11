import unittest
import cProfile

import AStar


class TestCasesAStar(unittest.TestCase):
    #      # Date default pt testare:
    # pozitia i din vectorul de noduri da si numarul liniei/coloanei corespunzatoare din matricea de adiacenta
    noduri = ["a", "b", "c", "d", "e", "f", "g", "i", "j", "k"]

    matriceAdiacenta = [
        [0, 1, 1, 1, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 1, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 1, 1, 0, 0, 1, 0, 0],
        [0, 0, 1, 0, 1, 0, 0, 0, 2, 1],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ]
    matricePonderi = [
        [0, 3, 9, 7, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 4, 100, 0, 0, 0, 0],
        [0, 0, 0, 0, 10, 0, 5, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 4, 0, 0],
        [0, 0, 1, 0, 0, 10, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 1, 7, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 1, 0, 0, 0, 1, 1],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    ]
    start = "a"
    scopuri = ["f"]
    # exemplu de euristica banala (1 daca nu e nod scop si 0 daca este)
    listaH = [0, 10, 3, 7, 8, 0, 14, 3, 1, 2]

    nrNoduri = 200
    nrMuchii = 300
    nrScopuri = 1
    nrSolutiiCautate = 1

    def testAStar(self):
        print("testAStar:")
        gr = AStar.GraphAStar(self.noduri, self.matriceAdiacenta, self.matricePonderi, self.start, self.scopuri, self.listaH)
        nrSolutiiCautate = self.nrSolutiiCautate

        AStar.NodParcurgereAStar.graf = gr

        cProfile.runctx('AStar.GraphAStar.aStar(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarDict(self):
        print("testAStarDict:")
        gr = AStar.GraphAStar(self.noduri, self.matriceAdiacenta, self.matricePonderi, self.start, self.scopuri, self.listaH)
        nrSolutiiCautate = self.nrSolutiiCautate

        AStar.NodParcurgereAStar.graf = gr

        cProfile.runctx('AStar.GraphAStar.aStarDict(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testIdAStar(self):
        print("testIdAStar:")
        gr = AStar.GraphAStar(self.noduri, self.matriceAdiacenta, self.matricePonderi, self.start, self.scopuri,
                              self.listaH)
        nrSolutiiCautate = self.nrSolutiiCautate

        AStar.NodParcurgereAStar.graf = gr

        cProfile.runctx('AStar.GraphAStar.idAStar(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarOpt(self):
        print("testAStarOpt:")
        gr = AStar.GraphAStar(self.noduri, self.matriceAdiacenta, self.matricePonderi, self.start, self.scopuri,
                              self.listaH)
        nrSolutiiCautate = self.nrSolutiiCautate

        AStar.NodParcurgereAStar.graf = gr

        cProfile.runctx('AStar.GraphAStar.aStarOpt(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

if __name__ == '__main__':
    unittest.main()
