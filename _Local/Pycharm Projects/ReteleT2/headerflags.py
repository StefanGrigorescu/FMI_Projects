from enum import IntEnum


class HeaderFlags(IntEnum):
    SYN: bool = 1 << 4
    ACK: bool = 1 << 5
    PSH: bool = 1 << 6
    FIN: bool = 1 << 7

    @staticmethod
    def getFlagsPacked(*flags):
        flagsPacked = 0
        for flag in flags:
            flagsPacked |= flag
        return flagsPacked

    @classmethod
    def getFlagsUnpacked(cls, flagsPacked):
        synFlag = cls.SYN & flagsPacked
        ackFlag = cls.ACK & flagsPacked
        pshFlag = cls.PSH & flagsPacked
        finFlag = cls.FIN & flagsPacked

        return synFlag, ackFlag, pshFlag, finFlag
