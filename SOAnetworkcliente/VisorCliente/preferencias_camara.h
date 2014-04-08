#ifndef PREFERENCIAS_CAMARA_H
#define PREFERENCIAS_CAMARA_H

#include <QDialog>
#include <QCamera>
#include <QSettings>
namespace Ui {
class Preferencias_Camara;
}

class Preferencias_Camara : public QDialog
{
    Q_OBJECT

public:
    explicit Preferencias_Camara(QWidget *parent = 0);
    ~Preferencias_Camara();

private slots:
    void on_pushButton_clicked();

    void on_Combo_disp_currentTextChanged(const QString &arg1);

private:
    Ui::Preferencias_Camara *ui;
    QSettings conf;
};

#endif // PREFERENCIAS_CAMARA_H
