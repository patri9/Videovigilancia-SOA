#include "preferencias_camara.h"
#include "ui_preferencias_camara.h"

Preferencias_Camara::Preferencias_Camara(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferencias_Camara)
{
    ui->setupUi(this);
    QList<QByteArray> combo_dispositivos = QCamera::availableDevices();
    foreach (QByteArray var,combo_dispositivos) {
        ui->Combo_disp->addItem(QCamera::deviceDescription(var));
    }
    ui->Combo_disp->setCurrentText(conf.value("Editor/combo_disp").toString());
}

Preferencias_Camara::~Preferencias_Camara()
{
    delete ui;
}

void Preferencias_Camara::on_pushButton_clicked()
{
    this->close();
}

void Preferencias_Camara::on_Combo_disp_currentTextChanged(const QString &arg1)
{
    conf.setValue("Editor/combo_disp",ui->Combo_disp->currentText());
}
