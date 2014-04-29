#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include "ui_viewerwindow.h"
#include "acercadedialog.h"
#include "preferencias.h"
#include "capturebuffer.h"
#include "preferencias.h"
#include "ui_preferencias.h"
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



namespace Ui {
class ViewerWindow;
}

class ViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ViewerWindow(QWidget *parent = 0);
    ~ViewerWindow();
    void workAsync(const QImage&);


signals:
    // Señal para comunicarnos con el hilo de trabajo
    void PasarImagen(const QImage& imagen);


private slots:
    void on_Quit_clicked();
    void on_actionSalir_triggered();
    void on_actionAbrir_triggered();
    void on_pause_clicked();
    void on_reproducir_stateChanged();
    void on_actionAcerca_de_triggered();
    void on_actionCapturar_triggered();
    void on_actionPreferencias_triggered();

    void errorsocket();
    void reconnect();

    void repro();

    // Slot para saber si la imagen ha sido trabajada
    void ImageWorked(const QImage &);
    void on_movie_updated(const QRect&);
    //Detección de movimiento
    void mostrar(const QImage &imagen, QVector<QRect> vRect);


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

    //Detección de movimiento
    QThread workingThread_;

    Worker Worker_;

};

#endif // VIEWERWINDOW_H
