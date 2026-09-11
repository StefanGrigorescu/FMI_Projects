# informatii despre un nod din arborele de parcurgere (nu din graful initial)
class NodParcurgere:
    def __init__(self, id, info, parinte):
        self.id = id  # este indicele din vectorul de noduri
        self.info = info
        self.parinte = parinte  # parintele din arborele de parcurgere

    def contineInDrum(self, infoNodNou):
        nodDrum = self
        while nodDrum is not None:
            if infoNodNou == nodDrum.info:
                return True
            nodDrum = nodDrum.parinte

        return False

    def afisDrum(self):  # returneaza si lungimea drumului
        drumul = self.obtineDrum()
        print("->".join(drumul))
        return drumul, len(drumul)

    def obtineDrum(self):
        drumul = [self.info]
        nod = self
        while nod.parinte is not None:
            drumul.insert(0, nod.parinte.info)   # insereaza la index 0
            nod = nod.parinte
        return drumul

    def __repr__(self):
        sir = ""
        sir += self.info + "("
        sir += "id = {}, ".format(self.id)
        sir += "drum ="
        drum = self.obtineDrum()
        sir += "->".join(drum)
        sir += ")"
        return sir

