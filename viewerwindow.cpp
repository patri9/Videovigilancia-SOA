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
    connect(captureBuffer, SIGNAL(senial(const QImage&)), this, SLOT(mostrar(const QImage &)));

    //Desactivar los botones al iniciar la aplicación
    ui->playbutton->setDisabled(true);
    ui->stopbutton->setDisabled(true);
    ui->pause->setDisabled(true);
    connect(ui->playbutton, SIGNAL(clicked()), movie_, SLOT(start()));
    connect(ui->stopbutton, SIGNAL(clicked()), movie_, SLOT(stop()));

    QSettings conf1;
    ui->reproducir->setChecked(conf1.value("Reproducir", false).toBool());

    //connect(movie_, SIGNAL(updated(const QRect&)), this, SLOT(on_movie_updated(const QRect&)));

    //qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    qRegisterMetaType< QImage >("QImage");
    // Registra QVector<QRect> como tipo en qt para que sea reconocido
    qRegisterMetaType< QVector<QRect> >("QVector<QRect>");

    connect(movie_, SIGNAL(updated(QRect)), this, SLOT(on_movie_updated(QRect)));
    connect(this, SIGNAL(PasarImagen(const QImage &)), &Worker_, SLOT(doWork(const QImage &)));
    connect(& Worker_, SIGNAL(ImageWorked(const QImage&, QVector<QRect>)), this, SLOT(mostrar(const QImage&, QVector<QRect>)));

    Worker_.moveToThread(&workingThread_);
    //Iniciar hilo
    workingThread_.start();


    //Configuración del socket
    Port = conf.value("Puerto ", "").toInt();
    Host = conf.value("IP ", "").toString();
    device = conf.value("Nombre ", "").toString();
    intervalo = conf.value("Reconectar ", "").toInt();
    intervalo = 1000;

    timer.setInterval(intervalo);
    QObject::connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorsocket()));
    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(reconnect()));
    timer.start(); 

}


ViewerWindow::~ViewerWindow()
{
    //Detencion del bucle de mensaje del hilo
    workingThread_.quit();
    Worker_.parar = true;
    //Esperar a que el hilo de trabajo termine
    workingThread_.wait();

    delete ui;
    delete movie_;
    delete captureBuffer;
    delete camera;
    delete viewfinder;
    delete imagen;
    delete tcpSocket;
    delete networkSession;

}


void ViewerWindow::workAsync(const QImage& imagen)
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    emit PasarImagen(imagen);
}


void ViewerWindow::ImageWorked(const QImage& imagen)
{
    ui->label->setPixmap(QPixmap::fromImage(imagen));
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
    else if(movie_->isValid())
    {
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


void ViewerWindow::on_movie_updated(const QRect&)
{
    QImage imagen = movie_->currentImage();
    QPixmap pixmap = movie_->currentPixmap();
    ui->label->setPixmap(pixmap);

    emit PasarImagen(imagen);
}


//SLOT mostrar
void ViewerWindow::mostrar(const QImage &frame, QVector<QRect> vRect)
{
    //Se crea una copia de la imagen.
    QImage img = frame;

    //Se convierte de QImage a QPixmap y se muestra.
    QPixmap pixmap;
    pixmap.convertFromImage(img);
    ui->label->setPixmap(pixmap);

    /*
     *  CABECERA
     */
    //Cabecera
    QByteArray cabecera = "Grupo01";
    quint32 size_cab = cabecera.size();
    //Nombre protocolo
    QByteArray proto = "CAM1";
    quint32 size_pro = proto.size();
    //Nombre de la cámara. Identifica a este cliente.
    QByteArray nombre_camara = "Camara usu1";
    quint32 size = nombre_camara.size();
    //Timestamp= número de segundos desde el 1 de enero de 1970.
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    timestamp = timestamp/1000;
    //Tamaño total cabecera
    quint32 tam_cabecera= (size_cab+size_pro+size+sizeof(timestamp));

    QBuffer buffer;
    pixmap.save(&buffer, "jpeg");
    QByteArray bytes = buffer.buffer();

    qint32 tam = 0;  //Entero de 4bytes
    qToLittleEndian(tam);

    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        tam = bytes.size();
        tcpSocket->write((char*)&tam,sizeof(tam));
        qDebug() << "Tamaño del paquete " << tam;
        tcpSocket->write(bytes);
        qDebug() << "Mensaje enviado.";
        qDebug() << "Cabecera: " << cabecera;
        qDebug() << "Protocolo: " << proto;
        qDebug() << "Usuario Actual " << nombre_camara;
        qDebug() << "Timestamp " << timestamp;
        qDebug() << "";

        qint32 x,y,ancho,alto;
        qint32 num_rect = vRect.size();
        tcpSocket->write((char*)&num_rect,sizeof(num_rect));
        for(int i=0; i<vRect.size(); i++)
        {
            x=vRect[i].x();
            tcpSocket->write((char*)&x,sizeof(x));
            y=vRect[i].y();
            tcpSocket->write((char*)&y,sizeof(y));
            ancho=vRect[i].width();
            tcpSocket->write((char*)&ancho,sizeof(ancho));
            alto=vRect[i].height();
            tcpSocket->write((char*)&alto,sizeof(alto));
        }
        tcpSocket->write((char*)&tam_cabecera,sizeof(tam_cabecera));
        tcpSocket->write((char*)&size_cab,sizeof(size_cab));
        tcpSocket->write(cabecera);
        tcpSocket->write((char*)&size_pro,sizeof(size_pro));
        tcpSocket->write(proto);
        tcpSocket->write((char*)&size,sizeof(size));
        tcpSocket->write(nombre_camara);
        tcpSocket->write((const char*)&timestamp,sizeof(timestamp));
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
            connect(captureBuffer, SIGNAL(senial(const QImage&)), &Worker_, SLOT(doWork(const QImage&)));

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
