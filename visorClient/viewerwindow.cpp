#include "viewerwindow.h"


ViewerWindow::ViewerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ViewerWindow),
    movie_(NULL),
    camera(NULL),
    viewfinder(NULL),
    captureBuffer(NULL),
    imagen(NULL),
    tcpSocket(NULL),
    networkSession(0)
{
    ui->setupUi(this);

    movie_ = new QMovie();
    ui->label->setMovie(movie_);

    tcpSocket = new QTcpSocket(this);

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

    intervalo = 1000;
    //Configuración del socket
    Port = conf.value("Puerto ", "").toInt();
    Host = conf.value("IP ", "").toString();
    device = conf.value("Nombre ", "").toString();
    intervalo = conf.value("Reconectar ", "").toInt();

    timer.setInterval(intervalo);
    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorsocket()));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(reconnect()));
    timer.start();

}


ViewerWindow::~ViewerWindow()
{
    delete ui;
    delete movie_;
    delete captureBuffer;
    delete camera;
    delete viewfinder;
    delete imagen;
    delete tcpSocket;
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

      QBuffer buffer;
      pixmap.save(&buffer, "jpeg");
      QByteArray bytes = buffer.buffer();

      qint32 tam;  //Entero de 4bytes
      qToLittleEndian(tam);

      /*
       CABECERA
       */
      const char* cabecera = "Recibiendo cabecera de grupo01";
      conf.setValue("Cabecera", cabecera);

      //Nombre de protocolo
      QString proto = "CAM1";
      conf.setValue("Protocolo", proto);

      //Timestamp= número de segundos desde el 1 de enero de 1970.
      quint64 timestamp = QDateTime::currentMSecsSinceEpoch();
      timestamp = timestamp/1000;
      conf.setValue("Time", timestamp);

      //Nombre de la cámara. Identifica a este cliente.
      QString nombre_camara = "Camara usu1";
      conf.setValue("CurrentCam", nombre_camara);


      if (tcpSocket->state() == QAbstractSocket::ConnectedState)
      {
          tam = bytes.size();
          tcpSocket->write((char*)&tam,sizeof(tam));
          qDebug() << "Tamaño del paquete " << tam;
          tcpSocket->write(bytes);
          qDebug() << "Mensaje enviado.";
          qDebug() << "Usuario Actual " << conf.value("CurrentCam").toString();
          qDebug() << "Timestamp " << conf.value("Time").toString();
      }
      else
      {
          qDebug() << "No se ha podido enviar el mensaje. ";
      }
}


void ViewerWindow::errorsocket()
{
  qDebug() << "Error de conexión del socket: " << tcpSocket->errorString();
  tcpSocket->close();
}


void ViewerWindow::reconnect()
{
    if(tcpSocket->state() == QAbstractSocket::UnconnectedState)
    {
        tcpSocket->connectToHost(conf.value("Host ","127.0.0.1").toString(), conf.value("Port ",15000).toInt());
        qDebug() << "Reconectando...";
    }
    timer.start();
}


void ViewerWindow::repro()
{
    camera->start();
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

            tcpSocket->abort();
            tcpSocket->connectToHost(conf.value("Host ","127.0.0.1").toString(), conf.value("Port ",15000).toInt());

            //Iniciar la reproducción sólo cuando la conexión se haya realizado, evitando el uso de waitFor* y el bloqueo del hilo principal.
            connect(tcpSocket, SIGNAL(connected()), this, SLOT(repro()));

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
