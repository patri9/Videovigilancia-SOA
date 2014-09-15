#ifndef C_ASINCRONO_H
#define C_ASINCRONO_H

#include "ui_viewerwindow.h"
#include <QObject>
#include <QDebug>
#include <QVector>
#include <QTcpSocket>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QDir>

class C_Asincrono : public QObject
{
    Q_OBJECT
public:
    explicit C_Asincrono(QObject *parent = 0, QTcpSocket *clientSocket = 0);

    quint32 size_cab;
    quint32 size_pro;
    quint32 tam_cabecera;
    quint32 tam_nombre_camara;
    qint64 timestamp;
    qint32 tam;
    qint32 num_rect;
    qint32 sizeCabecera;
    qint32 TotalsizeCabecera;
    qint32 condicion;
    bool flag;

    qint32 cont;
    QString hex;
    QDir directorio;

    QTcpSocket *clientConnection;

    QImage *imagen;
    QByteArray image;

signals:

public slots:
    void readData();
    void FalloConexion();

};

#endif // C_ASINCRONO_H
