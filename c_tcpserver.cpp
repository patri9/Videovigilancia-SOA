#include "c_tcpserver.h"

C_TcpServer::C_TcpServer(QObject *parent) :
    QTcpServer(parent)

{

}

void C_TcpServer::incomingConnection(qintptr socketDescriptor)
{
        qDebug() << "Nueva conexión. ";

        C_Asincrono *paqAsinc = new C_Asincrono(this,socketDescriptor);
        connect(paqAsinc, SIGNAL(finished()), paqAsinc, SLOT(deleteLater()));
        connect(paqAsinc, SIGNAL(senial_img(const QImage&)), this->parent(), SLOT(recibir_Dibujo(const QImage&)));
        paqAsinc->start();
}
