#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    video(NULL),
    camara(NULL),
    vista_capt_propia(NULL)
{

    ui->setupUi(this);
    ui->chk_reproducir->setChecked(conf.value("Editor/chk_reproducir").toBool());
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

      pixmap.convertFromImage(imagen);
      ui->label->setPixmap(pixmap);



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
            qDebug() << conf.value("Editor/combo_disp");
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
            connect(vista_capt_propia, SIGNAL(mostrar_marco(const QImage&)),this, SLOT(mostrar_marcos(const QImage&)));
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

void MainWindow::on_actionCaptura_de_Red_triggered()
{
    //Aquí debajo guardamos el puerto a la que se conectará el socket del servidor
    int port;
    port = conf.value("Editor/puerto_pred").toInt();
    tcpServer = new QTcpServer(this);
        if (!tcpServer->listen(QHostAddress::Any, port)) //El puerto lo saca de conf.value("Editor/puerto_pred").toInt);
        {
            QMessageBox::critical(this, tr("Servidor Camara"),
                                  tr("Error al iniciar el servidor: %1.")
                                  .arg(tcpServer->errorString()));
            close();
            return;
        }

        connect(tcpServer, SIGNAL(newConnection()), this, SLOT(send()));



}

void MainWindow::send()
{

    //Ahora creamos la conexión con el cliente
    clientconnection = tcpServer->nextPendingConnection();
    connect(clientconnection, SIGNAL(readyRead()), this , SLOT(readData()));

}

void MainWindow::readData()
{

    int tam;
    clientconnection->read((char*)&tam,4);
    QByteArray image = clientconnection->read(tam);
    QImage imagen; imagen.loadFromData(image, "jpeg");

    //Copia de la imagen.
    QImage frame = imagen;

    //Se convierte de QImage a QPixmap y se muestra.
    QPixmap pixmap;
    pixmap.convertFromImage(frame);
    ui->label->setPixmap(pixmap);

}
