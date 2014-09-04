#include "acercadedialog.h"
//#include "ui_acercadedialog.h"

AcercaDeDialog::AcercaDeDialog(QWidget *parent) :
    QDialog(parent),
    //ui(new Ui::AcercaDeDialog)
{
    ui->setupUi(this);
}

AcercaDeDialog::~AcercaDeDialog()
{
    delete ui;
}

void AcercaDeDialog::on_buttonBox_accepted()
{
    accept();
}
