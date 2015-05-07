/*
 * encode_file.cpp
 *
 *  Created on: Mar 29, 2014
 *      Author: root
 */

#include <sstream>
#include <ctime>
//#include <sys/time.h>

#include "encode_file.h"

uint64_t EncodeFile::get_encoded_time(void)
{
	uint64_t start_file_time_us = 0;
	uint64_t last_frame_time_us = 0;

	start_file_time_us = first_frame_timeval_.tv_sec * 1000000 + first_frame_timeval_.tv_usec;
	last_frame_time_us = last_frame_timeval_.tv_sec * 1000000 + last_frame_timeval_.tv_usec;

	return (last_frame_time_us - start_file_time_us);
}

int EncodeFile::add_video_cont(ConteinerShrPtr cont)
{
	double audio_time, video_time;
	uint64_t frame_timeval_ms;
	uint64_t frame_timeout_ms = 1000 / 30;
	uint64_t passed_time_ms;

	if (cont->yuv_.get() != NULL)
	{
		if (first_frame_timeval_.tv_sec == 0 &&
			first_frame_timeval_.tv_usec == 0)
		{
			/// first frame
			first_frame_timeval_ = cont->yuv_->get_timeval();
			first_frame_timeval_ms_ = (first_frame_timeval_.tv_sec * 1000) + (first_frame_timeval_.tv_usec / 1000);
		}

		// Compute current audio and video time.
		audio_time = audio_st ? audio_st->pts.val * av_q2d(audio_st->time_base) : 0.0;
		video_time = video_st ? video_st->pts.val * av_q2d(video_st->time_base) : 0.0;

		// write interleaved audio and video frames
	//	if (!video_st || (video_st && audio_st && audio_time < video_time)) {
	//		write_audio_frame(oc, audio_st);
	//	} else {
		frame_timeval_ms = (cont->yuv_->get_timeval().tv_sec * 1000) + (cont->yuv_->get_timeval().tv_usec / 1000);
		passed_time_ms = frame_timeval_ms - first_frame_timeval_ms_;
		if (frame->pts >= passed_time_ms / frame_timeout_ms)
		{
			/// last frame
			if (frame->pts == passed_time_ms / frame_timeout_ms)
			{
				frame->pts += 1;
			}
			else
			{
				/// frame->pts > passed_time_ms / frame_timeout_ms)
				frame->pts += 1;
			};
		}
		else
		{
			frame->pts = passed_time_ms / frame_timeout_ms;
		}
		//frame->pts++;

		last_frame_timeval_ = cont->yuv_->get_timeval();
		write_video_frame(oc, video_st, cont->yuv_);
		//frame->pts += av_rescale_q(1, video_st->codec->time_base, video_st->time_base);
	//	}
	};
}

int EncodeFile::add_audio_cont(ConteinerShrPtr cont)
{
	double audio_time, video_time;
	uint64_t frame_timeval_ms;
	uint64_t frame_timeout_ms = 1000 / 30;
	uint64_t passed_time_ms;

	if (cont->riff_.get() != NULL)
	{
		// Compute current audio and video time.
		audio_time = audio_st ? audio_st->pts.val * av_q2d(audio_st->time_base) : 0.0;
		video_time = video_st ? video_st->pts.val * av_q2d(video_st->time_base) : 0.0;

		// write interleaved audio and video frames
	//	if (!video_st || (video_st && audio_st && audio_time < video_time)) {
	//		write_audio_frame(oc, audio_st);
	//	} else {
		frame_timeval_ms = (cont->riff_->get_timeval().tv_sec * 1000) + (cont->riff_->get_timeval().tv_usec / 1000);
		passed_time_ms = frame_timeval_ms - first_frame_timeval_ms_;
		//frame->pts = passed_time_ms / frame_timeout_ms;

		add_data_to_audio_frame(oc, audio_st, cont->riff_);
		//write_video_frame(oc, video_st, conteiner->riff_);
		//frame->pts += av_rescale_q(1, video_st->codec->time_base, video_st->time_base);
	//	}
	};
}

/* Prepare a 16 bit dummy audio frame of 'frame_size' samples and
 * 'nb_channels' channels. */
void EncodeFile::get_audio_frame(int16_t *samples, int frame_size, int nb_channels, RIFFConteinerShrPtr riff_cont)
{
    int j, i, v, n = 0;
    int16_t *q;
    int16_t *qin;

    q = samples;
    for (j = 0; j < frame_size; j++)
    {
		qin = (int16_t*)&riff_cont->pointer()[n];
		*q++ = *qin;
		n += 2;
    }
}

