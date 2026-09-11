import unittest

import GraphBuilder as gBuilder


class TestCasesGraphBuilder(unittest.TestCase):
    listaH = [0, 10, 3, 7, 8, 0, 14, 3, 1, 2]

    nrNoduri = 200
    nrMuchii = 300
    nrScopuri = 1
    nrSolutiiCautate = 1
    type = "GRAPHASTAR"
    nrNoduri1 = 20
    nrMuchii1 = 30
    type2 = "GraphAStar"

    @staticmethod
    def verificaMatrici(matriceAdiacenta, matricePonderi):
        print("matriceAdiacenta:")
        for line in matriceAdiacenta:
            print(line)
        print("matricePonderi:")
        if matricePonderi is not None:
            for line in matricePonderi:
                print(line)
        else:
            print("is None :(")
        print("-----------")

    def testGetNrMuchiiNeorientat(self):
        print("testGetNrMuchiiNeorientat:")

        nrMuchiiNeorientat = gBuilder.MatricesBuilder.getNrMuchii(self.nrNoduri, self.nrMuchii, False)

        self.assertEqual(nrMuchiiNeorientat, 300)
    def testGetNrMuchiiOrientat(self):
        print("testGetNrMuchiiOrientat:")

        nrMuchiiOrientat = gBuilder.MatricesBuilder.getNrMuchii(self.nrNoduri, self.nrMuchii, True)

        self.assertEqual(nrMuchiiOrientat, 300)

    def testGetNrMuchiiNeorientat1(self):
        print("testGetNrMuchiiNeorientat1:")

        nrMuchiiNeorientat = gBuilder.MatricesBuilder.getNrMuchii(self.nrNoduri, 1000000, False)
        print(nrMuchiiNeorientat)

        self.assertEqual(nrMuchiiNeorientat, (self.nrNoduri - 1) // 2 * self.nrNoduri)

    def testGetNrMuchiiOrientat1(self):
        print("testGetNrMuchiiOrientat1:")

        nrMuchiiOrientat = gBuilder.MatricesBuilder.getNrMuchii(self.nrNoduri, 1000000, True)
        print(nrMuchiiOrientat)

        self.assertEqual(nrMuchiiOrientat, (self.nrNoduri - 1) * self.nrNoduri)

    def testInitMatrici(self):
        print("testInitMatrici:")

        aux = gBuilder.MatricesBuilder.initMatrici(self.nrNoduri1, self.type)
        matriceAdiacenta = aux[0]
        matricePonderi = aux[1]
        self.verificaMatrici(matriceAdiacenta, matricePonderi)

        self.assertEqual(True, True)

    def testGetMatrici(self):
        print("testGetMatrici:")

        aux = gBuilder.MatricesBuilder.getMatrici(self.nrNoduri1, self.nrMuchii1, self.type, False, minPondere=0, maxPondere=45)
        matriceAdiacenta = aux[0]
        matricePonderi = aux[1]
        self.verificaMatrici(matriceAdiacenta, matricePonderi)

        self.assertEqual(True, True)

    def testgetIdxNodStartAndNoduri(self):
        print("testgetIdxNodStartAndNoduri:")

        noduri = gBuilder.GraphBuilder.getListaNoduri(self.nrNoduri)
        start = gBuilder.GraphBuilder.getIdxNodStart(self.nrNoduri)
        print("noduri: " + str(noduri))
        print("start: " + str(start))

        verif = start in noduri

        self.assertEqual(verif, True)

    def testgetIdxNodStartAndNoduri1(self):
        print("testgetIdxNodStartAndNoduri1:")

        noduri = gBuilder.GraphBuilder.getListaNoduri(self.nrNoduri1)
        start = gBuilder.GraphBuilder.getIdxNodStart(self.nrNoduri1)
        print("noduri: " + str(noduri))
        print("start: " + str(start))

        verif = start in noduri

        self.assertEqual(verif, True)


if __name__ == '__main__':
    unittest.main()
