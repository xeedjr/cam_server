/*
 * encode.h
 *
 *  Created on: Feb 15, 2014
 *      Author: boy
 */

#ifndef ENCODE_H_
#define ENCODE_H_

extern "C" {

#include <math.h>
#include <stdint.h>

#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
}

#include "../conteiners/yuv_conteiner.h"

class Encode {
	int height_;
	int width_;
    AVCodec *codec;
    AVCodecContext *c= NULL;
    uint64_t pts;
    int ret, x, y, got_output;
    FILE *f;
    AVFrame *frame;
    AVPacket pkt;
    enum AVCodecID codec_id;
    std::vector<uint8_t> *yv, *crv, *cbv;
public:
	int encode_yuvcont(YUVConteinerShrPtr yuv_conteiner);
	~Encode();
	Encode(int height, int width);
};

typedef boost::shared_ptr<Encode> EncodeShrPtr;
#define ENCODE_CALLBACK_FUNC(x)	boost::bind(&Encode::encode_yuvcont, x, _1)

#endif /* ENCODE_H_ */
