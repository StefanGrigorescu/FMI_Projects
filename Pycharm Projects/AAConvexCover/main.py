import queue
from enum import Enum


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __lt__(self, other):
        return self.x < other.x or (self.x == other.x and self.y < other.y)

    def __repr__(self):
        s = "({}, {})".format(self.x, self.y)
        return s


def comp(fst: Point, snd: Point):
    if fst < snd:
        return 1
    if fst == snd:
        return 0
    return -1


# graham skam algorithm (var. Andrew)
class Cover:
    @staticmethod
    def getDet(p, q, r):
        return 1/2 * ((p.x - r.x)*(q.y - r.y) - (q.x - r.x)*(p.y - r.y))

    @staticmethod
    def getRotation(p, q, r):
        d = Cover.getDet(p, q, r)
        if d < 0:
            return Orientation(-1)
        if d > 0:
            return Orientation(1)
        return Orientation(0)

    @staticmethod
    def sortPoints(points):
        points.sort(key=comp)
        return points

    @classmethod
    def getCover(cls, *args):
        points = [arg for arg in args]
        points = cls.sortPoints(points)

        length = len(points)
        if length < 3:
            return None

        stack = queue.SimpleQueue()
        stack.put(points[0])
        stack.put(points[1])

        for i in range(2, length):
            r = points[i]
            q = stack.get()
            p = stack.get()
            stack.put(p)
            stack.put(q)
            if cls.getRotation(p, q, r) == Orientation.LEFT:
                stack.put(r)

        for i in range(length - 1, -1, -1):
            r = points[i]
            q = stack.get()
            p = stack.get()
            stack.put(p)
            stack.put(q)
            if cls.getRotation(p, q, r) == Orientation.LEFT:
                stack.put(r)

        return stack


class Orientation(Enum):
    RIGHT = -1
    TOUCH = 0
    LEFT = 1


if __name__ == "__main__":
    t = int(input())
    orientations = []
    for i in range(t):
        coords = [int(x) for x in input().split(" ")]
        p1 = Point(coords[0], coords[1])
        p2 = Point(coords[2], coords[3])
        p3 = Point(coords[4], coords[5])
        orientations.append(Cover.getRotation(p1, p2, p3).name)
    for orientation in orientations:
        print(orientation)
