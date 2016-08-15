#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QIODevice>
#include <QHostAddress>

#include <QDebug>

#include "TcpConnection.h"


class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(const QHostAddress &address, const quint16 &port, const QString &name);

public slots:


    void start();

    /**
     * @brief sendMessage
     *      Sends a message to server. Called from "returnPressed()" in "mainwindow.cpp"
     * @param message - The message obviously.
     */
    void sendMessage(const QString message);

    /**
     * @brief disconnectFromHost
     *      This slot is connected with the "Disconnect" action on the GUI Toolbar from the
     *      "mainwindow.c", "on_action_DisconnectTriggered".
     */
    inline void disconnectFromHost()
    {
        _myConn->disconnectFromHost();
    }



private slots:
    void readyRead(TcpConnection *server);

    void connected();

    void disconnected(TcpConnection *conn);


signals:
    void newMessage(QString from, QString message);
    void newParticipant(QString name);
    void participantLeft(QString name);

    /**
     * @brief sendMessage
     *      Sends a message to server. Called from "returnPressed()" in "mainwindow.cpp"
     * @param data - The message obviously.
     */
    void sendDataToServer(const QByteArray data);

private:
    QHostAddress _address;
    quint16 _port;
    QString _name;
    TcpConnection *_myConn;
};

#endif // TCPCLIENT_H
