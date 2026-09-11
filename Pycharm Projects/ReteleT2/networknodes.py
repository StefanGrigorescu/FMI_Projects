import socket
import struct
import time
import random

from headersmanager import HeadersManager as hm
from headerflags import HeaderFlags as hf
from dataformats import DataFormat as df
from dataformats import DataFormatSize as dfs


class NetworkNode:
    localHost: str = "127.0.0.1"

    def __init__(self, sourceHost, sourcePort, seqNumber, nodeRole, pkgLossChance, waitingTime):
        """
        Initializes the socket object used by the network node to send and receive packets.
        :param sourceHost: The host of the network node.
        :param sourcePort: The port number used by the network node.
        :param seqNumber: The sequence number to start indexing the packets with.
        :param nodeRole: What to be displayed in console (either Server or Client).
        :param pkgLossChance: Integer between 1 and 10 inclusive, representing the chance that a sent packet will be lost.
        :param waitingTime: The maximum waiting time before resending a packet.
        """
        self.sourceHost = sourceHost
        self.sourcePort = sourcePort
        self.socketObj = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socketObj.bind((sourceHost, sourcePort))
        self.seqNumber = abs(seqNumber)     # only positive values allowed as seq numbers
        self.ackNumber = 0
        self.nodeRole = nodeRole
        self.pkgLossChance = pkgLossChance
        self.waitingTime = waitingTime

    def sendAck(self, receiverAddress):
        print(self.nodeRole + ": Sending ack to ({}, {}) with ack number = {}.".format(receiverAddress[0], receiverAddress[1], self.ackNumber))
        flagsPacked = hf.getFlagsPacked(hf.ACK)
        self.sendDataViaRudp("", receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber, flagsPacked)

    def awaitAck(self):
        print(self.nodeRole + ": Awaiting ack...")
        rudpHeader, data, senderAddress = self.awaitPacket()

        if rudpHeader is None:
            return -1

        synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
        if not ackFlag:
            return -2

        ackNumber = rudpHeader[1]  # should be equal to self.seqNumber + 1
        print(self.nodeRole + ": Received ack from ({}, {}) with ack number = {}.".format(senderAddress[0], senderAddress[1], ackNumber))
        return ackNumber

    def sendPshAck(self, receiverAddress):
        data = str(input(self.nodeRole + ": Preparing data to be sent... Read input: "))
        packet = data.encode()
        print(self.nodeRole + ": Sending ack and psh to ({}, {}) with seq number = {}, ack number = {}, len(packet) = {}.".format(receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber, len(packet)))

        flagsPacked = hf.getFlagsPacked(hf.PSH, hf.ACK)
        if data == "":     # just ack
            flagsPacked = hf.getFlagsPacked(hf.ACK)
        self.sendDataViaRudp(data, receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber, flagsPacked)

        send = False
        if data != "":
            while True:
                if send:
                    send = False
                    self.sendDataViaRudp(data, receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber, flagsPacked)
                ackNumber = self.awaitPshAck(len(packet))
                if ackNumber == self.seqNumber + len(packet):
                    self.seqNumber += len(packet)
                    break
                else:
                    print(self.nodeRole + ": (Retrieved ackNumber is = {}, while my seqNumber is now = {}).".format(ackNumber, self.seqNumber + len(packet)))
                    send = True # int(input("Press 1 to resend the message or 0 to await. ")) == 1

    def awaitPshAck(self, dataLen):
        print(self.nodeRole + ": Awaiting ack, psh... with ack number = {}.".format(self.ackNumber))
        rudpHeader, data, senderAddress = self.awaitPacket()

        if rudpHeader is None:
            print(self.nodeRole + ": No packet received or invalid header.")
            return -1

        synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
        if not ackFlag:
            print(self.nodeRole + ": Missing ackFlag from header while expected.")
            return -2

        ackNumber = rudpHeader[1]
        if ackNumber != self.seqNumber + dataLen:
            print(self.nodeRole + ": (ackNumber = {} != self.seqNumber + dataLen = {} + {}).".format(ackNumber, self.seqNumber, dataLen))
            return ackNumber

        print(self.nodeRole + ": Received ack from ({}, {}) with ack number = {}.".format(senderAddress[0],
                                                                                          senderAddress[1], ackNumber))
        if pshFlag:
            seqNumber = rudpHeader[0]
            receivedChecksum = rudpHeader[6]
            calculatedChecksum = hm.calculateChecksum(data)
            isCorrupted = receivedChecksum != calculatedChecksum
            if isCorrupted:
                print(
                    self.nodeRole + ": Received corrupted packet from ({}, {}) with seq number = {}, ack number = {}, len(packet) = {}:".format(
                        senderAddress[0], senderAddress[1], seqNumber, ackNumber, len(data)))
            else:
                dataLen = rudpHeader[5]
                print(
                    self.nodeRole + ": Received packet from ({}, {}) with seq number = {}, ack number = {}, len(packet) = {}:".format(
                        senderAddress[0], senderAddress[1], seqNumber, ackNumber, dataLen))
                if seqNumber == self.ackNumber:
                    self.ackNumber += dataLen
                    print(data.decode())
            self.sendAck(senderAddress)

        return ackNumber

    def sendFin(self, receiverAddress):
        print(self.nodeRole + ": Sending fin to ({}, {}).".format(receiverAddress[0], receiverAddress[1]))
        flagsPacked = hf.getFlagsPacked(hf.FIN)
        self.sendDataViaRudp("", receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber, flagsPacked)

    def sendDataViaRudp(self, data, destinationHost, destinationPort, seqNumber, ackNumber, flagsPacked):
        """
        Sends packet to a server using the UDP (User Datagram Protocol).
        :param data: The packet to be sent.
        :param destinationHost: The host of the destination (server).
        :param destinationPort: The port number of the destination (server).
        :param seqNumber: The sequence number (2 bytes).
        :param ackNumber: The acknowledge number (2 bytes).
        :param flagsPacked: The flags to be included in the RUDP header (1 byte).
        :return:
        """
        pkgLoss = random.randint(1, 10) <= self.pkgLossChance
        if pkgLoss:
            print(self.nodeRole + ": * It seems the packet was lost... *")
            return

        packet = data.encode()      # encoding the packet (converting it to bytes)
        rudpHeader = hm.getRudpHeader(packet, self.sourcePort, destinationPort, seqNumber, ackNumber, flagsPacked)
        packetWithHeader = rudpHeader + packet

        receiverAddress = (destinationHost, destinationPort)
        self.socketObj.sendto(packetWithHeader, receiverAddress)

    def sendDataViaUdp(self, data, destinationHost, destinationPort):
        """
        Sends packet to a server using the UDP (User Datagram Protocol).
        :param data: The packet to be sent.
        :param destinationHost: The host of the destination (server).
        :param destinationPort: The port number of the destination (server).
        :return:
        """
        pkgLoss = random.randint(1, 10) <= self.pkgLossChance
        if pkgLoss:
            print(self.nodeRole + ": * It seems the packet was lost... *")
            return

        packet = data.encode()      # encoding the packet (converting it to bytes)
        udpHeader = hm.getUdpHeader(packet, self.sourcePort, destinationPort)
        packetWithHeader = udpHeader + packet

        receiverAddress = (destinationHost, destinationPort)
        self.socketObj.sendto(packetWithHeader, receiverAddress)

    def awaitPacket(self, waitingTime=-1):
        """
        Awaits a packet and returns the result.
        :param waitingTime: The maximum waiting time before resending a packet. If default or less than zero, self.waitingTime's value will be used instead.
        :return: The header, data and address of the sender if a packet was received, or o tuple of None, None, None.
        """

        if waitingTime < 0:
            waitingTime = self.waitingTime

        deadline = time.time() + waitingTime
        oldTimeout = self.socketObj.gettimeout()
        try:
            self.socketObj.settimeout(deadline - time.time())
            packetWithHeader = None
            packetWithHeader, senderAddress = self.socketObj.recvfrom(100)
            self.socketObj.settimeout(oldTimeout)

            if packetWithHeader is not None:
                rudpHeader = packetWithHeader[:dfs.rudpHeaderSize.value]
                rudpHeader = struct.unpack(df.rudpHeader, rudpHeader)
                data = packetWithHeader[dfs.rudpHeaderSize.value:]

                return rudpHeader, data, senderAddress

            return None, None, None
        except socket.timeout:
            self.socketObj.settimeout(oldTimeout)
            return None, None, None
