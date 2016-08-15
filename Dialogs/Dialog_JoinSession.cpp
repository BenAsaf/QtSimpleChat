#include "Dialog_JoinSession.h"
#include "ui_dialog_joinsession.h"

const QString DEFAULT_PORT = "0";

Dialog_JoinSession::Dialog_JoinSession(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_JoinSession)
{
    ui->setupUi(this);
    // Default port number:
    ui->Input_Port->setText(DEFAULT_PORT);
}

Dialog_JoinSession::~Dialog_JoinSession()
{
    delete ui;
}

void Dialog_JoinSession::on_Button_Cancel_clicked()
{
    this->close();
}

void Dialog_JoinSession::on_Button_Connect_clicked()
{
    if (ui->Input_Name->text().size() > 0 && !ui->Input_Name->text().contains(" ") && ui->Input_Port->text().size() != 0)
    {
        inputClientName = ui->Input_Name->text();
        inputServerAddress = ui->Input_ServerAddress->text();
        inputServerPort = ui->Input_Port->text().toInt();
    }
    else
    {
        QMessageBox::information(this, tr("Meh, problem."), tr("Empty name or\and empty port."));
        this->setResult(QDialog::Rejected);
    }
}
