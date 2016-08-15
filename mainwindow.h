#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextTableFormat>
#include <QTextTable>
#include <QScrollBar>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QThread>

// Dialogs include:
#include "Dialogs\Dialog_JoinSession.h"
#include "Dialogs\Dialog_HostSession.h"
#include "Dialogs\Dialog_About.h"
// Meh includes:
#include "TcpServer.h"
#include "TcpClient.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    /**
     * @brief disconnect
     *      Signal to close Server/Client.
     *      Emitted from "on_action_Disconnect_triggered()"
     */
    void disconnect();

    void sendMessage(const QString message);

private slots:
    // Tool bar related actions START//
    void on_action_Quit_triggered();
    void on_action_JoinSession_triggered();
    void on_action_HostSession_triggered();
    void on_action_About_triggered();
    void on_action_Disconnect_triggered();
    // Tool Bar related END //

    /**
     * @brief startServer
     *      Called from "on_action_HostSession_triggered()".
     *      It creates a thread and runs the server given the following parameters from the Host-Session Dialog.
     *      NOTE: The thread will be removed since it's parent is being assigned to the instance of "mainwindow"
     * @param isLocalServer - Boolean value wether or not the server is being run locally or not.
     * @param port - Port number
     * @param maxClients - Maximum number of clients
     */
    void startServer(const bool isLocalServer, const int port, const int maxClients);


    /**
     * @brief startClient
     *      Called from "on_action_JoinSession_triggered()".
     *      It creates a thread and runs the client given the following parameters from the Join-Session Dialog
     *      NOTE: The thread will be removed since it's parent is being assigned to the instance of "mainwindow"
     * @param serverAddress - Server address
     * @param serverPort - Server port number
     * @param clientName - Name of the client
     */
    void startClient(const QHostAddress &serverAddress, const quint16 &serverPort, const QString &clientName);

    /**
     * @brief newMessage
     *      When client receieves a message it will be added to the GUI.
     * @param from - Sender
     * @param message - Message
     */
    void appendMessage(QString from, QString message);

    void appendLogMessage(QString msg);

    /**
     * @brief newParticipant
     *      When a new client joins the server.
     * @param nick - Name of the client to remove from the Connected Clients list.
     */
    void newParticipant(const QString &nick);

    /**
     * @brief participantLeft
     *      When a client left the server.
     * @param nick - Name of the client to remove from the Connected Clients list.
     */
    void participantLeft(const QString &nick);

    /**
     * @brief returnPressed
     *      Send a message to Server. This event is triggered when the user presses Enter when in text input.
     */
    void returnPressed();

private:

    Ui::MainWindow *ui;
    QString myName;
    QTextTableFormat tableFormat;
    TcpServer *myServer;
    TcpClient *myClient;

    QThread *serverThread;
    QThread *clientThread;

    bool isInSession;
};

#endif // MAINWINDOW_H
