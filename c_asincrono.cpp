#include "c_asincrono.h"


C_Asincrono::C_Asincrono(QObject *parent, QTcpSocket *clientSocket, QLabel *label) :
    QObject(parent),
    clientConnection(clientSocket),
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
    qDebug() << "Error de conexión: " << clientConnection->errorString();
    clientConnection->disconnect();
    clientConnection->deleteLater();
}

void C_Asincrono::readData()
{

    flag = true;
    qint32 TotalsizeRect;
    qint32 x,y,ancho,alto;
    QVector<QRect> vRect;
    qDebug() << "Entra a la clase de c_asincrono";



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
                    qDebug() << "frame: " << frame;
                    qDebug() << "pixmap: " << pixmap;
                    qDebug() << "antes de mostrar";
                    qlabel->setPixmap(pixmap);
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

                    condicion = 0;

            }

    }
}
