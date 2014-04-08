#ifndef BUFFER_CAPTURA_H
#define BUFFER_CAPTURA_H

#include <QAbstractVideoSurface>

class Buffer_captura : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    Buffer_captura();
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
               QAbstractVideoBuffer::HandleType) const;
    bool present(const QVideoFrame &frame);


};

#endif // BUFFER_CAPTURA_H
