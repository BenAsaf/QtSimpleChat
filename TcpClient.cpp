#include "TcpClient.h"

TcpClient::TcpClient(const QHostAddress &address, const quint16 &port, const QString &name) : _address(address), _port(port), _name(name)
{
}

void TcpClient::start()
{
    qDebug() << "Client: Connecting to host on" << _address.toString() << ":" << _port;
    _myConn = new TcpConnection();
    _myConn->setPeerName(_name);
    connect(_myConn, SIGNAL(readyRead(TcpConnection*)), this, SLOT(readyRead(TcpConnection*)));
    connect(_myConn, SIGNAL(connected()), this, SLOT(connected()));
    connect(_myConn, SIGNAL(disconnected(TcpConnection*)), this, SLOT(disconnected(TcpConnection*)));
    _myConn->connectToHost(_address, _port);
}

void TcpClient::sendMessage(const QString message)
{
    QString data = QString::number(DataType::PlainText) + " " + message;
    _myConn->write(data.toUtf8());
}

////////////////////////////////////////////////////////////////////////
////////////////////////////// SLOTS ///////////////////////////////////
////////////////////////////////////////////////////////////////////////

void TcpClient::connected()
{
    qDebug() << "Client: Connected to host!";
    QString data = QString::number(DataType::Login) + " " + this->_myConn->peerName();
    this->_myConn->write(data.toUtf8());
    emit newParticipant(_myConn->peerName());
}

void TcpClient::disconnected(TcpConnection *conn)
{
    qDebug() << "Client: Disconnected from server.";
    _myConn->close();
    _myConn->deleteLater();
    this->deleteLater();
}


void TcpClient::readyRead(TcpConnection *server)
{
    // We can safely ignore the parameter. It is mainly used for the server to know which connection just sent information.
    QByteArray buffer = this->_myConn->readAll();
    QString data;
    const int DATA_TYPE = TcpConnection::parseHeaderAndData(buffer, data);
    switch (DATA_TYPE)
    {
    case PlainText:
    {
        QString sender = TcpConnection::parseSenderAndMessage(data);
        emit newMessage(sender, data);
    }
        break;
    case ListOfClients:
    {
        foreach (const QString &name, TcpConnection::parseClientsList(data))
        {
            emit newParticipant(name);
        }
    }
        break;
    case NewClient:
        emit newParticipant(data);
        break;
    case ClientLeft:
        emit participantLeft(data);
        break;
    case Ping:
        break;
    case Pong:
        break;
    }
}

