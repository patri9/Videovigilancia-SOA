#ifndef ACERCADEDIALOG_H
#define ACERCADEDIALOG_H

#include <QDialog>

namespace Ui {
class AcercaDeDialog;
}

class AcercaDeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AcercaDeDialog(QWidget *parent = 0);
    ~AcercaDeDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AcercaDeDialog *ui;
};

#endif // ACERCADEDIALOG_H
