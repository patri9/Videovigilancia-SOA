#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QObject>
#include "getopt.h"
#include "capturebuffer.h"
#include "cvmatandqimage.h"
#include "worker.h"
#include <QApplication>
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
#include <QThread>
#include <QRect>
#include <vector>
#include <QIODevice>
#include <QDebug>
#include <QTcpSocket>
#include <QDialog>
#include <QSettings>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <getopt.h>
#include <QSocketNotifier>
#include <sys/socket.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

int setupUnixSignalHandlers();

class ViewerWindow : public QObject
{
    Q_OBJECT

public:
    explicit ViewerWindow(QObject *parent = 0);
    ~ViewerWindow();
    void workAsync(const QImage&);

    //Señales POSIX
    static void intSignalHandler(int unused);
    static void hupSignalHandler(int unused);

signals:
    // Señal para comunicarnos con el hilo de trabajo
    void PasarImagen(const QImage& imagen);

private slots:
    void Capturar();
    void errorsocket();
    void reconnect();
    void repro();

    //Detección de movimiento
    void mostrar(const QImage &imagen, QVector<QRect> vRect);

    // Slot donde atender la señal POSIX SIGTERM cuando la
    // convirtamos en una señal de Qt
    void handleSigInt();
    void handleSigHup();

private:
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

    //Detección de movimiento
    QThread workingThread_;

    Worker Worker_;


    // Pareja de sockets. Un par por señal a manejar.
    static int sigInt[2];
    static int sigHup[2];

    // Objeto para monitorizar la pareja de sockets
    QSocketNotifier *sigIntNotifier;
    QSocketNotifier *sigHupNotifier;

};

#endif // VIEWERWINDOW_H
