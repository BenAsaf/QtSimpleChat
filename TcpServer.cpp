#include "TcpServer.h"



TcpServer::TcpServer(const bool &isLocalServer, const quint16 &portNumber,const int &maxClients) :
    _isLocal(isLocalServer), _port(portNumber), _maxClients(maxClients), _connectedClients()
{
}

void TcpServer::start()
{
        _myServer = new QTcpServer();
        _myServer->setMaxPendingConnections(_maxClients);

        connect(_myServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

        const QHostAddress address = _isLocal ? QHostAddress::LocalHost : QHostAddress::Any;

        if (_myServer->listen(address, this->_port))
        {
            qDebug() << "Server: Server started.";
            emit running();
        }
        else
        {
            qCritical() << "Server: ERROR. Server could not be started.\n";
            stop();
        }
}

void TcpServer::stop()
{
    qDebug() << "Server: Shutting down.";
    foreach (TcpConnection *cur, _connectedClients.values())
    {
        cur->close();
        cur->deleteLater();
    }
    _myServer->close();
    this->deleteLater();
}

void TcpServer::newConnection()
{
    TcpConnection *newConn = new TcpConnection();
    newConn->setSocketDescriptor(_myServer->nextPendingConnection()->socketDescriptor());
    connect(newConn, SIGNAL(readyRead(TcpConnection*)), this, SLOT(processReadyRead(TcpConnection*)));
    connect(newConn, SIGNAL(disconnected(TcpConnection*)), this, SLOT(clientDisconnected(TcpConnection*)));
    qDebug() << "Server: Incoming new connection.";
}

void TcpServer::greetClient(TcpConnection *conn, const QString &data)
{
    conn->setPeerName(data);
    qDebug() << "Server:" << conn->peerName() << "joined the server." << "\t" << "from IP:" << conn->peerAddress().toString();
    // Now sending the new client the connected clients list if there is so
    if (_connectedClients.count() > 0)  // If there are more clients then let the new client know it.
    {
        // Constructing the list of clients:
        QString list = QString::number(DataType::ListOfClients);
        /**
        * Generating a list of all connected clients for the new client while sending each client that a new client has joined.
        */
        const QByteArray NEW_CLIENT_DATA = (QString::number(DataType::NewClient) + SPACER + conn->peerName()).toUtf8();
        foreach (const TcpConnection *cur, this->_connectedClients.values())
        {
            list.append(" " + cur->peerName());  // Appending the existing clients name for a list made for the new client.
            cur->write(NEW_CLIENT_DATA);  // Telling existing client a new client has joined
        }
        conn->write(list.toUtf8());
    }
    _connectedClients.insert(conn->peerAddress(), conn);  // Adding the new client.
}


void TcpServer::processReadyRead(TcpConnection *conn)
{
    // Task to read and parse the client request.
    // Future plans? sending files, ping, message.
    QByteArray buffer = conn->readAll();
    QString data;
    const int DATA_TYPE = TcpConnection::parseHeaderAndData(buffer, data);
    switch (DATA_TYPE)
    {
    case Login:
        greetClient(conn, data);
        break;
    case PlainText:
        incomingMessage(conn, data);
        break;
    case Ping:
        break;
    case Pong:
        break;
    default:
        break;
    }
}

void TcpServer::clientDisconnected(TcpConnection *conn)
{
    qDebug() << "Server:" << conn->peerName() << "has disconnected.";
    _connectedClients.remove(conn->peerAddress());
    const QString DATA = QString::number(DataType::ClientLeft) + SPACER + conn->peerName();
    sendToAll(DATA.toUtf8(), nullptr);
    conn->close();
}

void TcpServer::incomingMessage(const TcpConnection *conn, const QString &data) const
{
    qDebug() << conn->peerName() << ":" << data;
    const QString DATA = QString::number(DataType::PlainText) + SPACER + conn->peerName() + SPACER + data;
    sendToAll(DATA.toUtf8(), nullptr);
}

void TcpServer::sendToAll(const QByteArray &data, const QSet<QHostAddress> *ignoreSet) const
{
    foreach (const TcpConnection *cur, _connectedClients.values())
    {
        if (ignoreSet != nullptr && ignoreSet->contains(cur->peerAddress()))
        {
            continue;
        }
        cur->write(data);
    }
}

