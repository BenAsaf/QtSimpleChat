#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QAbstractSocket>

#include <QThreadPool>
#include <QRunnable>

#include <QHash>
#include <QByteArray>

#include <QDebug>
// End of Tasks include //
#include "TcpConnection.h"

const QString SPACER = " ";


class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(const bool &isLocalServer, const quint16 &portNumber, const int &maxClients);


    inline quint16 serverPort() const
    {
        return _myServer->serverPort();
    }

    inline QHostAddress serverAddress() const
    {
        return _myServer->serverAddress();
    }

signals:
    void running();

public slots:
    void start();


    void stop();


private slots:
    void newConnection();
    void processReadyRead(TcpConnection *conn);
    void clientDisconnected(TcpConnection *conn);
    void sendToAll(const QByteArray &data, const QSet<QHostAddress> *ignoreSet) const;
    void incomingMessage(const TcpConnection *conn, const QString &message) const;
    void greetClient(TcpConnection *conn, const QString &data);

private:
    bool _isLocal;
    quint16 _port;
    int _maxClients;
    QTcpServer *_myServer;
    QHash<QHostAddress, TcpConnection*> _connectedClients;
};

#endif // TCPSERVER_H
