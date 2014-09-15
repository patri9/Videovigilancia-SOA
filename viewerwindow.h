#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "ui_viewerwindow.h"
#include "capturebuffer.h"
#include "ui_preferencias.h"
#include "capturebuffer.h"
#include "c_asincrono.h"
#include <QMainWindow>
#include <QObject>
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
#include <getopt.h>
#include <QSocketNotifier>
#include <sys/socket.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>
#include <fstream>
#include <stdio.h>

int setupUnixSignalHandlers();

class ViewerWindow : public QObject
{
    Q_OBJECT

public:
    explicit ViewerWindow(QObject *parent = 0);
    ~ViewerWindow();

    // Manejadores de señal POSIX
    static void hupSignalHandler(int unused);
    static void termSignalHandler(int unused);

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
    QByteArray image;

private slots:
    void on_Quit_clicked();
    void on_actionSalir_triggered();
    void on_actionCaptura_de_red_triggered();
    void readData();
    void send();
    void FalloConexion();

public slots:
    // Slots Qt donde atender las señales POSIX
    void handleSigHup();
    void handleSigTerm();

private:
    QCameraViewfinder *viewfinder;
    CaptureBuffer *captureBuffer;    
    QImage *imagen;
    //QSettings conf;

    //Server
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;
    QTcpSocket *clientConnection;

    int Port;
    QString Host;
    int intervalo;
    QTimer timer;
    QString device;
    QString Dir;

    // Pares de sockets. Un par por señal a manejar
    static int sigHupSd[2];
    static int sigTermSd[2];

   // Objetos para monitorizar los pares de sockets
   QSocketNotifier *sigHupNotifier;
   QSocketNotifier *sigTermNotifier;

};

#endif // VIEWERWINDOW_H
