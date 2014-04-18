#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "ui_viewerwindow.h"
#include "acercadedialog.h"
#include "preferencias.h"
#include "capturebuffer.h"
#include "preferencias.h"
#include "ui_preferencias.h"
#include "capturebuffer.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QImageReader>
#include <QTextStream>
#include <QMessageBox>
#include <QWaitCondition>
#include <QMutex>
#include <QMovie>
#include <QSettings>
#include <QCamera>
#include <QCameraViewfinder>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
#include <QImage>
#include <QPainter>
#include <QSettings>
#include <QtNetwork>
#include <QTimer>

namespace Ui {
class ViewerWindow;
}

class ViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *parent = 0);
    ~ViewerWindow();

private slots:
    void on_Quit_clicked();

    void on_actionSalir_triggered();

    void on_actionAbrir_triggered();

    void on_pause_clicked();

    void on_reproducir_stateChanged();

    void on_actionAcerca_de_triggered();

    void on_actionCapturar_triggered();

    void on_actionPreferencias_triggered();

    void mostrar(const QImage &imagen);

    void errorsocket();

    void reconnect();

    void repro();

private:
    Ui::ViewerWindow *ui;
    QMovie *movie_;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    CaptureBuffer *captureBuffer;    
    QImage *imagen;
    QSettings conf;

    //Cliente
    QTcpSocket *tcpSocket;
    QNetworkSession *networkSession;

    //
    int Port;
    QString Host;
    int intervalo;
    QTimer timer;
    QString device;

};

#endif // VIEWERWINDOW_H
