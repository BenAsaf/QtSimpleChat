#ifndef DIALOG_JOINSESSION_H
#define DIALOG_JOINSESSION_H

#include <QDialog>
#include <QMessageBox>
#include <QHostAddress>

namespace Ui {
class Dialog_JoinSession;
}

class Dialog_JoinSession : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_JoinSession(QWidget *parent = 0);
    ~Dialog_JoinSession();

    inline QHostAddress getServerAddress() const
    {
        return inputServerAddress;
    }

    inline quint16 getServerPort() const
    {
        return inputServerPort;
    }

    inline QString getClientName() const
    {
        return inputClientName;
    }


private slots:
    void on_Button_Cancel_clicked();

    void on_Button_Connect_clicked();

private:
    Ui::Dialog_JoinSession *ui;

    QHostAddress inputServerAddress;
    quint16 inputServerPort;
    QString inputClientName;
};

#endif // DIALOG_JOINSESSION_H
