#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include <QImage>

extern "C"
{
    #include <libavdevice/avdevice.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/imgutils.h>
    #include <libswscale/swscale.h>
}

//2017.8.10---lizhen
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class VideoPlayer : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer();
    ~VideoPlayer();

    void startPlay();

signals:
    void sig_GetOneFrame(QImage); //每获取到一帧图像 就发送此信号
    void sig_GetRFrame(QImage);   ///2017.8.11---lizhen

protected:
    void run();

private:
    QString mFileName;

    //2017.8.10---lizhen
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    unsigned char *frame_buffer;
    unsigned char *out_buffer;
    AVPacket *packet;
    QImage image;
};

#endif // VIDEOPLAYER_H
