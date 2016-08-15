#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(this->geometry().width(),this->geometry().height());

    this->serverThread = new QThread();
    this->clientThread = new QThread();

    this->isInSession = false;  // is the user currently in a session
    tableFormat.setBorder(0);


    QLineEdit * const inputMessage = ui->LineEdit_InputMessage;
    connect(inputMessage, SIGNAL(returnPressed()), this, SLOT(returnPressed()));

    // Maybe start Client with the app???
}

MainWindow::~MainWindow()
{
    delete ui;
    if (serverThread != nullptr)
    {
        serverThread->quit();
        serverThread->wait();
        delete serverThread;
    }
    if (clientThread != nullptr)
    {
        clientThread->quit();
        clientThread->wait();
        delete clientThread;
    }
}

/**
 * @brief MainWindow::on_action_About_triggered
 * Display the "About" of the creator of the chat.
 */
void MainWindow::on_action_About_triggered()
{
    Dialog_About about(this);
    about.setModal(true);
    about.exec();
    this->activateWindow();
}
void MainWindow::on_action_Quit_triggered()
{
    QApplication::quit();
}
void MainWindow::on_action_Disconnect_triggered()
{
    if (isInSession)
    {
        // Close Client
        // Close Server
        emit disconnect();
        isInSession = false;
        serverThread->quit();
        serverThread->wait();
        clientThread->quit();
        clientThread->wait();
        //QMessageBox::information(this, tr("Info"), tr("Disconnected."));
        appendLogMessage("Server stopped.");
        ui->List_ClientList->clear();
    }
}

void MainWindow::on_action_HostSession_triggered()
{
    if (isInSession)
    {
        QMessageBox::critical(this, tr("EEEHH!!"), tr("You are already in a session! WTF!"));
        return;
    }

    Dialog_HostSession hostSession(this);
    hostSession.setModal(true);
    const int RESULT = hostSession.exec();
    this->activateWindow();
    if (RESULT == QDialog::Accepted)
    {
        isInSession = true;  // Restricting user from Hosting/Joining since now he is in session.
        // Self Server related:
        bool IS_LOCAL_SERVER = hostSession.isLocalServer();
        const quint16 PORT = hostSession.getPortNumber();
        const int MAX_CLIENTS = hostSession.getMaxClients();
        startServer(IS_LOCAL_SERVER, PORT, MAX_CLIENTS);

        /////////////////////////////////////////////////////
        // Self Client related:
        const QString CLIENT_NAME = hostSession.getName();
        startClient(QHostAddress::LocalHost, myServer->serverPort(), CLIENT_NAME);
        /////////////////////////////////////////////////////
    }
}

void MainWindow::on_action_JoinSession_triggered()
{
    if (isInSession)
    {
        QMessageBox::critical(this, tr("EEEHH!!"), tr("You are already in a session! WTF!"));
        return;
    }
    Dialog_JoinSession joinSession;
    joinSession.setModal(true);
    const int RESULT = joinSession.exec();
    this->activateWindow();
    if (RESULT == QDialog::Accepted)
    {
        isInSession = true;  // Restricting user from Hosting/Joining since now he is in session.

        const QHostAddress SERVER_ADDRESS = joinSession.getServerAddress();
        const quint16 SERVER_PORT = joinSession.getServerPort();
        const QString CLIENT_NAME = joinSession.getClientName();

        startClient(SERVER_ADDRESS, SERVER_PORT, CLIENT_NAME);
    }
}

void MainWindow::startServer(const bool IS_LOCAL_SERVER, const int PORT, const int MAX_CLIENTS)
{
    myServer = new TcpServer(IS_LOCAL_SERVER, PORT, MAX_CLIENTS);
    myServer->moveToThread(serverThread);
    connect(serverThread, SIGNAL(started()), myServer, SLOT(start()));
    connect(this, SIGNAL(disconnect()), myServer, SLOT(stop()), Qt::QueuedConnection);

    serverThread->start();

    /**
     * @brief loop
     *      Simple wait event loop until the server is actually running. AVOIDING NULL EXCEPTION
     */
    QEventLoop loop;
    loop.connect(myServer, SIGNAL(running()), SLOT(quit()));
    loop.exec();

    appendLogMessage("Server started on " + myServer->serverAddress().toString() + ":" + QString::number(myServer->serverPort()));
}

