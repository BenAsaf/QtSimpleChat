#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QHostAddress>
#include <QByteArray>

enum DataType {
    Login,
    PlainText,
    NewClient,
    ClientLeft,
    ListOfClients,
    Ping,
    Pong,
    Undefined
};

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnection();

    explicit TcpConnection(const TcpConnection &conn);


    inline void connectToHost(const QHostAddress &address, quint16 port)
    {
        this->mySocket->connectToHost(address, port);
    }

    inline void disconnectFromHost()
    {
        this->mySocket->disconnectFromHost();
    }

    inline void close()
    {
        this->mySocket->close();
    }

    inline void setSocketDescriptor(qintptr socketDescriptor)
    {
        this->mySocket->setSocketDescriptor(socketDescriptor);
    }

    inline qintptr socketDescriptor() const
    {
        return this->mySocket->socketDescriptor();
    }

    inline void setPeerName(const QString &name)
    {
        this->name = name;
    }

    inline QString peerName() const
    {
        return this->name;
    }

    inline QHostAddress peerAddress() const
    {
        return this->mySocket->peerAddress();
    }

    inline QByteArray readAll() const
    {
        return this->mySocket->readAll();
    }

    static int parseHeaderAndData(QByteArray &buffer, QString &data);
    static QString parseSenderAndMessage(QString &data);
    static QStringList parseClientsList(QString &data);



public slots:
    inline qint64 write(const QByteArray data) const
    {
        return this->mySocket->write(data);
    }

signals:

    void connected();
    void disconnected(TcpConnection *conn);
    void readyRead(TcpConnection *conn);

private slots:
    inline void readyRead()
    {
        emit readyRead(this);
    }

    inline void disconnected()
    {
        emit disconnected(this);
    }

private:
    QTcpSocket *mySocket;
    QString name;
};

#endif // TCPCONNECTION_H
