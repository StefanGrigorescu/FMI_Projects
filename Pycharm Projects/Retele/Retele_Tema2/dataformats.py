from enum import Enum


class DataFormat(str, Enum):
    # see official docs for Format Characters here:  https://docs.python.org/3/library/struct.html#format-characters
    udpHeader = '!IIII'     # 4 unsigned integers (x4 bytes one)

    rudpHeader = '!HHBIIII'  # 2 unsigned short (x2 bytes one), 5 unsigned integers (x4 bytes one)


class DataFormatSize(int, Enum):
    udpHeaderSize = 16
    rudpHeaderSize = 21
