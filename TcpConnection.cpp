#include "TcpConnection.h"

TcpConnection::TcpConnection() : QObject()
{
    this->mySocket = new QTcpSocket();
    this->name = mySocket->peerName();
    connect(mySocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(mySocket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(mySocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

TcpConnection::TcpConnection(const TcpConnection &conn) : TcpConnection()
{
    this->mySocket->setSocketDescriptor(conn.socketDescriptor());
}


int TcpConnection::parseHeaderAndData(QByteArray &buffer, QString &data)
{
    data = QString::fromUtf8(buffer);
    const int SPLIT_INDEX = data.indexOf(" ");

    const int DATA_TYPE = data.left(SPLIT_INDEX).toInt();
    data = data.mid(SPLIT_INDEX+1);

    return DATA_TYPE;
}

QString TcpConnection::parseSenderAndMessage(QString &data)
{
    const int SPLIT_INDEX = data.indexOf(" ");

    QString sender = data.left(SPLIT_INDEX);
    data = data.mid(SPLIT_INDEX+1);
    return sender;
}

QStringList TcpConnection::parseClientsList(QString &data)
{
    return data.split(" ");
}
