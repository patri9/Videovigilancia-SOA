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

    tam = 0;
    condicion = 0;

    imagen = new QImage();

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

    flag = true;
    qint32 TotalsizeRect;
    qint32 x,y,ancho,alto;
    QVector<QRect> vRect;



    while(flag)
    {
            switch (condicion)
            {
            case 0:

                sizeCabecera = sizeof(tam_cabecera);
                if( sizeCabecera <= clientConnection->bytesAvailable())
                {
                    clientConnection->read((char*)&tam_cabecera,sizeof(tam_cabecera));
                    qDebug() << "Tamaño Cabecera Antes: " << tam_cabecera;
                    condicion = 1;
                }
                else
                {
                    condicion = 0;
                    flag = false;
                    break;
                }
            case 1:

                TotalsizeCabecera = tam_cabecera;
                if(TotalsizeCabecera <= clientConnection->bytesAvailable())
                {
                    qDebug() << "Tamaño Cabecera Despues: " << tam_cabecera;
                    clientConnection->read((char*)&size_cab,sizeof(size_cab));
                    QByteArray cabecera  = clientConnection->read(size_cab);
                    qDebug() << "Cabecera: " << cabecera;
                    clientConnection->read((char*)&size_pro,sizeof(size_pro));
                    QByteArray proto = clientConnection->read(size_pro);
                    qDebug() << "Protocolo: " << proto;
                    clientConnection->read((char*)&tam_nombre_camara,sizeof(tam_nombre_camara));
                    QByteArray nombre_camara  = clientConnection->read(tam_nombre_camara);
                    qDebug() << "Nombre Cámara: " << nombre_camara;
                    clientConnection->read((char*)&timestamp,sizeof(timestamp));

                    qDebug() << "Timestamp: " << timestamp << " segundos";
                    qDebug() << "";

                    condicion = 2;
                }
                else
                {
                    condicion = 1;
                    flag = false;
                    break;
                }

            case 2:

                if(sizeof(tam) <= clientConnection->bytesAvailable())
                {
                    clientConnection->read((char*)&tam,4);
                    qDebug() << "Tamaño Imagen: " << tam;
                    condicion = 3;
                }
                else
                {
                    condicion = 2;
                    flag = false;
                    break;
                }



            case 3:

                if(tam <= clientConnection->bytesAvailable())
                {
                    QByteArray image = clientConnection->read(tam);
                    imagen->loadFromData(image, "jpeg");
                    condicion = 4;
                }
                else
                {
                    condicion = 3;
                    flag = false;
                    break;
                }


            case 4:

                if(sizeof(num_rect) <= clientConnection->bytesAvailable())
                {
                    clientConnection->read((char*)&num_rect, sizeof(num_rect)); //Almaceno aqui el numero de rectangulos para el sig checkpoint
                    qDebug() << "Numero de rectangulos : " << num_rect;
                    condicion = 5;
                }
                else
                {
                    condicion = 4;
                    flag = false;
                    break;
                }



            case 5:


                TotalsizeRect = sizeof(x)*4*num_rect;
                if(TotalsizeRect <= clientConnection->bytesAvailable())
                {
                    for(int i=0; i<num_rect; i++)
                    {
                        clientConnection->read((char*)&x,sizeof(x));;
                        clientConnection->read((char*)&y,sizeof(y));
                        clientConnection->read((char*)&ancho,sizeof(ancho));
                        clientConnection->read((char*)&alto,sizeof(alto));

                        QRect rect(x,y,ancho,alto);
                        vRect.append(rect);
                    }
                    //Final segundo checkpoint

                    condicion = 6;
                }
                else
                {
                    condicion = 5;
                    flag = false;
                    break;
                }


            case 6:

                    //Copia de la imagen.
                    QImage frame = *imagen;

                    //Se pintan los rectángulos a partir de la imagen.
                    QPainter pintor (&frame);
                    QColor color(0, 255, 0, 255);
                    pintor.setPen(color);
                    pintor.drawRects(vRect);

                    //Se convierte de QImage a QPixmap y se muestra.
                    QPixmap pixmap;
                    pixmap.convertFromImage(frame);
                    ui->label->setPixmap(pixmap);

                    condicion = 0;

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
