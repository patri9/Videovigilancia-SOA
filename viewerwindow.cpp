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

    //Configuracion socket

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
    qDebug() << conf.value("Cabecera").toString();

    clientConnection = tcpServer->nextPendingConnection();

    connect(clientConnection,SIGNAL(readyRead()),this, SLOT(readData()));
    connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(FalloConexion()));
}


void ViewerWindow::readData()
{
    int tam;
    clientConnection->read((char*)&tam,4);

    QByteArray image = clientConnection->read(tam);

    QImage imagen;
    imagen.loadFromData(image, "jpeg");

    //Copia de la imagen.
    QImage frame = imagen;

    //Se convierte de QImage a QPixmap y se muestra.
    QPixmap pixmap;
    pixmap.convertFromImage(frame);
    ui->label->setPixmap(pixmap);

    /*
     *  CABECERA
     */
    qDebug() << "Protocolo: " << conf.value("Protocolo").toString();
    qDebug() << "Timestamp: " << conf.value("Time").toString() << " segundos";
    qDebug() << "Nombre Cámara: " << conf.value("CurrentCam").toString();
    qDebug() << "";

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
