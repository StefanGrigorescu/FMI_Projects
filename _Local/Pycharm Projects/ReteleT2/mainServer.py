import networknodes as nn
from servers import Server


host = nn.NetworkNode.localHost
serverPort = Server.sourcePort1
seqNumber = 5
pkgLossChance = 4
waitingTime = 5.0

server1 = Server(host, serverPort, seqNumber, pkgLossChance, waitingTime)