void EncodeFile::add_data_to_audio_frame(AVFormatContext *oc, AVStream *st, RIFFConteinerShrPtr riff_cont)
{
	AVCodecContext *c;
    int j, i, v, n = 0, ret;
    int16_t *q;
    int16_t *qin;
    enum AVSampleFormat sample_fmt_in;

    c = st->codec;

    switch(riff_cont->get_sample_fmt())
    {
    case RIFFConteiner::SAMPLE_FMT_S16:
    	sample_fmt_in = AV_SAMPLE_FMT_S16;
    	break;
    }

    if (src_samples_data_allocated == false)
    {
		src_nb_samples = c->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE ?
			10000 : c->frame_size;

		ret = av_samples_alloc_array_and_samples(&src_samples_data, &src_samples_linesize, c->channels,
												 src_nb_samples, sample_fmt_in, 0);
		if (ret < 0) {
			fprintf(stderr, "Could not allocate source samples\n");
			throw 1;
		}

		src_samples_data_allocated = true;

	    /* create resampler context */
	    if (c->sample_fmt != sample_fmt_in)
	    {
	        swr_ctx = swr_alloc();
	        if (!swr_ctx) {
	            fprintf(stderr, "Could not allocate resampler context\n");
	            throw 1;
	        }

	        /* set options */
	        av_opt_set_int       (swr_ctx, "in_channel_count",   riff_cont->get_channels(),       0);
	        av_opt_set_int       (swr_ctx, "in_sample_rate",     riff_cont->get_sample_rate(),    0);
	        av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt",      sample_fmt_in, 0);
	        av_opt_set_int       (swr_ctx, "out_channel_count",  c->channels,       0);
	        av_opt_set_int       (swr_ctx, "out_sample_rate",    c->sample_rate,    0);
	        av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt",     c->sample_fmt,     0);

	        /* initialize the resampling context */
	        if ((ret = swr_init(swr_ctx)) < 0) {
	            fprintf(stderr, "Failed to initialize the resampling context\n");
	            throw 1;
	        }
	    }
    };



    q = (int16_t*)src_samples_data[0];

    for (i = 0; i < riff_cont->raw_data_.size() / 2; i++)
    {
		if (src_nb_samples_fill < src_nb_samples)
		{
			q[src_nb_samples_fill] = ((int16_t*)riff_cont->pointer())[i];
			src_nb_samples_fill++;
		}
		else
		{
			/// src buffer full
		    write_audio_frame(oc, st);
		    src_nb_samples_fill = 0;
		}
    };

    /// frame is
}

void EncodeFile::write_audio_frame(AVFormatContext *oc, AVStream *st)
{
    AVCodecContext *c;
    AVPacket pkt = { 0 }; // data and size must be 0;
    AVFrame *frame = avcodec_alloc_frame();
    int got_packet, ret, dst_nb_samples;

    av_init_packet(&pkt);
    c = st->codec;

//    get_audio_frame((int16_t *)src_samples_data[0], src_nb_samples, c->channels, riff_cont);

    /* convert samples from native format to destination codec format, using the resampler */
    if (swr_ctx) {
        /* compute destination number of samples */
        dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, c->sample_rate) + src_nb_samples,
                                        c->sample_rate, c->sample_rate, AV_ROUND_UP);
        if (dst_nb_samples > max_dst_nb_samples) {
            av_free(dst_samples_data[0]);
            ret = av_samples_alloc(dst_samples_data, &dst_samples_linesize, c->channels,
                                   dst_nb_samples, c->sample_fmt, 0);
            if (ret < 0)
                throw 1;
            max_dst_nb_samples = dst_nb_samples;
            dst_samples_size = av_samples_get_buffer_size(NULL, c->channels, dst_nb_samples,
                                                          c->sample_fmt, 0);
        }

        /* convert to destination format */
        ret = swr_convert(swr_ctx,
                          dst_samples_data, dst_nb_samples,
                          (const uint8_t **)src_samples_data, src_nb_samples);
        if (ret < 0) {
            fprintf(stderr, "Error while converting\n");
            throw 1;
        }
    } else {
        dst_samples_data[0] = src_samples_data[0];
        dst_nb_samples = src_nb_samples;
    }

    frame->nb_samples = dst_nb_samples;
    avcodec_fill_audio_frame(frame, c->channels, c->sample_fmt,
                             dst_samples_data[0], dst_samples_size, 0);

    ret = avcodec_encode_audio2(c, &pkt, frame, &got_packet);
    if (ret < 0) {
        fprintf(stderr, "Error encoding audio frame:\n");
        throw 1;
    }

    if (!got_packet)
        return;

    pkt.stream_index = st->index;

    /* Write the compressed frame to the media file. */
    ret = av_interleaved_write_frame(oc, &pkt);
    if (ret != 0) {
        fprintf(stderr, "Error while writing audio frame: \n");
        throw 1;
    }
    avcodec_free_frame(&frame);
}


