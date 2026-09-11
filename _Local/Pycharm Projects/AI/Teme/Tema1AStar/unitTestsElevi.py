import unittest
import cProfile
import Elevi
from GraphEleviBuilder import GraphEleviBuilder


class TestCasesElevi(unittest.TestCase):
    nrSolutiiCautate = 1
    pathIn = "IOFilesElevi/Elevi.in"
    pathOut = "IOFilesElevi/Elevi.out"

    pathInFaraSolutie = "IOFilesElevi/EleviFaraSolutie.in"
    pathOutFaraSolutie = "IOFilesElevi/EleviFaraSolutie.out"

    pathInStartEgalFinal = "IOFilesElevi/EleviStartEgalFinal.in"
    pathOutStartEgalFinal = "IOFilesElevi/EleviStartEgalFinal.out"

    pathInNeadmisibila = "IOFilesElevi/EleviNeadmisibila.in"
    pathOutNeadmisibila = "IOFilesElevi/EleviNeadmisibila.out"

    def testParseInputFile(self):
        print("testParseInputFile:")
        k, matElevi, dElevi, lNumeAscultati, m, lNumeElevi, numeSursa, numeDestinatie = GraphEleviBuilder.parseInputFile(self.pathIn)

        primulGresit = 0
        if k != 3:
            primulGresit = 1
        elif len(matElevi) != 7:
            primulGresit = 2
        elif len(matElevi[0]) != 6:
            primulGresit = 3
        elif matElevi[1][3].nume != "liber":
            primulGresit = 4
        elif len(dElevi) != len(lNumeElevi):
            primulGresit = 5
        elif len(lNumeAscultati) != 2:
            primulGresit = 6
        elif lNumeAscultati[0] != "monica":
            primulGresit = 7
        elif m != 4:
            primulGresit = 8
        elif numeSursa != "ionel":
            primulGresit = 9
        elif numeDestinatie != "dragos":
            primulGresit = 10
        elif "ionel" not in dElevi["george"].sNumeSuparati:
            primulGresit = 11
        elif "george" not in dElevi["ionel"].sNumeSuparati:
            primulGresit = 12
        elif len(dElevi["teo"].sNumeSuparati) != 2:
            primulGresit = 13

        self.assertEqual(0, primulGresit)

    def testGetGraphElevi(self):
        print("testGetGraphElevi:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)

        primulGresit = 0
        if gr.noduri[9] != "nadia":
            primulGresit = 1
        elif len(gr.noduri) != len(gr.matriceAdiacenta):
            primulGresit = 2
        elif len(gr.matriceAdiacenta) != len(gr.matricePonderi):
            primulGresit = 3
        elif len(gr.scopuri) != 1:
            primulGresit = 4
        elif gr.scopuri[0] != "dragos":
            primulGresit = 5
        elif len(gr.listaH) != len(gr.noduri):
            primulGresit = 6
        elif gr.listaH[gr.dElevi[gr.scopuri[0]].idxLNumeElevi] != 0:    # daca nodul scop are un h != 0
            primulGresit = 7
        elif len(gr.dElevi["ionel"].sNumeSuparati) != 1:
            primulGresit = 8
        elif "ionel" not in gr.dElevi["george"].sNumeSuparati:
            primulGresit = 9
        elif len(gr.matElevi[0]) != gr.k * 2:
            primulGresit = 10
        elif gr.matElevi[0][0].nume != "ionel":
            primulGresit = 11
        elif gr.matElevi[0][0].sNumeSuparati != gr.dElevi["ionel"].sNumeSuparati:
            primulGresit = 12

        print("noduri:")
        print(gr.noduri)
        print("\nmatriceAdiacenta:")
        for linie in gr.matriceAdiacenta:
            print(linie)
        print("\nmatricePonderi")
        for linie in gr.matricePonderi:
            print(linie)

        self.assertEqual(0, primulGresit)

    def testAStarOpt(self):
        print("testAStarOpt:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOut

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStarOpt(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStar(self):
        print("testAStar:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOut

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testIdAStar(self):
        print("testIdAStar:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOut

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.idAStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testBreadthFirstQ(self):
        print("testBreadthFirstQ:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOut

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.breadthFirstQ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirst(self):
        print("testDepthFirst:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOut

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirst(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstIterativ(self):
        print("testDepthFirstIterativ:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathIn)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOut

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirstIterativ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarOptFaraSolutie(self):
        print("testAStarOptFaraSolutie:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInFaraSolutie)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutFaraSolutie

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStarOpt(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarFaraSolutie(self):
        print("testAStarFaraSolutie:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInFaraSolutie)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutFaraSolutie

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testIdAStarFaraSolutie(self):
        print("testIdAStarFaraSolutie:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInFaraSolutie)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutFaraSolutie

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.idAStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testBreadthFirstQFaraSolutie(self):
        print("testBreadthFirstQFaraSolutie:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInFaraSolutie)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutFaraSolutie

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.breadthFirstQ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstFaraSolutie(self):
        print("testDepthFirstFaraSolutie:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInFaraSolutie)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutFaraSolutie

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirst(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstIterativFaraSolutie(self):
        print("testDepthFirstIterativFaraSolutie:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInFaraSolutie)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutFaraSolutie

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirstIterativ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarOptStartEgalFinal(self):
        print("testAStarOptStartEgalFinal:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInStartEgalFinal)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutStartEgalFinal

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStarOpt(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarStartEgalFinal(self):
        print("testAStarStartEgalFinal:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInStartEgalFinal)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutStartEgalFinal

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testIdAStarStartEgalFinal(self):
        print("testIdAStarStartEgalFinal:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInStartEgalFinal)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutStartEgalFinal

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.idAStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testBreadthFirstQEgalFinal(self):
        print("testBreadthFirstQEgalFinal:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInStartEgalFinal)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutStartEgalFinal

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.breadthFirstQ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstStartEgalFinal(self):
        print("testDepthFirstStartEgalFinal:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInStartEgalFinal)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutStartEgalFinal

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirst(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstIterativStartEgalFinal(self):
        print("testDepthFirstIterativStartEgalFinal:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInStartEgalFinal)
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutStartEgalFinal

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirstIterativ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarOptNeadmisibila(self):
        print("testAStarOptNeadmisibila:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInNeadmisibila, "NEADMISIBILA")
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutNeadmisibila

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStarOpt(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testAStarNeadmisibila(self):
        print("testAStarNeadmisibila:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInNeadmisibila, "NEADMISIBILA")
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutNeadmisibila

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.aStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testIdAStarNeadmisibila(self):
        print("testIdAStarNeadmisibila:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInNeadmisibila, "NEADMISIBILA")
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutNeadmisibila

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.idAStar(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testBreadthFirstQNeadmisibila(self):
        print("testBreadthFirstQNeadmisibila:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInNeadmisibila, "NEADMISIBILA")
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutNeadmisibila

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.breadthFirstQ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstNeadmisibila(self):
        print("testDepthFirstNeadmisibila:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInNeadmisibila, "NEADMISIBILA")
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutNeadmisibila

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirst(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)

    def testDepthFirstIterativNeadmisibila(self):
        print("testDepthFirstIterativNeadmisibila:")
        gr = GraphEleviBuilder.getGraphElevi(self.pathInNeadmisibila, "NEADMISIBILA")
        nrSolutiiCautate = self.nrSolutiiCautate
        pathOut = self.pathOutNeadmisibila

        Elevi.NodParcurgereElevi.graf = gr

        cProfile.runctx('Elevi.GraphElevi.depthFirstIterativ(gr, nrSolutiiCautate, pathOut)', globals(), locals())

        self.assertEqual(True, True)


if __name__ == '__main__':
    unittest.main()