void MainWindow::startClient(const QHostAddress &serverAddress, const quint16 &serverPort, const QString &clientName)
{
    myClient = new TcpClient(serverAddress, serverPort, clientName);
    myClient->moveToThread(clientThread);

    connect(clientThread, SIGNAL(started()), myClient, SLOT(start()));
    connect(this, SIGNAL(sendMessage(const QString)), myClient, SLOT(sendMessage(const QString)), Qt::QueuedConnection);
    connect(myClient, SIGNAL(newMessage(QString, QString)), this, SLOT(appendMessage(QString, QString)));
    connect(myClient, SIGNAL(newParticipant(QString)), this, SLOT(newParticipant(QString)));
    connect(myClient, SIGNAL(participantLeft(QString)), this, SLOT(participantLeft(QString)));
    connect(this, SIGNAL(disconnect()), myClient, SLOT(disconnectFromHost()));

    clientThread->start();
}

void MainWindow::appendLogMessage(QString msg)
{
    QTextEdit * const chatbox = ui->textEdit_ChatMessages;

    QColor color = chatbox->textColor();
    chatbox->setTextColor(Qt::blue);
    chatbox->append("[" + QTime::currentTime().toString() + "] " + msg);
    chatbox->setTextColor(color);
}

void MainWindow::appendMessage(QString from, QString message)
{
    if (from.isEmpty() || message.isEmpty())
    {
        return;
    }
    QTextEdit * const chatbox = ui->textEdit_ChatMessages;

    QTextCursor cursor(chatbox->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText("[" + QTime::currentTime().toString() + "]" + " <" + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = chatbox->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MainWindow::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
    {
        return;
    }
    QTextEdit * const chatbox = ui->textEdit_ChatMessages;
    QListWidget * const clientsList = ui->List_ClientList;

    QColor color = chatbox->textColor();
    chatbox->setTextColor(Qt::gray);
    chatbox->append("[" + QTime::currentTime().toString() + "] " + nick + " has joined.");
    chatbox->setTextColor(color);
    clientsList->addItem(nick);
}
void MainWindow::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
    {
        return;
    }
    QTextEdit * const chatbox = ui->textEdit_ChatMessages;
    QListWidget * const clientsList = ui->List_ClientList;

    QList<QListWidgetItem *> items = clientsList->findItems(nick, Qt::MatchExactly);
    if (items.isEmpty())
    {
        return;
    }
    delete items.at(0);
    QColor color = chatbox->textColor();
    chatbox->setTextColor(Qt::gray);
    chatbox->append("[" + QTime::currentTime().toString() + "] " + nick + " has disconnected.");
    chatbox->setTextColor(color);
}

void MainWindow::returnPressed()
{
    QTextEdit * const chatbox = ui->textEdit_ChatMessages;
    QLineEdit * const inputMessage = ui->LineEdit_InputMessage;
    const QString text = inputMessage->text();
    if (text.isEmpty())
    {
        return;
    }
    else if (!isInSession)  // If not in session and user is bored then just delete the text.
    {
         inputMessage->clear();
         return;
    }
    else if (text.startsWith(QChar('/')))
    {
        QColor color = chatbox->textColor();
        chatbox->setTextColor(Qt::red);
        chatbox->append(tr("! Unknown command: %1").arg(text.left(text.indexOf(' '))));
        chatbox->setTextColor(color);
    }
    else
    {
        emit sendMessage(text);
        //appendMessage(myClient->getPeerName(), text);
    }
    inputMessage->clear();
}
