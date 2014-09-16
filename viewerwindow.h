#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "ui_viewerwindow.h"
#include "acercadedialog.h"
#include "preferencias.h"
#include "capturebuffer.h"
#include "preferencias.h"
#include "ui_preferencias.h"
#include "capturebuffer.h"
#include "c_asincrono.h"
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
#include <QTcpServer>
#include <QByteArray>


namespace Ui {
class ViewerWindow;
}

class ViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *parent = 0);
    ~ViewerWindow();

public:
    //Variables para readData

    quint32 size_cab;
    quint32 size_pro;
    quint32 tam_cabecera;
    quint32 tam_nombre_camara;
    qint64 timestamp;
    qint32 tam;
    qint32 num_rect;
    qint32 sizeCabecera;
    qint32 TotalsizeCabecera;
    qint32 condicion;
    bool flag;

    qint32 cont;
    QString hex;
    QDir directorio;

    qint32 flag_port;

signals:
     void error(QAbstractSocket::SocketError socketError);

protected:
    void incomingConnection(int socketDescriptor);


private slots:
    void on_Quit_clicked();
    void on_actionSalir_triggered();
    void on_actionAbrir_triggered();
    void on_pause_clicked();
    void on_reproducir_stateChanged();
    void on_actionAcerca_de_triggered();
    void on_actionCapturar_triggered();
    void on_actionPreferencias_triggered();    
    void on_actionCaptura_de_red_triggered();

    void mostrar(const QImage &imagen);

    void send();

    void readData();

    void FalloConexion();

    void recibir_Dibujo(const QImage &imagen2);

private:
    Ui::ViewerWindow *ui;
    QMovie *movie_;
    QCamera *camera;
    QCameraViewfinder *viewfinder;
    CaptureBuffer *captureBuffer;    
    QImage *imagen;
    QSettings conf;

    //Server
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;
    QTcpSocket *clientConnection;

    C_Asincrono updated;

};

#endif // VIEWERWINDOW_H
