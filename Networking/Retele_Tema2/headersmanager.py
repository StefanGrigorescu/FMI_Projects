import zlib
import struct

from dataformats import DataFormat as df


class HeadersManager:
    @classmethod
    def getRudpHeader(cls, packet, sourcePort, destinationPort, seqNumber, ackNumber, flagsPacked):
        """
        Generates the RUDP Header for the given data, source port, destination port and flags.
        :param packet: The packet to be sent.
        :param sourcePort: The source port number.
        :param destinationPort: The destination port number.
        :param seqNumber: The sequence number (2 bytes).
        :param ackNumber: The acknowledge number (2 bytes).
        :param flagsPacked: The flags to be included in the RUDP header (1 byte).
        :return: The RUDP Header.
        """
        dataLen = len(packet)
        checksum = cls.calculateChecksum(packet)

        rudpHeader = struct.pack(df.rudpHeader, seqNumber, ackNumber, flagsPacked, sourcePort, destinationPort, dataLen, checksum)
        return rudpHeader

    @classmethod
    def getUdpHeader(cls, packet, sourcePort, destinationPort):
        """
        Generates the UDP Header for the given data, source port and destination port.
        :param packet: The packet to be sent.
        :param sourcePort: The source port number.
        :param destinationPort: The destination port number.
        :return: The UDP Header.
        """
        dataLen = len(packet)
        checksum = cls.calculateChecksum(packet)

        udpHeader = struct.pack(df.udpHeader, sourcePort, destinationPort, dataLen, checksum)
        return udpHeader

    @staticmethod
    def calculateChecksum(data):
        """
        Calculates the checksum of the packet using the zlib library.
        :param data: The packet for which checksum is calculated.
        :return: The checksum.
        """
        checksum = zlib.crc32(data)
        return checksum
