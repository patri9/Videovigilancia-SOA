#include "viewerwindow.h"


ViewerWindow::ViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    movie_(NULL),
    camera(NULL),
    viewfinder(NULL),
    captureBuffer(NULL),
    imagen(NULL),
    tcpServer(NULL),
    networkSession(0),
    clientConnection(NULL)
{
    ui->setupUi(this);

    status = false;

    movie_ = new QMovie();
    ui->label->setMovie(movie_);

    tcpServer = new QTcpServer(this);

    captureBuffer = new CaptureBuffer;
    connect(captureBuffer, SIGNAL(senial(const QImage&)), this, SLOT(mostrar(const QImage&)));

    //Desactivar los botones al iniciar la aplicación
    ui->playbutton->setDisabled(true);
    ui->stopbutton->setDisabled(true);
    ui->pause->setDisabled(true);

    connect(ui->playbutton, SIGNAL(clicked()), movie_, SLOT(start()));
    connect(ui->stopbutton, SIGNAL(clicked()), movie_, SLOT(stop()));

    QSettings conf1;
    ui->reproducir->setChecked(conf1.value("Reproducir", false).toBool());

    qRegisterMetaType< QImage >("QImage");
    qRegisterMetaType< QVector<QRect> > ("QVector<QRect>");
 }


ViewerWindow::~ViewerWindow()
{
    delete ui;
    delete movie_;
    delete captureBuffer;
    delete camera;
    delete viewfinder;
    delete imagen;
    delete tcpServer;
    delete clientConnection;
    delete networkSession;
}


void ViewerWindow::on_Quit_clicked()
{
    qApp->quit();
}


void ViewerWindow::on_actionSalir_triggered()
{
    qApp->quit();
}


void ViewerWindow::on_actionAbrir_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Abrir archivo",
                                                    QString(),
                                                    "Video (*.mjpeg)");
    movie_->setFileName(fileName);

    if (!movie_->isValid()) {
        QMessageBox::critical(this, tr("Error"),
                       tr("No se pudo abrir el archivo o el formato"
                          " no es inválido"));
        return;
    }
    else if(movie_->isValid()){
        ui->playbutton->setEnabled(true);
        ui->stopbutton->setEnabled(true);
        ui->pause->setEnabled(true);
        //Abrir el video sin reproducirlo
        movie_->start();
        movie_->stop();

        if(ui->reproducir->isChecked())
            movie_->start();
    }
}


void ViewerWindow::on_pause_clicked()
{
    movie_->setPaused(true);
}


void ViewerWindow::on_reproducir_stateChanged()
{
    QSettings conf;
    conf.setValue("Reproducir", ui->reproducir->isChecked());
}


//Acerca de...
void ViewerWindow::on_actionAcerca_de_triggered()
{
    AcercaDeDialog acerca;
    acerca.exec();
}


//Preferencias
void ViewerWindow::on_actionPreferencias_triggered()
{
    Preferencias pref;
    pref.exec();
}


//SLOT mostrar
void ViewerWindow::mostrar(const QImage &imagen)
{
      QPixmap pixmap;

      pixmap.convertFromImage(imagen);
      ui->label->setPixmap(pixmap);
}


//Captura WebCam
void ViewerWindow::on_actionCapturar_triggered()
{

 //   camera = new QCamera(conf.value("Camera", camara0).toByteArray());

    if (camera==NULL)
    {
        camera = new QCamera(conf.value("Camera").toByteArray());
        captureBuffer = new CaptureBuffer;
        camera->setViewfinder(captureBuffer);

        if(camera->state()!=QCamera::ActiveState)
        {
            connect(captureBuffer, SIGNAL(senial(const QImage&)), this, SLOT(mostrar(const QImage&)));

            camera->start();

        }
        else
        {
            if (camera->state()==QCamera::ActiveState)
            {
                camera->stop();
                delete camera;
            }
        }
    }
}


void ViewerWindow::send()
{
    qDebug() << "Nueva Conexión. ";

    clientConnection = tcpServer->nextPendingConnection();

    connect(clientConnection,SIGNAL(readyRead()),this, SLOT(readData()));
    connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)()),this, SLOT(FalloConexion()));
}


void ViewerWindow::readData()
{
    quint32 size_cab;
    quint32 size_pro;
    quint32 tam_cabecera;
    quint32 tam_nombre_camara;
    qint64 timestamp;
    qint32 tam=0;

    while(true)
    {
        if(status == false)
        {
            if(sizeof(tam) <= clientConnection->bytesAvailable())
            {
                clientConnection->read((char*)&tam,4);
                status = true;
            }
            else
                break;
        }

        if(status == true)
        {
            if(tam <= clientConnection->bytesAvailable())
            {
                QByteArray image = clientConnection->read(tam);
                QImage imagen;
                imagen.loadFromData(image, "jpeg");

                qint32 x,y,ancho,alto;
                qint32 num_rect;
                clientConnection->read((char*)&num_rect, sizeof(num_rect));
                QVector<QRect> vRect;
                for(int i=0; i<num_rect; i++)
                {
                    clientConnection->read((char*)&x,sizeof(x));
                    clientConnection->read((char*)&y,sizeof(y));
                    clientConnection->read((char*)&ancho,sizeof(ancho));
                    clientConnection->read((char*)&alto,sizeof(alto));

                    QRect rect(x,y,ancho,alto);
                    vRect.append(rect);
                }
                clientConnection->read((char*)&tam_cabecera,sizeof(tam_cabecera));
                clientConnection->read((char*)&size_cab,sizeof(size_cab));
                QByteArray cabecera  = clientConnection->read(size_cab);
                clientConnection->read((char*)&size_pro,sizeof(size_pro));
                QByteArray proto = clientConnection->read(size_pro);
                clientConnection->read((char*)&tam_nombre_camara,sizeof(tam_nombre_camara));
                QByteArray nombre_camara  = clientConnection->read(tam_nombre_camara);
                clientConnection->read((char*)&timestamp,sizeof(timestamp));

                qDebug() << "tam " << tam_cabecera;
                qDebug() << "Cabecera: " << cabecera;
                qDebug() << "Protocolo: " << proto;
                qDebug() << "Nombre Cámara: " << nombre_camara;
                qDebug() << "Timestamp: " << timestamp << " segundos";
                qDebug() << "";

                //Copia de la imagen.
                QImage frame = imagen;

                //Se pintan los rectángulos a partir de la imagen.
                QPainter pintor (&frame);
                QColor color(0, 255, 0, 255);
                pintor.setPen(color);
                pintor.drawRects(vRect);

                //Se convierte de QImage a QPixmap y se muestra.
                QPixmap pixmap;
                pixmap.convertFromImage(frame);
                ui->label->setPixmap(pixmap);

                status=false;
            }
            else
                break;
        }
    }
}


void ViewerWindow::FalloConexion()
{
    qDebug() << "Error de conexión: " << clientConnection->errorString();
    clientConnection->disconnect();
    clientConnection->deleteLater();
}


void ViewerWindow::on_actionCaptura_de_red_triggered()
{
     //QTcpServer a la espera de conexiones
    tcpServer = new QTcpServer(this);

    if (!tcpServer->listen(QHostAddress::Any, 15000))
    {
        QMessageBox::critical(this, tr("Server "),
                              tr("Unable to start the server: %1.")
                              .arg(tcpServer->errorString()));
        close();
        return;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(send()));

}
