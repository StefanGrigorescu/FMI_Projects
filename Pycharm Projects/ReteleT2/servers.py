import networknodes as nn
from headersmanager import HeadersManager as hm
from headerflags import HeaderFlags as hf


class Server(nn.NetworkNode):
    sourcePort1 = 1112

    def __init__(self, sourceHost, sourcePort, seqNumber=5, pkgLossChance=4, waitingTime=5.0):
        """
        Initializes the socket object used by the server to send and receive packet.
        :param sourceHost: The host of the server.
        :param sourcePort: The port number used by the server.
        :param seqNumber: The sequence number to start indexing the packets with.
        :param pkgLossChance: Integer between 1 and 10 inclusive, representing the chance that a sent packet will be lost.
        :param waitingTime: The maximum waiting time before resending a packet.
        """
        super(Server, self).__init__(sourceHost, sourcePort, seqNumber, "Server", pkgLossChance, waitingTime)
        self.awaitSyn()

    def awaitSyn(self):
        while True:
            print("Server: Awaiting syn from client...")
            rudpHeader, data, senderAddress = self.awaitPacket()

            if rudpHeader is not None:
                synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
                if synFlag:
                    seqNumber = rudpHeader[0]
                    self.ackNumber = seqNumber + 1
                    print("Server: Received syn from ({}, {}) with seq number = {}.".format(senderAddress[0],
                                                                                            senderAddress[1],
                                                                                            seqNumber))
                    self.sendSynAck(senderAddress)
                    break

    def sendSynAck(self, receiverAddress):
        while True:
            print("Server: Sending syn ack to ({}, {}) with seq number = {}, ack number = {}.".format(receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber))
            flagsPacked = hf.getFlagsPacked(hf.SYN, hf.ACK)
            self.sendDataViaRudp("", receiverAddress[0], receiverAddress[1], self.seqNumber, self.ackNumber, flagsPacked)

            ackNumber = self.awaitAck()
            if ackNumber == self.seqNumber + 1:
                self.seqNumber += 1
                break

        print("Server: Connection established with ({}, {}) with seq number = {}.".format(receiverAddress[0], receiverAddress[1], ackNumber))

        finished = False
        while not finished:
            finished = self.awaitPshFin()

    def awaitPshFin(self):
        print("Server: Awaiting psh or fin from client...")

        ackNumber = -1
        rudpHeader, data, senderAddress = None, None, None

        while True:     # keep sending acks until receiving a response
            rudpHeader, data, senderAddress = self.awaitPacket(20.0)
            if rudpHeader is not None:
                ackNumber = rudpHeader[1]
                if ackNumber == self.seqNumber:
                    break
                else:
                    print(self.nodeRole + ": (Retrieved ackNumber is = {}, while my seqNumber is now = {}).".format(ackNumber, self.seqNumber))

        synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
        if pshFlag:
            receivedChecksum = rudpHeader[6]
            calculatedChecksum = hm.calculateChecksum(data)
            isCorrupted = receivedChecksum != calculatedChecksum
            if isCorrupted:
                print("Server: Received corrupted packet from ({}, {}).".format(senderAddress[0], senderAddress[1]))
                self.sendAck(senderAddress)
            else:
                seqNumber = rudpHeader[0]
                dataLen = rudpHeader[5]
                print(
                    self.nodeRole + ": Received packet from ({}, {}) with seq number = {}, ack number = {}, len(packet) = {}:".format(
                        senderAddress[0], senderAddress[1], seqNumber, ackNumber, dataLen))
                if seqNumber == self.ackNumber:
                    self.ackNumber += dataLen
                    print(data.decode())
                    self.sendPshAck(senderAddress)
                else:
                    self.sendAck(senderAddress)

                return False

        elif finFlag:
            print("Server: Received fin from ({}, {}):".format(senderAddress[0], senderAddress[1]))
            self.ackNumber += 1

            ackNumber = -1
            while True:
                self.sendAck(senderAddress)
                self.sendFin(senderAddress)

                ackNumber = self.awaitAck()
                if ackNumber == self.seqNumber + 1:
                    self.seqNumber += 1
                    break

            print("Server: Connection closed with ({}, {}) with seq number = {}.".format(senderAddress[0], senderAddress[1], ackNumber))
            return True
