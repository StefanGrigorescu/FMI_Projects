import unittest
import cProfile

import BF
import DF
import GraphBuilder as gBuilder


class TestCasesBfDf(unittest.TestCase):
    #      # Date default pt testare:
    # pozitia i din vectorul de noduri da si numarul liniei/coloanei corespunzatoare din matricea de adiacenta
    noduri = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j"]

    m = [
        [0, 1, 0, 1, 1, 0, 0, 0, 0, 0],
        [1, 0, 1, 0, 0, 1, 0, 0, 0, 0],
        [0, 1, 0, 0, 0, 1, 0, 1, 0, 0],
        [1, 0, 0, 0, 0, 0, 1, 0, 0, 0],
        [1, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [0, 1, 1, 0, 0, 0, 0, 0, 0, 0],
        [0, 0, 0, 1, 0, 0, 0, 0, 0, 0],
        [0, 0, 1, 0, 1, 0, 0, 0, 1, 1],
        [0, 0, 0, 0, 0, 0, 0, 1, 0, 0],
        [0, 0, 0, 0, 0, 0, 0, 1, 0, 0]
    ]

    start = "a"
    scopuri = ["f", "j"]

    nrNoduri = 200
    nrMuchii = 300
    nrScopuri = 4
    nrSolutiiCautate = 4

    def testBf(self):
        print("testBf:")
        gr = BF.GraphBF(self.noduri, self.m, self.start, self.scopuri)
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('BF.GraphBF.breadthFirst(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testBfQ(self):
        print("testBfQ:")
        gr = BF.GraphBF(self.noduri, self.m, self.start, self.scopuri)
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('BF.GraphBF.breadthFirstQ(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testDf(self):
        print("testDf:")
        gr = DF.GraphDF(self.noduri, self.m, self.start, self.scopuri)
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('DF.GraphDF.depthFirst(gr, nrSolutiiCautate)', globals(), locals())
        # df(a)->df(b)->df(c)
        #############################################
        self.assertEqual(True, True)

    def testDfI(self):
        print("testDfI:")
        gr = DF.GraphDF(self.noduri, self.m, self.start, self.scopuri)
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('DF.GraphDF.depthFirstIterativ(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testDfLifo(self):
        print("testDfLifo: ")
        gr = DF.GraphDF(self.noduri, self.m, self.start, self.scopuri)
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('DF.GraphDF.depthFirstLIFO(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testBf1(self):
        print("testBf1:")
        gr = gBuilder.GraphBuilder.getGraph(self.nrNoduri, self.nrMuchii, self.nrScopuri, "GraphBF")
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('BF.GraphBF.breadthFirst(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testBfQ1(self):
        print("testBfQ1:")
        gr = gBuilder.GraphBuilder.getGraph(self.nrNoduri, self.nrMuchii, self.nrScopuri, "GraphBF")
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('BF.GraphBF.breadthFirstQ(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testDf1(self):
        print("testDf1:")
        gr = gBuilder.GraphBuilder.getGraph(self.nrNoduri, self.nrMuchii, self.nrScopuri, "GraphDF")
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('DF.GraphDF.depthFirst(gr, nrSolutiiCautate)', globals(), locals())

        self.assertEqual(True, True)

    def testDfI1(self):
        print("testDfI1:")
        gr = gBuilder.GraphBuilder.getGraph(self.nrNoduri, self.nrMuchii, self.nrScopuri, "GraphDF")
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('DF.GraphDF.depthFirstIterativ(gr, nrSolutiiCautate)', globals(), locals())

    def testDfLifo1(self):
        print("testDfLifo1:")
        gr = gBuilder.GraphBuilder.getGraph(self.nrNoduri, self.nrMuchii, self.nrScopuri, "GraphDF")
        nrSolutiiCautate = self.nrSolutiiCautate

        cProfile.runctx('DF.GraphDF.depthFirstLIFO(gr, nrSolutiiCautate)', globals(), locals())


if __name__ == '__main__':
    unittest.main()
