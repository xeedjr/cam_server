/*
 * jpeg_to_yuv.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */

#include "jpeg_to_yuv.h"

#include "jpeg_codec.h"

void JpegToYUV::callback_jpeg_in(ConteinerShrPtr cont)
{
	int height, width;
	/// decode
	JpegCodec jpeg_codec;

	jpeg_codec.get_info(cont->jpeg_->get_buffer_pointer(),
						&height,
						&width);

	cont->yuv_.reset(new YUVConteiner(YUVConteiner::pix_fmt::YUV444P,
						width,
						height));
	cont->yuv_->set_timeval(cont->jpeg_->timeval_);


	jpeg_codec.decode_to_yuv2(cont->jpeg_->get_buffer_pointer(),
								width,
								height,
								cont->yuv_->get_y_buffer_pointer(),
								cont->yuv_->get_cr_buffer_pointer(),
								cont->yuv_->get_cb_buffer_pointer());
	next_module_(cont);
}

JpegToYUV::JpegToYUV(boost::function<void (ConteinerShrPtr cont)> next_module)
{
	next_module_ = next_module;
}
