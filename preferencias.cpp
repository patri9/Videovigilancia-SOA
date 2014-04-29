#include "preferencias.h"
#include "ui_preferencias.h"
#include <QString>

Preferencias::Preferencias(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferencias)
{
    ui->setupUi(this);

    QList<QByteArray> devices = QCamera::availableDevices();

    for (int i=0; i<devices.size(); i++){
        ui->comboBox->addItem(QCamera::deviceDescription(devices[i]), devices[i]);
        QString conficam = conf.value("Camera").toString();
        if(conficam == devices[i]){
            ui->comboBox->setCurrentIndex(i);
        }
    }

}

Preferencias::~Preferencias()
{
    delete ui;
}

void Preferencias::on_aceptar_clicked()
{
    conf.setValue("Camera", ui->comboBox->currentData());

    accept();
}


void Preferencias::on_cancelar_clicked()
{
    close();
}
