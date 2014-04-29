#ifndef PREFERENCIAS_H
#define PREFERENCIAS_H

#include <QSettings>
#include "viewerwindow.h"

namespace Ui {
class Preferencias;
}

class Preferencias : public QDialog
{
    Q_OBJECT

public:
    explicit Preferencias(QWidget *parent = 0);
    ~Preferencias();

private slots:
    void on_aceptar_clicked();

    void on_cancelar_clicked();

private:
    Ui::Preferencias *ui;
    QSettings conf;
};

#endif // PREFERENCIAS_H
