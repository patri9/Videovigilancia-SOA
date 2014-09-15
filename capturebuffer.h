#ifndef CAPTUREBUFFER_H
#define CAPTUREBUFFER_H

#include <QAbstractVideoSurface>
#include <QImage>
#include <QVideoFrame>

class CaptureBuffer : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    CaptureBuffer();
    ~CaptureBuffer();
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType =
            QAbstractVideoBuffer::NoHandle) const;
    bool present(const QVideoFrame &frame);

signals:
    void senial(QImage);

};

#endif // CAPTUREBUFFER_H
