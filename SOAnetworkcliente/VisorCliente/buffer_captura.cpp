#include "buffer_captura.h"

Buffer_captura::Buffer_captura(QObject * parent = 0):QAbstractVideoSurface(parent){}
QList<QVideoFrame::PixelFormat> Buffer_captura::supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType ) const
{

           QList<QVideoFrame::PixelFormat> formats;
           formats << QVideoFrame::Format_ARGB32;
           formats << QVideoFrame::Format_ARGB32_Premultiplied;
           formats << QVideoFrame::Format_RGB32;
           formats << QVideoFrame::Format_RGB24;
           formats << QVideoFrame::Format_RGB565;
           formats << QVideoFrame::Format_RGB555;
           return formats;

}
bool Buffer_captura::present(const QVideoFrame &frame)
{

        return true;
}
