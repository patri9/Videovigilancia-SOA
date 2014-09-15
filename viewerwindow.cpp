#include "viewerwindow.h"
#include "c_asincrono.h"

int ViewerWindow::sigHupSd[2];
int ViewerWindow::sigTermSd[2];

ViewerWindow::ViewerWindow(QObject *parent) :
    QObject(parent),
    viewfinder(NULL),
    captureBuffer(NULL),
    imagen(NULL),
    tcpServer(NULL),
    networkSession(0),
    clientConnection(NULL)
{
    tam = 0;
    condicion = 0;
    flag_port = 0;
    cont=0;
    hex="%1";
    imagen = new QImage();

    tcpServer = new QTcpServer(this);

    captureBuffer = new CaptureBuffer;
    //connect(captureBuffer, SIGNAL(senial(const QImage&)), this, SLOT(mostrar(const QImage&)));
    on_actionCaptura_de_red_triggered();

    QSettings conf1(APP_CONFFILE, QSettings::IniFormat);
    //ui->reproducir->setChecked(conf1.value("Reproducir", false).toBool());

    qRegisterMetaType< QImage >("QImage");
    qRegisterMetaType< QVector<QRect> > ("QVector<QRect>");

    /*
     * Config con QSettings
     */
    QSettings config("config", QSettings::IniFormat);
    Port = config.value("Puerto ", "").toInt();
    Host = config.value("IP ", "").toString();
    device = config.value("Nombre ", "").toString();
    intervalo = config.value("Reconectar ", "").toInt();
    intervalo = 1000;
    Dir = config.value("Directorio ",APP_VARDIR).toString();

    // Cambia el usuario
    setuid(config.value("uid",0).toInt());
    // Cambia el grupo
    setgid(config.value("gid",0).toInt());
    // Cambia la umask
    bool msk;
    umask(config.value("umask",0).toString().toUInt(&msk,8));
    /*
     * Fin config
     */

    /*
     * SEÑALES POSIX
     */
    // Crear las parejas de sockets UNIX
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigHupSd))
        qFatal("Couldn't create HUP socketpair");
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigTermSd))
        qFatal("Couldn't create TERM socketpair");

    // Crear los objetos para monitorizar uno de los socket
    // de cada pareja.
    sigHupNotifier = new QSocketNotifier(sigHupSd[1],
            QSocketNotifier::Read, this);
    sigTermNotifier = new QSocketNotifier(sigTermSd[1],
            QSocketNotifier::Read, this);

    // Conectar la señal activated() de cada objeto
    // QSocketNotifier con el slot correspondiente. Esta señal
    // será emitida cuando hayan datos para ser leidos en el
    // socket monitorizado.
    connect(sigHupNotifier, SIGNAL(activated(int)), this,
            SLOT(handleSigHup()));
    connect(sigTermNotifier, SIGNAL(activated(int)), this,
            SLOT(handleSigTerm()));
      /*
       *  SEÑALES POSIX
       */
 }


ViewerWindow::~ViewerWindow()
{
    delete captureBuffer;
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


void ViewerWindow::send()
{
    while(tcpServer->hasPendingConnections())
    {
        qDebug() << "Nueva conexión. ";

        clientConnection = tcpServer->nextPendingConnection();

        C_Asincrono *paqAsinc = new C_Asincrono(this,clientConnection);
        connect(clientConnection,SIGNAL(readyRead()),paqAsinc, SLOT(readData())); //Enviar al slot de c_asincrono
        connect(clientConnection,SIGNAL(error(QAbstractSocket::SocketError)()),paqAsinc, SLOT(FalloConexion())); //Enviar al slot de c_asincrono
    }
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
                    clientConnection->read((char*)&size_cab,sizeof(size_cab));
                    clientConnection->read((char*)&size_pro,sizeof(size_pro));
                    clientConnection->read((char*)&tam_nombre_camara,sizeof(tam_nombre_camara));
                    clientConnection->read((char*)&timestamp,sizeof(timestamp));

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
                    image = clientConnection->read(tam);
                    //imagen->loadFromData(image, "jpeg");
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

                    condicion = 6;
                }
                else
                {
                    condicion = 5;
                    flag = false;
                    break;
                }

            case 6:
                //Almacenamineto de imágenes en disco duro
                hex = QString("%1").arg(cont, 32, 16, QLatin1Char('0'));
                QString Dir1 = hex.left(4);
                QString Dir2 = hex.mid(4, 4);
                QString nombre = hex;
                QString ruta = QString(APP_VARDIR) + "/" + Dir1 + "/" + Dir2 + "/";
                directorio.mkpath(ruta);
                QFile imag(ruta+nombre+".jpg");
                if(imag.isOpen())
                {
                    imag.write(image);
                }

                cont++;

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
        tcpServer->errorString();

        return;
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(send()));
}