/**************************************************************/


/* Prepare a dummy image. */
void EncodeFile::fill_yuv_image(AVPicture *pict,
                           int width, int height, YUVConteinerShrPtr yuv_conteiner)
{
    std::vector<uint8_t> *yv, *crv, *cbv;
    int x, y;

    yv = yuv_conteiner->get_y_buffer_pointer();
    crv = yuv_conteiner->get_cr_buffer_pointer();
    cbv = yuv_conteiner->get_cb_buffer_pointer();

    if (yv->size() == height * pict->linesize[0])
    {
    	memcpy(pict->data[0], yv->data(), height * pict->linesize[0]);
    }
    if (cbv->size() == height * pict->linesize[0])
    {
    	memcpy(pict->data[1], cbv->data(), height * pict->linesize[1]);
    }
    if (crv->size() == height * pict->linesize[0])
    {
    	memcpy(pict->data[2], crv->data(), height * pict->linesize[2]);
    }
/*	for(y=0; y<height; y++) {
		for(x=0; x<width; x++) {
			pict->data[0][y * pict->linesize[0] + x] = (unsigned char)(yv->data()[x + (y * width)]);
			pict->data[1][y * pict->linesize[1] + x] = (unsigned char)(cbv->data()[x + (y * width)]);
			pict->data[2][y * pict->linesize[2] + x] = (unsigned char)(crv->data()[x + (y * width)]);

		}
	}
*/
/*	/// Cb and Cr to 4:4:4 to 4:2:2
	for(y=0; y<height; y++) {
		for(x=0; x<width/2; x++) {
			/// scanline
			pict->data[1][y * pict->linesize[1] + x] = (unsigned char)cbv->data()[y * width + x*2];
			pict->data[2][y * pict->linesize[2] + x] = (unsigned char)crv->data()[y * width + x*2];
		}
	}
*/
    /* Cb and Cr */
/*    for (y = 0; y < height / 2; y++) {
        for (x = 0; x < width / 2; x++) {
            pict->data[1][y * pict->linesize[1] + x] = 128;
            pict->data[2][y * pict->linesize[2] + x] = 128;
        }
    }
   */
}

void EncodeFile::write_video_frame(AVFormatContext *oc, AVStream *st, YUVConteinerShrPtr yuv_conteiner)
{
    int ret;
	int sws_flags = SWS_BICUBIC;
    AVCodecContext *c = st->codec;
    enum AVPixelFormat input_pix_fmt;

    switch(yuv_conteiner->get_pix_fmt())
    {
    case YUVConteiner::YUV444P:
    	input_pix_fmt = AV_PIX_FMT_YUV444P;
    	break;
    case YUVConteiner::YUV422P:
    	input_pix_fmt = AV_PIX_FMT_YUV422P;
    	break;
    default:
    	input_pix_fmt = AV_PIX_FMT_NONE;
    	break;
    }

    if ((c->pix_fmt != input_pix_fmt) &&
    	(src_picture_allocated == false))
	{
		ret = avpicture_alloc(&src_picture,
								input_pix_fmt,
								yuv_conteiner->get_width(),
								yuv_conteiner->get_height());
		if (ret < 0) {
			fprintf(stderr, "Could not allocate temporary picture: \n");
			throw 1;
		}
		src_picture_allocated = true;
	}

	if (c->pix_fmt != input_pix_fmt) {
		// as we only generate a YUV420P picture, we must convert it
		// to the codec pixel format if needed
		if (!sws_ctx) {
			sws_ctx = sws_getContext(yuv_conteiner->get_width(),
									yuv_conteiner->get_height(),
									input_pix_fmt,
									 c->width,
									 c->height,
									 c->pix_fmt,
									 sws_flags, NULL, NULL, NULL);
			if (!sws_ctx) {
				fprintf(stderr,
						"Could not initialize the conversion context\n");
				throw (1);
			}
		}
		fill_yuv_image(&src_picture,
						yuv_conteiner->get_width(),
						yuv_conteiner->get_height(),
						yuv_conteiner);
		sws_scale(sws_ctx,
				  (const uint8_t * const *)src_picture.data, src_picture.linesize,
				  0, c->height, dst_picture.data, dst_picture.linesize);
	} else {
		fill_yuv_image(&dst_picture,
						yuv_conteiner->get_width(),
						yuv_conteiner->get_height(),
						yuv_conteiner);
	}

    if (oc->oformat->flags & AVFMT_RAWPICTURE) {
        // Raw video case - directly store the picture in the packet
        AVPacket pkt;
        av_init_packet(&pkt);

        pkt.flags        |= AV_PKT_FLAG_KEY;
        pkt.stream_index  = st->index;
        pkt.data          = dst_picture.data[0];
        pkt.size          = sizeof(AVPicture);

        ret = av_interleaved_write_frame(oc, &pkt);
        if (ret < 0) {
            fprintf(stderr, "Error encoding video frame 2: \n");
            av_free_packet(&pkt);
            throw (1);
        }

    } else {
        AVPacket pkt = { 0 };
        int got_packet;
        av_init_packet(&pkt);

        // encode the image
        ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
        if (ret < 0) {
            fprintf(stderr, "Error encoding video frame: \n");
            av_free_packet(&pkt);
            throw (1);
        }
        // If size is zero, it means the image was buffered.

        if (!ret && got_packet && pkt.size) {
            pkt.stream_index = st->index;

            // Write the compressed frame to the media file.
            ret = av_interleaved_write_frame(oc, &pkt);
            if (ret < 0) {
                fprintf(stderr, "Error encoding video frame 1: \n");
                av_free_packet(&pkt);
                throw (1);
            }
            ret = av_interleaved_write_frame(oc, NULL);
            if (ret < 0) {
                fprintf(stderr, "Error encoding video frame 2: \n");
                av_free_packet(&pkt);
                throw (1);
            }
            //ret = av_write_frame(oc, &pkt);

        } else {
            ret = 0;
        }
    }

    if (ret != 0) {
        fprintf(stderr, "Error while writing video frame: \n");
        throw (1);
    }
}





