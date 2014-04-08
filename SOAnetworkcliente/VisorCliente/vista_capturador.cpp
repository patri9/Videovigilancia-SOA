#include "vista_capturador.h"

vista_capturador::vista_capturador(QObject * parent):QAbstractVideoSurface(parent)
{
}

QList<QVideoFrame::PixelFormat> vista_capturador::supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType) const
    {
        // A través de este método nos preguntan que formatos de
        // vídeo soportamos. Como vamos a guardar los frames en
        // objetos QImage nos sirve cualquiera de los formatos
        // sorportados por dicha clase: http://kcy.me/z6pa
        QList<QVideoFrame::PixelFormat> formats;
        formats << QVideoFrame::Format_ARGB32;
        formats << QVideoFrame::Format_ARGB32_Premultiplied;
        formats << QVideoFrame::Format_RGB32;
        formats << QVideoFrame::Format_RGB24;
        formats << QVideoFrame::Format_RGB565;
        formats << QVideoFrame::Format_RGB555;
        return formats;
    }

bool vista_capturador::present(const QVideoFrame &frame)
    {
        // A través de este método nos darán el frame para que
        // lo mostremos.
    QVideoFrame a = frame;
    a.map(QAbstractVideoBuffer::ReadOnly);
    QImage frameAsImage = QImage(frame.bits(), frame.width(),
        frame.height(), frame.bytesPerLine(),
        QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));

    // Aquí el código que manipula frameAsImage...

    a.unmap();
    emit mostrar_marco(frameAsImage);
        return true;
    }
vista_capturador::~vista_capturador(){}
