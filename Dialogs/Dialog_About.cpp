#include "Dialog_About.h"
#include "ui_dialog_about.h"

Dialog_About::Dialog_About(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog_About)
{
    ui->setupUi(this);
    setFixedSize(this->geometry().width(),this->geometry().height());
}

Dialog_About::~Dialog_About()
{
    delete ui;
}

void Dialog_About::on_Button_OK_clicked()
{
    this->close();
}
