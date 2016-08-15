#include "Dialog_HostSession.h"
#include "ui_dialog_hostsession.h"

const QString DEFAULT_PORT = "0";

Dialog_HostSession::Dialog_HostSession(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_HostSession),
                                                            inputName(""), inputPortNumber(0), inputMaxClients(0)
{
    ui->setupUi(this);

    //Server.serverPort()


    // Default port number:
    ui->Input_Port->setText(DEFAULT_PORT);
}

Dialog_HostSession::~Dialog_HostSession()
{
    delete ui;
}

void Dialog_HostSession::on_Button_Cancel_clicked()
{
    this->close();
}

void Dialog_HostSession::on_Button_HostSession_clicked()
{
    if (ui->Input_Name->text().size() > 0 && !ui->Input_Name->text().contains(" "))
    {
        inputName = ui->Input_Name->text();
        inputPortNumber = ui->Input_Port->text().toUInt();
        inputMaxClients = ui->spinBox_MaxClients->text().toInt();
        isLocal = ui->checkBox_IsLocalServer->isChecked();
    }
    else
    {
        QMessageBox::information(this, tr("Meh, problem."), tr("You must enter thy name."));
        this->setResult(QDialog::Rejected);
    }
}
