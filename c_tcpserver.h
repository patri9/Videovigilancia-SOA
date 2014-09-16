#ifndef C_TCPSERVER_H
#define C_TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "c_asincrono.h"


class C_TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit C_TcpServer(QObject *parent = 0);
   // Ui::ViewerWindow *ui;

protected:
     void incomingConnection(qintptr socketDescriptor);

signals:

public slots:

};

#endif // C_TCPSERVER_H
