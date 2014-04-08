#ifndef VISTA_CAPTURADOR_H
#define VISTA_CAPTURADOR_H

#include <QAbstractVideoSurface>

class vista_capturador : public QAbstractVideoSurface
{
   Q_OBJECT

signals:
   void mostrar_marco(QImage imagen);
public:
    vista_capturador(QObject * parent = 0);
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType =
                QAbstractVideoBuffer::NoHandle) const;
     bool present(const QVideoFrame &);
     ~vista_capturador();
};

#endif // VISTA_CAPTURADOR_H