/* Add an output stream. */
AVStream *EncodeFile::add_stream(AVFormatContext *oc, AVCodec **codec,
                            enum AVCodecID codec_id)
{
    AVCodecContext *c;
    AVStream *st;

    /* find the encoder */
    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec)) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
                avcodec_get_name(codec_id));
        throw 1;
    }

    st = avformat_new_stream(oc, *codec);
    if (!st) {
        fprintf(stderr, "Could not allocate stream\n");
        throw 1;
    }
    st->id = oc->nb_streams-1;
    c = st->codec;

    switch ((*codec)->type) {
    case AVMEDIA_TYPE_AUDIO:
        c->sample_fmt  = AV_SAMPLE_FMT_FLTP;
        //c->bit_rate    = 64000;
        //c->sample_rate = 44100;
        c->bit_rate    = 64000;
        c->sample_rate = 11025;
        c->channels    = 1;
        break;

    case AVMEDIA_TYPE_VIDEO:
        c->codec_id = codec_id;

        c->bit_rate = 400000;
        /* Resolution must be a multiple of two. */
       // c->width    = 352;
       // c->height   = 288;
        c->width    = width_;
        c->height   = height_;
        /* timebase: This is the fundamental unit of time (in seconds) in terms
         * of which frame timestamps are represented. For fixed-fps content,
         * timebase should be 1/framerate and timestamp increments should be
         * identical to 1. */
        c->time_base.den = fps_;
        c->time_base.num = 1;
        c->gop_size      = 12; /* emit one intra frame every twelve frames at most */
        c->pix_fmt       = STREAM_PIX_FMT;
        if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
            /* just for testing, we also add B frames */
            c->max_b_frames = 2;
        }
        if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
            /* Needed to avoid using macroblocks in which some coeffs overflow.
             * This does not happen with normal video, it just happens here as
             * the motion of the chroma plane does not match the luma plane. */
            c->mb_decision = 2;
        }
    break;

    default:
        break;
    }

    /* Some formats want stream headers to be separate. */
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= CODEC_FLAG_GLOBAL_HEADER;

    return st;
}

/**************************************************************/
/* audio output */

void EncodeFile::open_audio(AVFormatContext *oc, AVCodec *codec, AVStream *st)
{
    AVCodecContext *c;
    int ret;

    c = st->codec;

    /* open it */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open audio codec: \n");
        throw 1;
    }

    /* compute the number of converted samples: buffering is avoided
     * ensuring that the output buffer will contain at least all the
     * converted input samples */
    //max_dst_nb_samples = src_nb_samples;
    max_dst_nb_samples = c->codec->capabilities & CODEC_CAP_VARIABLE_FRAME_SIZE ?
            10000 : c->frame_size;
    ret = av_samples_alloc_array_and_samples(&dst_samples_data, &dst_samples_linesize, c->channels,
                                             max_dst_nb_samples, c->sample_fmt, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate destination samples\n");
        throw 1;
    }
    dst_samples_size = av_samples_get_buffer_size(NULL, c->channels, max_dst_nb_samples,
                                                  c->sample_fmt, 0);
}

