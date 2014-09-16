#include "c_asincrono.h"

//Aquí hay que sustituir el socket por un socketdescriptor, una semilla para tomar depués el socket
C_Asincrono::C_Asincrono(QObject *parent, qintptr socketDescriptor, QLabel *label) :
    QThread(parent),
    socketDescriptor(socketDescriptor),
    qlabel(label)

{


    tam = 0;
    condicion = 0;
    cont=0;
    hex="%1";

    imagen = new QImage();


}

void C_Asincrono::FalloConexion()
{
    qDebug() << "Error de conexión: " << clientConnection.errorString();
    clientConnection.disconnect();
    clientConnection.deleteLater();
}

void C_Asincrono::run()
{

     QTcpSocket clientConnection;
     qDebug() << "Entra en el run";

    if (!(clientConnection.setSocketDescriptor(socketDescriptor)))
    {
        qDebug() << "Emite FalloConexion";
        emit FalloConexion();
        qDebug() << "Retorna al no encontrar socketdescriptor";
        return;
    }

    qDebug() << clientConnection.socketDescriptor();
    qDebug() << "Sale sin problemas del if";

    flag = true;
    qint32 TotalsizeRect;
    qint32 x,y,ancho,alto;
    QVector<QRect> vRect;
    qDebug() << "Entra a la clase de c_asincrono";


    while(flag)
    {

                sizeCabecera = sizeof(tam_cabecera);
                while(sizeCabecera > clientConnection.bytesAvailable())
                {
                    clientConnection.waitForReadyRead();
                }
                clientConnection.read((char*)&tam_cabecera,sizeof(tam_cabecera));
                qDebug() << "Tamaño Cabecera Antes: " << tam_cabecera;

                TotalsizeCabecera = tam_cabecera;

                while(TotalsizeCabecera >= clientConnection.bytesAvailable())
                {
                    clientConnection.waitForReadyRead();
                }


                qDebug() << "Tamaño Cabecera Despues: " << tam_cabecera;
                clientConnection.read((char*)&size_cab,sizeof(size_cab));
                QByteArray cabecera  = clientConnection.read(size_cab);
                qDebug() << "Cabecera: " << cabecera;
                clientConnection.read((char*)&size_pro,sizeof(size_pro));
                QByteArray proto = clientConnection.read(size_pro);
                qDebug() << "Protocolo: " << proto;
                clientConnection.read((char*)&tam_nombre_camara,sizeof(tam_nombre_camara));
                QByteArray nombre_camara  = clientConnection.read(tam_nombre_camara);
                qDebug() << "Nombre Cámara: " << nombre_camara;
                clientConnection.read((char*)&timestamp,sizeof(timestamp));

                qDebug() << "Timestamp: " << timestamp << " segundos";
                qDebug() << "";

                while(sizeof(tam) >= clientConnection.bytesAvailable())
                {
                    clientConnection.waitForReadyRead();
                }


                clientConnection.read((char*)&tam,4);
                qDebug() << "Tamaño Imagen: " << tam;

                while(tam >= clientConnection.bytesAvailable())
                {
                    clientConnection.waitForReadyRead();
                }


                QByteArray imag = clientConnection.read(tam);
                imagen->loadFromData(imag, "jpeg");

                while(sizeof(num_rect) >= clientConnection.bytesAvailable())
                {
                    clientConnection.waitForReadyRead();
                }


                clientConnection.read((char*)&num_rect, sizeof(num_rect)); //Almaceno aqui el numero de rectangulos para el sig checkpoint
                qDebug() << "Numero de rectangulos : " << num_rect;

                TotalsizeRect = sizeof(x)*4*num_rect;

                while(TotalsizeRect >= clientConnection.bytesAvailable())
                {

                    clientConnection.waitForReadyRead();
                }


                for(int i=0; i<num_rect; i++)
                {
                    clientConnection.read((char*)&x,sizeof(x));;
                    clientConnection.read((char*)&y,sizeof(y));
                    clientConnection.read((char*)&ancho,sizeof(ancho));
                    clientConnection.read((char*)&alto,sizeof(alto));

                    QRect rect(x,y,ancho,alto);
                    vRect.append(rect);
                }


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
                qDebug() << "frame: " << frame;
                qDebug() << "pixmap: " << pixmap;
                qDebug() << "antes de mostrar";
                emit senial_img(frame);
                qDebug() << "Despues de mostrar";


                //Almacenamineto de imágenes en disco duro
                QImage image = *imagen;
                hex = QString("%1").arg(cont, 32, 16, QLatin1Char('0'));
                qDebug() << hex;
                QString Dir1 = hex.left(4);
                QString Dir2 = hex.mid(4, 4);
                QString nombre = hex;
                QString ruta = QString(APP_VARDIR) + "/" + Dir1 + "/" + Dir2 + "/";
                directorio.mkpath(ruta);
                image.save(ruta+"/"+nombre+".jpg");
                cont++;


    }
}
