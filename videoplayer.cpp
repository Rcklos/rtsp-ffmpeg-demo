#define VIDEO_PLAYER_DEBUG 1

#include "videoplayer.h"

#include <iostream>
#include <stdio.h>
using namespace std;

VideoPlayer::VideoPlayer() {}
VideoPlayer::~VideoPlayer() {}
void VideoPlayer::startPlay() {
    this->start();
}

void VideoPlayer::run() {
    AVFormatContext *format_ctx;
    const AVCodec *dec;

    int ret = -1;

    avdevice_register_all(); // 注册设备
    avformat_network_init(); // 初始化网络

    // 组合命令option: ffplay -rtsp_transport tcp -max_delay 100 url
    AVDictionary *av_dic = NULL;
    av_dict_set(&av_dic, "rtsp_transport", "tcp", 0);
    av_dict_set(&av_dic, "max_delay", "100", 0);
    char url[] = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mp4";
    // 输入流
    format_ctx = avformat_alloc_context();

#if VIDEO_PLAYER_DEBUG
    cout << "opening avformat input" << endl;
#endif

    ret = avformat_open_input(&format_ctx, url, NULL, &av_dic);
    if(ret != 0) {
        cout << "can't open avformat input of url" << " ret" << ret << endl;
        return;
    }

#if VIDEO_PLAYER_DEBUG
    cout << "finding stream info" << endl;
#endif

    ret = avformat_find_stream_info(format_ctx, NULL);
    if(ret < 0) {
        cout << "could't find stream info" << endl;
        return;
    }

    AVStream *stream = NULL;
    for(int i = 0; i < format_ctx->nb_streams; i++) 
        if(format_ctx->streams[i]->codecpar->codec_type ==  AVMEDIA_TYPE_VIDEO) {
            stream = format_ctx->streams[i];
            break;
        }
    if(stream ==  NULL) {
        cout << "could't find video stream" << endl;
        return;
    }

    const AVCodec *codec_ = avcodec_find_decoder(stream->codecpar->codec_id);
    if(codec_ ==  NULL) {
        cout << "could't find codec" << endl;
        return;
    }
    AVCodecContext *dec_ctx = avcodec_alloc_context3(codec_);
    if(dec_ctx ==  NULL) {
        cout << "could't find context of codec" << endl;
        return;
    }
    avcodec_parameters_to_context(dec_ctx, stream->codecpar);
    ret = avcodec_open2(dec_ctx, codec_, NULL);
    if(ret < 0) {
        cout << "could't open codec" << endl;
        return;
    }

    // ______________frame_______________
    AVFrame *sws_frame = av_frame_alloc();
    static struct SwsContext *sws_ctx = sws_getContext(dec_ctx->width, dec_ctx->height,
            dec_ctx->pix_fmt, dec_ctx->width, dec_ctx->height,
            AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_RGB32, 
            dec_ctx->width, dec_ctx->height, 1);
    frame_buffer = (uint8_t *) av_malloc(buffer_size * sizeof(uint8_t));
    av_image_fill_arrays(sws_frame->data, sws_frame->linesize, frame_buffer, 
            AV_PIX_FMT_RGB32, dec_ctx->width, dec_ctx->height, 1);

    int y_size = dec_ctx->width * dec_ctx->height;
    packet = (AVPacket *) malloc(sizeof(AVPacket));
    av_new_packet(packet, y_size);

    AVFrame *stream_frame = av_frame_alloc();
    while(1) {
        if(av_read_frame(format_ctx, packet) < 0) break;
        ret = avcodec_send_packet(dec_ctx, packet);
        if(ret != 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            cout << "occur error while sending packet" << endl;
            continue;
        }
        else if(ret != 0) {
            continue;
        }

        ret = avcodec_receive_frame(dec_ctx, stream_frame);
        if(ret != 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
            cout << "occur error while sending packet" << endl;
            continue;
        }
        else if(ret != 0) {
            continue;
        }

        sws_scale(sws_ctx,
                stream_frame->data,
                stream_frame->linesize, 0, dec_ctx->height,
                sws_frame->data, sws_frame->linesize);

        QImage tmp((uchar *)frame_buffer, dec_ctx->width, dec_ctx->height, QImage::Format_RGB32);
        this->image = tmp.copy();
        emit sig_GetOneFrame(this->image);

        // for(int i = 0; i < dec_ctx->width; i++)
        //     for(int j = 0; j < dec_ctx->height; j++) {
        //         QRgb rgb = this->image.pixel(i, j);
        //         int r = qRed(rgb);
        //         this->image.setPixel(i, j, qRgb(r, 0, 0));
        //     }
        emit sig_GetRFrame(image);

        av_packet_unref(packet);
        msleep(2);
    } 
    av_free(frame_buffer);
    av_free(sws_frame);
    avcodec_close(dec_ctx);
    avformat_close_input(&format_ctx);
}
