#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QMovie>
#include <QSettings>
#include <QCamera>
#include <QCameraViewfinder>
#include <QPainter>
#include <QDateTime>
#include <QtNetwork>
#include "ayuda.h"
#include "preferencias_camara.h"
#include "vista_capturador.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_Boton_Cerrar_clicked();
    void on_actionAbrir_triggered();
    void on_actionSalir_triggered();
    void on_actionPreferencias_triggered();
    void on_actionAcerca_de_Visor_triggered();

    void on_Reproducir_clicked();
    void on_Parar_clicked();
    void on_Pausa_clicked();

    void on_chk_reproducir_stateChanged();
    void mostrar_marcos(const QImage &imagen);

    void on_actionCapturar_triggered();
    void rep_connected();


private:
    Ui::MainWindow *ui;
    QMovie *video;
    QSettings conf;
    QCamera *camara;
    QImage *imagen;
    //QCameraViewfinder *vista_captura;
    vista_capturador *vista_capt_propia;
    QPainter p;

    //NUEVO: Declaración del socket y sesión Qnetwork
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;

};

#endif // MAINWINDOW_H
