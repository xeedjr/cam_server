/*
 * encode.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: boy
 */


#include "encode.h"

int Encode::encode_yuvcont(YUVConteinerShrPtr yuv_conteiner)
{
	if (height_ != yuv_conteiner->get_height() ||
		width_ != yuv_conteiner->get_width())
	{
		return -1;
	}

    yv = yuv_conteiner->get_y_buffer_pointer();
    crv = yuv_conteiner->get_cr_buffer_pointer();
    cbv = yuv_conteiner->get_cb_buffer_pointer();

	av_init_packet(&pkt);
	pkt.data = NULL;    // packet data will be allocated by the encoder
	pkt.size = 0;

	fflush(stdout);
	/* prepare a dummy image */

	for(y=0; y<c->height; y++) {
		for(x=0; x<c->width; x++) {
			frame->data[0][y * frame->linesize[0] + x] = (unsigned char)(yv->data()[x + (y * c->width)]);
		}
	}

	/// Cb and Cr to 4:4:4 to 4:2:2
	for(y=0; y<c->height; y++) {
		for(x=0; x<c->width/2; x++) {
			/// scanline
			frame->data[1][y * frame->linesize[1] + x] = (unsigned char)cbv->data()[y * c->width + x*2];
			frame->data[2][y * frame->linesize[2] + x] = (unsigned char)crv->data()[y * c->width + x*2];
		}
	}

	frame->pts = pts++;

	/* encode the image */
	ret = avcodec_encode_video2(c, &pkt, frame, &got_output);
	if (ret < 0) {
		fprintf(stderr, "Error encoding frame\n");
		throw (1);
	}

	if (got_output) {
		//printf("Write frame %3d (size=%5d)\n", pts, pkt.size);
		fwrite(pkt.data, 1, pkt.size, f);
		av_free_packet(&pkt);
	}

	return 1;
}

Encode::~Encode()
{
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };

    /* add sequence end code to have a real mpeg file */
    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    avcodec_free_frame(&frame);
    printf("\n");
}

Encode::Encode(int height, int width)
{
	height_ = height;
	width_ = width;
	codec_id = AV_CODEC_ID_MPEG2VIDEO;
	pts = 1;

    avcodec_register_all();

    printf("Encode video file %s\n", "test.mpg");

    /* find the mpeg1 video encoder */
    codec = avcodec_find_encoder(codec_id);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        throw 1;
    }

    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        throw 2;
    }

    /* put sample parameters */
    c->bit_rate = 1000000;
    /* resolution must be a multiple of two */
    c->width = 352;
    c->height = 288;
    /* frames per second */
    c->time_base= (AVRational){1,25};
    c->gop_size = 10; /* emit one intra frame every ten frames */
    c->max_b_frames=1;
    c->pix_fmt = AV_PIX_FMT_YUV422P;

	c->height = height_;
	c->width = width_;

    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        throw 3;
    }

    f = fopen("test.mpg", "wb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", "test.mpg");
        throw 4;
    }

    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        throw 5;
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    /* the image can be allocated by any means and av_image_alloc() is
     * just the most convenient way if av_malloc() is to be used */
    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
                         c->pix_fmt, 32);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        throw 6;
    }
}


