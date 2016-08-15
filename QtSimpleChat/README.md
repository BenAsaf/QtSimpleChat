# QtSimpleChat
Simple chat using QT C++

Known bugs are:
1) Disconnect action might keep the socket alive, causing the Client to stay connected even though he left.
Hence it doesn't declare that the user left the server.
2) Suspicion that the Client/Server destructors are not working properly.
