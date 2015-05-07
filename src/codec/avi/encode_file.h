/*
 * encode_file.h
 *
 *  Created on: Mar 29, 2014
 *      Author: root
 */

#ifndef ENCODE_FILE_H_
#define ENCODE_FILE_H_

#include <boost/thread.hpp>

extern "C" {
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <libavutil/opt.h>
#include <libavutil/error.h>
#include <libavutil/mathematics.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#include "../../utilits/concurent_queue.h"
#include "../../conteiners/conteiner.h"

//#define INPUT_PIX_FMT    AV_PIX_FMT_YUV444P /* default pix_fmt */
#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

class EncodeFile {
public:
	boost::thread* thread_;
	ConcurrentQueue<ConteinerShrPtr> cont_queue_;

	int height_;
	int width_;
	int fps_;
	struct timeval first_frame_timeval_;
	struct timeval last_frame_timeval_;
    uint64_t first_frame_timeval_ms_;
    std::string file_name_;

    AVOutputFormat *fmt = NULL;
    AVFormatContext *oc = NULL;
    AVStream *audio_st = NULL, *video_st = NULL;

    bool src_samples_data_allocated;
	uint8_t **src_samples_data = NULL;
	int       src_samples_linesize;
	int       src_nb_samples;
	int       src_nb_samples_fill;

	int max_dst_nb_samples;
	uint8_t **dst_samples_data = NULL;
	int       dst_samples_linesize;
	int       dst_samples_size;

	struct SwrContext *swr_ctx = NULL;

    struct SwsContext *sws_ctx = NULL;
	AVFrame *frame = NULL;
	AVPicture src_picture, dst_picture;
	bool src_picture_allocated;
public:
	uint64_t get_encoded_time(void);
	int add_video_cont(ConteinerShrPtr cont);
	int add_audio_cont(ConteinerShrPtr cont);
	void add_data_to_audio_frame(AVFormatContext *oc, AVStream *st, RIFFConteinerShrPtr riff_cont);
    AVStream *add_stream(AVFormatContext *oc, AVCodec **codec,
                                enum AVCodecID codec_id);
    void open_audio(AVFormatContext *oc, AVCodec *codec, AVStream *st);
    void get_audio_frame(int16_t *samples, int frame_size, int nb_channels, RIFFConteinerShrPtr riff_cont);
    void write_audio_frame(AVFormatContext *oc, AVStream *st);
    void close_audio(AVFormatContext *oc, AVStream *st);
    void open_video(AVFormatContext *oc, AVCodec *codec, AVStream *st);
    void fill_yuv_image(AVPicture *pict,
                               int width, int height, YUVConteinerShrPtr yuv_conteiner);
    void write_video_frame(AVFormatContext *oc, AVStream *st, YUVConteinerShrPtr yuv_conteiner);
    void close_video(AVFormatContext *oc, AVStream *st);
    int encode_cont(ConteinerShrPtr conteiner);
    int thread();

	~EncodeFile();
	EncodeFile(int width, int height, int fps, std::string file_name);

};

typedef boost::shared_ptr<EncodeFile> EncodeFileShrPtr;
#define ENCODE_FILE_CALLBACK_FUNC(x)	boost::bind(&EncodeFile::encode_cont, x, _1)

#endif /* ENCODE_FILE_H_ */
