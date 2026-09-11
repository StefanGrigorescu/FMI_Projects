import networknodes as nn
from clients import Client
from servers import Server

host = nn.NetworkNode.localHost
clientPort = Client.sourcePort1
serverPort = Server.sourcePort1
seqNumber = 0
pkgLossChance = 4
waitingTime = 5.0

client1 = Client(host, clientPort, seqNumber, pkgLossChance, waitingTime)

client1.sendSyn(host, serverPort)


