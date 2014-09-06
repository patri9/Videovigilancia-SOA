#include "viewerwindow.h"

int ViewerWindow::sigInt[2];
int ViewerWindow::sigHup[2];

ViewerWindow::ViewerWindow(QObject *parent):
    QObject(parent),
    //movie_(NULL),
    camera(NULL),
    viewfinder(NULL),
    captureBuffer(NULL),
    imagen(NULL),
    tcpSocket(NULL),
    networkSession(0)
{

    tcpSocket = new QTcpSocket(this);

    captureBuffer = new CaptureBuffer;
    connect(captureBuffer, SIGNAL(senial(const QImage&)), this, SLOT(mostrar(const QImage&)));

    Capturar();

    QSettings conf(APP_CONFFILE, QSettings::IniFormat);

    connect(movie_, SIGNAL(updated(const QRect&)), this, SLOT(on_movie_updated(const QRect&)));

    qRegisterMetaType< QImage >("QImage");
    // Registra QVector<QRect> como tipo en qt para que sea reconocido
    qRegisterMetaType< QVector<QRect> >("QVector<QRect>");

    //connect(this, SIGNAL(PasarImagen(const QImage &)), &Worker_, SLOT(doWork(const QImage &)));
    connect(& Worker_, SIGNAL(ImageWorked(const QImage&, QVector<QRect>)), this, SLOT(mostrar(const QImage&, QVector<QRect>)));

    Worker_.moveToThread(&workingThread_);
    //Iniciar hilo
    workingThread_.start();


    /*** SEÑALES ***/
    // Crear las parejas de sockets UNIX
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sigInt))
        qFatal("Couldn't create INT socketpair");
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sigHup))
        qFatal("Couldn't create HUP socketpair");

    // Crear los objetos para monitorizar uno de los socket
    // de cada pareja.
    sigIntNotifier = new QSocketNotifier(sigInt[1], QSocketNotifier::Read, this);
    sigHupNotifier = new QSocketNotifier(sigHup[1], QSocketNotifier::Read, this);

    // Conectar la señal activated() de cada objeto
    // QSocketNotifier con el slot correspondiente. Esta señal
    // será emitida cuando hayan datos para ser leidos en el
    // socket monitorizado.
    connect(sigIntNotifier, SIGNAL(activated(int)), this, SLOT(handleSigInt()));
    connect(sigHupNotifier, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    /*** FIN SEÑALES ***/



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

    //delete movie_;
    delete captureBuffer;
    delete camera;
    delete viewfinder;
    delete imagen;
    delete tcpSocket;
    delete networkSession;

    sigIntNotifier->deleteLater();
    sigHupNotifier->deleteLater();
}


void ViewerWindow::workAsync(const QImage& imagen)
{
    qDebug() << Q_FUNC_INFO << QThread::currentThreadId();

    emit PasarImagen(imagen);
}


//SLOT
void ViewerWindow::mostrar(const QImage &frame, QVector<QRect> vRect)
{
    //Se crea una copia de la imagen.
    QImage img = frame;

    //Se convierte de QImage a QPixmap y se muestra.
    QPixmap pixmap;
    pixmap.convertFromImage(img);
    //ui->label->setPixmap(pixmap);

    /*
     *  CABECERA
     */
    //Cabecera
    QByteArray cabecera = "Grupo01";
    qint32 size_cab = cabecera.size();
    //Nombre protocolo
    QByteArray proto = "CAM1";
    qint32 size_pro = proto.size();
    //Nombre de la cámara. Identifica a este cliente.
    QByteArray nombre_camara = "Camara usu1";
    qint32 size = nombre_camara.size();
    //Timestamp= número de segundos desde el 1 de enero de 1970.
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
    timestamp = timestamp/1000;
    //Tamaño total cabecera
    qint32 tam_cabecera= (size_cab+size_pro+size+sizeof(timestamp));

    QBuffer buffer;
    pixmap.save(&buffer, "jpeg");
    QByteArray bytes = buffer.buffer();

    qint32 tam = 0;  //Entero de 4bytes
    qToLittleEndian(tam);

    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        tcpSocket->write((char*)&tam_cabecera,sizeof(tam_cabecera));
        tcpSocket->write((char*)&size_cab,sizeof(size_cab));
        tcpSocket->write(cabecera);
        tcpSocket->write((char*)&size_pro,sizeof(size_pro));
        tcpSocket->write(proto);
        tcpSocket->write((char*)&size,sizeof(size));
        tcpSocket->write(nombre_camara);
        tcpSocket->write((const char*)&timestamp,sizeof(timestamp));

        tam = bytes.size();
        tcpSocket->write((char*)&tam,sizeof(tam));
        qDebug() << "Tamaño de la imagen " << tam;
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
void ViewerWindow::Capturar()
{
    if (camera==NULL)
    {
        conf.setValue("Camera", "/dev/video0");
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


//
// Manejador de la señal SIGINT
//
void ViewerWindow::intSignalHandler(int)
{
    char a = 1;
    write(sigInt[0], &a, sizeof(a));
}

//
// Manejador de la señal SIGHUP
//
void ViewerWindow::hupSignalHandler(int)
{
    char a = 1;
    write(sigHup[0], &a, sizeof(a));
}


//
// Configurar los manejadores de señal
//
int setupUnixSignalHandlers()
{
    struct ::sigaction iint, hup;

    // Señal SIGINT //
    iint.sa_handler = &ViewerWindow::intSignalHandler;
    // Vaciamos la máscara para indicar que no queremos bloquear
    // la llegada de ninguna señal POSIX.
    sigemptyset(&iint.sa_mask);
    // SA_RESTART indica que si la señal interrumpe alguna
    // llamada al sistema lanzada desde otro punto del programa,
    // al volver del manejador la llamada al sistema debe
    // continuar. En caso contrario dicha llamada retornará
    // indicando un error.
    iint.sa_flags = SA_RESTART;

    // Establecer manejador de la señal SIGINT
    if (::sigaction(SIGINT, &iint, 0) > 0)
    return 1;


    // Señal SIGHUP //
    hup.sa_handler = &ViewerWindow::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = SA_RESTART;

    if (::sigaction(SIGHUP, &hup, 0) > 0)
    return 2;

    return 0;
}


void ViewerWindow::handleSigInt()
{
    // Desactivar la monitorización para que por el momento no
    // lleguen más señales de Qt
    sigIntNotifier->setEnabled(false);

    // Leer y desechar el byte enviado
    char tmp;
    ::read(sigInt[1], &tmp, sizeof(tmp));

    qApp->quit();
    qDebug() << " //***** Señal SigInt *****// " ;
    qDebug() << " " ;
    //deleteLater();

    // Activar la monitorización para que vuelvan a llegar
    // señales de Qt
    sigIntNotifier->setEnabled(true);
}


void ViewerWindow::handleSigHup()
{
    // Desactivar la monitorización para que por el momento no
    // lleguen más señales de Qt
    sigHupNotifier->setEnabled(false);

    // Leer y desechar el byte enviado
    char tmp;
    ::read(sigHup[1], &tmp, sizeof(tmp));

    qApp->quit();
    qDebug() << " //***** Señal SigHup *****//" ;
    qDebug() << " " ;
    //deleteLater();

    // Activar la monitorización para que vuelvan a llegar
    // señales de Qt
    sigHupNotifier->setEnabled(true);
}

