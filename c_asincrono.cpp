#include "c_asincrono.h"

C_Asincrono::C_Asincrono(QObject *parent, QTcpSocket *clientSocket) :
    QObject(parent),
    clientConnection(clientSocket)
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
                    QByteArray cabecera  = clientConnection->read(size_cab);
                    clientConnection->read((char*)&size_pro,sizeof(size_pro));
                    QByteArray proto = clientConnection->read(size_pro);
                    clientConnection->read((char*)&tam_nombre_camara,sizeof(tam_nombre_camara));
                    QByteArray nombre_camara  = clientConnection->read(tam_nombre_camara);
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
                    //image.save(ruta+"/"+nombre+".jpg");
                }

                cont++;

                condicion = 0;
            }
    }
}