//
// Manejador de la señal SIGHUP
//
void ViewerWindow::hupSignalHandler(int)
{
    char a = 1;
    ::write(sigHupSd[0], &a, sizeof(a));
}

//
// Manejador de la señal SIGTERM
//
void ViewerWindow::termSignalHandler(int)
{
    char a = 1;
    ::write(sigTermSd[0], &a, sizeof(a));
}

//
// Configurar los manejadores de señal
//
int setupUnixSignalHandlers()
{
    struct ::sigaction hup, term;

    // Señal SIGHUP //
    hup.sa_handler = &ViewerWindow::hupSignalHandler;
    // Vaciamos la máscara para indicar que no queremos bloquear
    // la llegada de ninguna señal POSIX.
    sigemptyset(&hup.sa_mask);
    // SA_RESTART indica que si la señal interrumpe alguna
    // llamada al sistema lanzada desde otro punto del programa,
    // al volver del manejador la llamada al sistema debe
    // continuar. En caso contrario dicha llamada retornará
    // indicando un error.
    hup.sa_flags = SA_RESTART;

    // Establecer manejador de la señal SIGHUP
    if (::sigaction(SIGHUP, &hup, 0) > 0)
    return 1;

     // Señal SIGTERM //
    term.sa_handler = &ViewerWindow::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags = SA_RESTART;

    // Establecer manejador de la señal SIGTERM
    if (::sigaction(SIGTERM, &term, 0) > 0)
    return 2;

    return 0;
}

void ViewerWindow::handleSigHup()
{
    // Desactivar la monitorización para que por el momento no
    // lleguen más señales de Qt
    sigHupNotifier->setEnabled(false);
    // Leer y desechar el byte enviado
    char tmp;
    ::read(sigHupSd[1], &tmp, sizeof(tmp));

    qApp->quit();
    qDebug() << " //***** Señal SigHup *****// " ;
    qDebug() << " " ;
    //deleteLater();

    //Configuración del socket
    //QSettings config(APP_CONFFILE, QSettings::IniFormat);
    QSettings config("Config", QSettings::IniFormat);
    Port = config.value("Puerto ", "").toInt();
    Host = config.value("IP ", "").toString();
    device = config.value("Nombre ", "").toString();
    intervalo = config.value("Reconectar ", "").toInt();
    intervalo = 1000;
    Dir = config.value("Directorio ",APP_VARDIR).toString();

    // Activar la monitorización para que vuelvan a llegar
    // señales de Qt
    sigHupNotifier->setEnabled(true);
}

void ViewerWindow::handleSigTerm()
{
    // Desactivar la monitorización para que por el momento no
    // lleguen más señales de Qt
    sigTermNotifier->setEnabled(false);
    // Leer y desechar el byte enviado
    char tmp;
    ::read(sigTermSd[1], &tmp, sizeof(tmp));

    qApp->quit();
    qDebug() << " //***** Señal SigTerm *****//" ;
    qDebug() << " " ;
    deleteLater();

    // Activar la monitorización para que vuelvan a llegar
    // señales de Qt
    sigTermNotifier->setEnabled(true);
}
