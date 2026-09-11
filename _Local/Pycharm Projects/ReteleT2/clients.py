import networknodes as nn
from headerflags import HeaderFlags as hf


class Client(nn.NetworkNode):
    sourcePort1 = 1111

    def __init__(self, sourceHost, sourcePort, seqNumber=0, pkgLossChance=4, waitingTime=5.0):
        """
        Initializes the socket object used by the client to send and receive packet.
        :param sourceHost: The host of the client.
        :param sourcePort: The port number used by the client.
        :param seqNumber: The sequence number to start indexing the packets with.
        :param pkgLossChance: Integer between 1 and 10 inclusive, representing the chance that a sent packet will be lost.
        :param waitingTime: The maximum waiting time before resending a packet.
        """
        super(Client, self).__init__(sourceHost, sourcePort, seqNumber, "Client", pkgLossChance, waitingTime)

    def sendSyn(self, destinationHost, destinationPort):
        receiverAddress = None
        while True:
            print("Client: Sending syn to ({}, {}) with seq number = {}.".format(destinationHost, destinationPort, self.seqNumber))
            flagsPacked = hf.getFlagsPacked(hf.SYN)
            self.sendDataViaRudp("", destinationHost, destinationPort, self.seqNumber, self.ackNumber, flagsPacked)

            ackNumber, receiverAddress = self.awaitSynAck()
            if ackNumber >= 0:
                self.ackNumber = ackNumber
                break

        self.seqNumber += 1
        self.sendAck(receiverAddress)

        print("Server: Connection established with ({}, {}) with seq number = {}.".format(receiverAddress[0], receiverAddress[1], self.ackNumber))
        self.selectOption(receiverAddress)

    def awaitSynAck(self):
        """
        Awaits a syn ack and returns the ack number and the senderAddress.
        :return: A tuple with the ack number and the senderAddress.
        """
        print("Client: Awaiting syn ack from server...")
        rudpHeader, data, senderAddress = self.awaitPacket(5.0)

        if rudpHeader is None:
            print(self.nodeRole + ": No packet received or invalid header.")
            return -1, None

        synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
        if not (synFlag and ackFlag):
            print(self.nodeRole + ": Missing ackFlag or synFlag from header while expected.")
            return -2, None

        seqNumber = rudpHeader[0]
        ackNumber = rudpHeader[1]  # should be equal to self.seqNumber + 1
        print("Client: Received syn ack from ({}, {}) with seq number = {}, ack number = {}.".format(senderAddress[0], senderAddress[1], seqNumber, ackNumber))

        if ackNumber != self.seqNumber + 1:
            return -3, None

        return seqNumber + 1, senderAddress

    def selectOption(self, receiverAddress):
        while True:
            option = int(input("Client: press 1 for psh or 0 for fin: "))
            if option == 1:
                self.sendPshAck(receiverAddress)
            elif option == 0:
                while True:
                    self.sendFin(receiverAddress)
                    ackNumber = self.awaitAck()
                    if ackNumber == self.seqNumber + 1:
                        self.seqNumber += 1
                        break

                while True:
                    print(self.nodeRole + ": Awaiting fin from server...")
                    rudpHeader, data, senderAddress = self.awaitPacket(5.0)
                    if rudpHeader is not None:
                        synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
                        if finFlag:
                            print("Server: Received fin from ({}, {}):".format(senderAddress[0], senderAddress[1]))
                            self.ackNumber += 1
                            break

                while True:     # await for server to receive the ack
                    print(self.nodeRole + ": Awaiting for server to close...")
                    self.sendAck(receiverAddress)

                    rudpHeader, data, senderAddress = self.awaitPacket(15.0)
                    if rudpHeader is not None:
                        synFlag, ackFlag, pshFlag, finFlag = hf.getFlagsUnpacked(rudpHeader[2])
                        if finFlag or ackFlag:
                            continue
                    break

                print("Client: Connection closed with ({}, {}) with seq number = {}.".format(receiverAddress[0],
                                                                                                  receiverAddress[1],
                                                                                                  ackNumber))
                break
