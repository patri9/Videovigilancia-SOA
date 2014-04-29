#ifndef WORKER_H
#define WORKER_H

#include <QApplication>
#include <QMainWindow>
#include <QThread>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QIODevice>
#include <QImageReader>
#include <QMovie>
#include <QWaitCondition>
#include <QMutex>
#include <QRect>
#include <QDebug>
#include <QPainter>
#include <QTcpSocket>

#include <ui_viewerwindow.h>

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cvmatandqimage.h"



typedef std::vector<cv::Mat> ImagesType;
typedef std::vector<std::vector<cv::Point> > ContoursType;

class Worker : public QObject
{
    Q_OBJECT

public:
    cv::BackgroundSubtractorMOG2 *backgroundSubtractor;
    //Fondo
    cv::Mat foregroundMask;

    bool parar;

    Worker();
    ~Worker();

signals:
    // Señal emitida cuando la imagen ha sido trabajada
    void ImageWorked(const QImage& , QVector<QRect> );

public slots:
    // Método encargado de trabajar la imagen
    void doWork(const QImage& imagen);
};

#endif // WORKER_H