void EncodeFile::open_video(AVFormatContext *oc, AVCodec *codec, AVStream *st)
{
    int ret;
    AVCodecContext *c = st->codec;

    /* open the codec */
    ret = avcodec_open2(c, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: \n");
        throw 1;
    }

    /* allocate and init a re-usable frame */
    frame = avcodec_alloc_frame();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        throw 1;
    }

    /* Allocate the encoded raw picture. */
    ret = avpicture_alloc(&dst_picture, c->pix_fmt, c->width, c->height);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate picture: \n");
        throw 1;
    }

    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    /*if (c->pix_fmt != INPUT_PIX_FMT) {
        ret = avpicture_alloc(&src_picture, INPUT_PIX_FMT, c->width, c->height);
        if (ret < 0) {
            fprintf(stderr, "Could not allocate temporary picture: \n");
            throw 1;
        }
    }
*/
    /* copy data and linesize picture pointers to frame */
    *((AVPicture *)frame) = dst_picture;
}

void EncodeFile::close_audio(AVFormatContext *oc, AVStream *st)
{
    avcodec_close(st->codec);
    if (src_samples_data != NULL)
    {
    	av_free(src_samples_data[0]);
    }
    if (dst_samples_data != NULL)
    {
    	av_free(dst_samples_data[0]);
    };
}

void EncodeFile::close_video(AVFormatContext *oc, AVStream *st)
{
    avcodec_close(st->codec);
    if (src_picture.data[0] != NULL)
    {
    	av_free(src_picture.data[0]);
    };
    if (dst_picture.data[0] != NULL)
    {
    	av_free(dst_picture.data[0]);
    }
    if (frame != NULL)
    {
    	av_free(frame);
    };
}

EncodeFile::~EncodeFile()
{
    /* Write the trailer, if any. The trailer must be written before you
     * close the CodecContexts open when you wrote the header; otherwise
     * av_write_trailer() may try to use memory that was freed on
     * av_codec_close(). */
    av_write_trailer(oc);

    /* Close each codec. */
    if (video_st)
        close_video(oc, video_st);
    if (audio_st)
        close_audio(oc, audio_st);

    if (!(fmt->flags & AVFMT_NOFILE))
        /* Close the output file. */
        avio_close(oc->pb);

    /* free the stream */
    avformat_free_context(oc);
}

EncodeFile::EncodeFile(int width, int height, int fps, std::string file_name)
{
    AVCodec *audio_codec, *video_codec;
    int ret;

    src_samples_data_allocated = false;
    src_picture_allocated = false;
	height_ = height;
	width_ = width;
	fps_ = fps;
	file_name_ = file_name;
	first_frame_timeval_.tv_sec = 0;
	first_frame_timeval_.tv_usec = 0;
	last_frame_timeval_.tv_sec = 0;
	last_frame_timeval_.tv_usec = 0;
	src_nb_samples_fill = 0;
	std::memset(&src_picture, 0, sizeof(AVPicture));
	std::memset(&dst_picture, 0, sizeof(AVPicture));

    /* Initialize libavcodec, and register all codecs and formats. */
    av_register_all();

    avformat_alloc_output_context2(&oc, NULL, "avi", file_name_.data());
    if (!oc) {
        throw 1;
    }

    fmt = oc->oformat;

    /* Add the audio and video streams using the default format codecs
     * and initialize the codecs. */
    video_st = NULL;
    audio_st = NULL;

    //fmt->video_codec = AV_CODEC_ID_MPEG2VIDEO; /// Change default video encoder

    if (fmt->video_codec != AV_CODEC_ID_NONE) {
        video_st = add_stream(oc, &video_codec, fmt->video_codec);
    }
    if (fmt->audio_codec != AV_CODEC_ID_NONE) {
        audio_st = add_stream(oc, &audio_codec, fmt->audio_codec);
    }

    /* Now that all the parameters are set, we can open the audio and
     * video codecs and allocate the necessary encode buffers. */
    if (video_st)
        open_video(oc, video_codec, video_st);
    if (audio_st)
        open_audio(oc, audio_codec, audio_st);

    av_dump_format(oc, 0, file_name_.data(), 1);

    /* open the output file, if needed */
    if (!(fmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&oc->pb, file_name_.data(), AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open '%s': \n", file_name_.data());
            throw 1;
        }
    }

    /* Write the stream header, if any. */
    ret = avformat_write_header(oc, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: \n");
        throw 1;
    }

	if (frame)
		frame->pts = 0;
}



