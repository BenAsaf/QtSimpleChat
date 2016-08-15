#ifndef DIALOG_HOSTSESSION_H
#define DIALOG_HOSTSESSION_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Dialog_HostSession;
}

class Dialog_HostSession : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_HostSession(QWidget *parent = 0);
    ~Dialog_HostSession();

    inline QString getName() const
    {
        return inputName;
    }

    inline quint16 getPortNumber() const
    {
        return inputPortNumber;
    }

    inline int getMaxClients() const
    {
        return inputMaxClients;
    }

    inline bool isLocalServer() const
    {
        return isLocal;
    }

private slots:
    void on_Button_Cancel_clicked();

    void on_Button_HostSession_clicked();

private:
    Ui::Dialog_HostSession *ui;

    QString inputName;
    uint inputPortNumber;
    int inputMaxClients;
    bool isLocal;
};

#endif // DIALOG_HOSTSESSION_H
