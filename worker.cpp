#include "worker.h"
#include "cvmatandqimage.h"


Worker::Worker()
{
    parar = false;
    //Crea un backgroundSubtractor y desactiva la deteccción de sombras
    backgroundSubtractor = new cv::BackgroundSubtractorMOG2(500, 16,false);
    backgroundSubtractor->set("nmixtures",3);
}


Worker::~Worker()
{
}


void Worker::doWork(const QImage &imagen){

    //qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    if (parar == true)
        exit(0);

    // Definimos algunos tipos para que el código se lea mejor
    QVector<QRect> vRect;

    // Pasa la imagen de Qimage a cv::Mat para ser procesada por opencv
    cv::Mat imagemat = QtOcv::image2Mat(imagen,CV_8UC(3));
    // Sustracción del fondo
    (*backgroundSubtractor)(imagemat,foregroundMask);

    // Operaciones morfolóficas para eliminar las regiones de
    // pequeño tamaño. Erode() las encoge y dilate() las vuelve a
    // agrandar.    
    cv::erode(foregroundMask, foregroundMask, cv::Mat(),cv::Point(-1,-1),3);
    cv::dilate(foregroundMask, foregroundMask, cv::Mat(),cv::Point(-1,-1),3);

    // Obtener los contornos que bordean las regiones externas
    // (CV_RETR_EXTERNAL) encontradas. Cada contorno es un vector
    // de puntos y se devuelve uno por región en la máscara.
    ContoursType contours;
    cv::findContours(foregroundMask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


    //Pasa el vector de contornos(rect) a vector de Qrect
    vRect.clear();
    for (ContoursType::const_iterator i = contours.begin(); i < contours.end(); ++i)
    {
        cv::Rect rect = cv::boundingRect(*i);
        QRect qrect (rect.x, rect.y, rect.width, rect.height);
        vRect.push_back (qrect);
    }

    //Mandar la imagen procesada al hilo
    emit ImageWorked(imagen, vRect);
}
