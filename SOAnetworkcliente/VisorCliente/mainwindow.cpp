#include "mainwindow.h"
#include "ui_mainwindow.h"

//NUEVO: networkSession(0)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    networkSession(0),
    video(NULL),
    camara(NULL),
    vista_capt_propia(NULL),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->chk_reproducir->setChecked(conf.value("Editor/chk_reproducir").toBool());

    //NUEVO: creación de señales tcpsocket
    tcpSocket = new QTcpSocket(this);

    //connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(/*Aqui la funcion a la que se conecta()*/));

}
MainWindow::~MainWindow()
{

    if (video->isValid())
     {

        delete video;
        video = NULL;
     }

     delete ui;
     delete vista_capt_propia;
     delete camara;
}
void MainWindow::on_Boton_Cerrar_clicked()
{
    qApp->quit();

}
void MainWindow::mostrar_marcos(const QImage &imagen)
{
      //QPixmap pixmap = video->currentPixmap();
      QPixmap pixmap;

      //p.begin(&imagen);
      //p.setPen(QPen(QColor(QT::color2)));
      //p.drawRect(QRect(0,0,5,5));


      //Aquí, en el slot que pinta los pixmap, hay que mandar al servidor las imagenes

      pixmap.convertFromImage(imagen);

      //Le enviamos al servidor los pixmap, pero hay que convertirlos aquí. Pero como? Pasando primero tamaño y luego bytes


      QBuffer buffer;
      pixmap.save(&buffer, "jpeg");         //El pixmap lo guarda en el buffer con formato jpeg
      QByteArray bytes = buffer.buffer();   //Lo que hay en el buffer lo guarda como QByteArray


      int tam;
      tam = bytes.size();

      tcpSocket->write((char*)&tam, sizeof(tam));
      tcpSocket->write(bytes);

      ui->label->setPixmap(pixmap);
     // tcpSocket->write("Holaa");


}
void MainWindow::on_actionAbrir_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open File"), QDir::currentPath(),"Imagenes-Videos (*.mjpeg *.png *.xpm *.jpg)");
       if (!fileName.isEmpty())
       {
           //QImage image(fileName);
           video = new QMovie(fileName);
               if (video->isValid())
               {
                   ui->label->setScaledContents(true);
                  // ui->label->setMovie(video);

                   // Conectamos la señal de actualización de un qmovie con la ranura qmovie que se encarga de mostrar los marcos a través de los refrescos de la ventana principal
                   connect(video, SIGNAL(updated(const QRect&)),this, SLOT(mostrar_marcos(const QRect&)));
                   if(ui->chk_reproducir->isChecked())
                   {
                        video->start();
                        ui->Parar->setEnabled(true);
                        ui->Pausa->setEnabled(true);

                   }
                   else
                   {
                        ui->Reproducir->setEnabled(true);
                   }
               }

              /* if (image.isNull())
               {
                   QMessageBox::information(this, tr("Visor de Imagenes"),tr("No se ha podido cargar %1.").arg(fileName));
                   return;
               }
               ui->label->setScaledContents(true);
               ui->label->setPixmap(QPixmap::fromImage(image));
               */
           // Se podria usar un objeto de la clase imagereader que nos permite saber los errores de la codificación de la imagen
       }

       /* Delay para producir una espera

       QWaitCondition wc;
       QMutex mutex;
       // Los 5000 ms son el tiempo máximo que el hilo tiene que esperar hasta que otro hilo lo despierte
       wc.wait(&mutex,5000);

       */


}

void MainWindow::on_actionSalir_triggered()
{
     qApp->quit();
}

void MainWindow::on_Reproducir_clicked()
{

    if(video->isValid())
    {
        video->start();
        ui->Reproducir->setEnabled(false);
        ui->Parar->setEnabled(true);
        ui->Pausa->setEnabled(true);
    }
}

void MainWindow::on_Parar_clicked()
{
    if(video->isValid())
    {
        video->stop();
        ui->Reproducir->setEnabled(true);
        ui->Parar->setEnabled(false);
        ui->Pausa->setEnabled(false);
     }
}

void MainWindow::on_Pausa_clicked()
{
    if(video->isValid())
    {
        video->setPaused(true);
        ui->Reproducir->setEnabled(true);
        ui->Parar->setEnabled(false);
        ui->Pausa->setEnabled(false);
    }
}


void MainWindow::on_chk_reproducir_stateChanged()
{

        conf.setValue("Editor/chk_reproducir",ui->chk_reproducir->checkState());
}
void MainWindow::on_actionAcerca_de_Visor_triggered()
{

   Ayuda  form;
   form.exec();

}
void MainWindow::on_actionPreferencias_triggered()
{
    Preferencias_Camara form;
    form.exec();
}

void MainWindow::on_actionCapturar_triggered()
{


    if(camara==NULL && vista_capt_propia==NULL )
    {
        QList<QByteArray> dispositivos = QCamera::availableDevices();
        int ind=0;
        for (int i=0;i< dispositivos.size();i++)
        {
            qDebug() << conf.value("Editor/combo_disp").toString();
            if(conf.value("Editor/combo_disp").toString() == QCamera::deviceDescription(dispositivos[i]))
                ind = i;
        }

        camara = new QCamera(dispositivos[ind]);
        //vista_capt_propia = new Buffer_captura();
         vista_capt_propia = new vista_capturador;
    }
    if(camara->state()!=QCamera::ActiveState)
    {
            camara->setViewfinder(vista_capt_propia);

            //Crear un socket QTcpSocket (en el constructor)    OK
            //El puerto y el servidor que se pasan al connecttohost() habrá que sacarlos del Qsettings   OK
            //Despues hay que hacer un waitfor() para que espere a que la conexion se haga

            connect(vista_capt_propia, SIGNAL(mostrar_marco(const QImage&)),this, SLOT(mostrar_marcos(const QImage&)));

            tcpSocket->abort();
            tcpSocket->connectToHost(conf.value("Editor/servidor_pred","127.0.0.1").toString(), conf.value("Editor/puerto_pred",15000).toInt());

            //Una forma de guardar la direccion y puerto predeterminados
            //conf.setValue("Editor/servidor_pred","127.0.0.1");
            //conf.setValue("Editor/puerto_pred",15000);

            qDebug()<<(conf.value("Editor/servidor_pred").toString());
            qDebug()<<(conf.value("Editor/puerto_pred").toString());


          /*  if (tcpSocket->waitForConnected(1000))
            {
                qDebug("Conexión establecida!");
            }
*/
            connect(tcpSocket, SIGNAL(connected()), this, SLOT(rep_connected()));

            //camara->setViewfinder(vista_captura);
            //vista_captura->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
            //setCentralWidget(vista_captura);
            //camara->setCaptureMode(QCamera::CaptureViewfinder);
            camara->start();
    }
    else
     {
            if (camara->state()==QCamera::ActiveState)
            {
                camara->stop();
            }

     }

}

void MainWindow::rep_connected()
{

     camara->start();
}
