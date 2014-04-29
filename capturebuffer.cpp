#include "capturebuffer.h"

CaptureBuffer::CaptureBuffer()
{
}

CaptureBuffer::~CaptureBuffer()
{
}


QList<QVideoFrame::PixelFormat> CaptureBuffer::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const
{
    // A través de este método nos preguntan que formatos de vídeo soportamos. Como vamos a guardar los frames en
    // objetos QImage nos sirve cualquiera de los formatos sorportados por dicha clase: http://kcy.me/z6pa
    QList<QVideoFrame::PixelFormat> formats;
    formats << QVideoFrame::Format_ARGB32;
    formats << QVideoFrame::Format_ARGB32_Premultiplied;
    formats << QVideoFrame::Format_RGB32;
    formats << QVideoFrame::Format_RGB24;
    formats << QVideoFrame::Format_RGB565;
    formats << QVideoFrame::Format_RGB555;
    return formats;
}


bool CaptureBuffer::present(const QVideoFrame &frame)
{
    QVideoFrame framecp;
    framecp = frame;

    framecp.map(QAbstractVideoBuffer::ReadOnly);
    QImage frameAsImage = QImage(framecp.bits(), framecp.width(),
        framecp.height(), framecp.bytesPerLine(),
        QVideoFrame::imageFormatFromPixelFormat(framecp.pixelFormat()));

    frameAsImage = frameAsImage.convertToFormat(QImage::Format_RGB32, Qt::ColorOnly); // !!
    //Emitir la señal al SLOT de ViewerWindow
    emit senial(frameAsImage);

    framecp.unmap();

    // A través de este método nos darán el frame para que lo mostremos.
    return true;
}

