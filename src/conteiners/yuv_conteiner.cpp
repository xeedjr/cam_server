/*
 * yuv_conteiner.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: boy
 */

#include "yuv_conteiner.h"

enum YUVConteiner::pix_fmt YUVConteiner::get_pix_fmt()
{
	return pix_fmt_;
}

int YUVConteiner::get_height()
{
	return height_;
}

int YUVConteiner::get_width()
{
	return width_;
}

struct timeval YUVConteiner::get_timeval()
{
	return timeval_;
}

void YUVConteiner::set_height(int height)
{
	height_ = height;
}

void YUVConteiner::set_width(int width)
{
	width_ = width;
}

std::vector<uint8_t>* YUVConteiner::get_y_buffer_pointer()
{
	return &Y_;
}

std::vector<uint8_t>* YUVConteiner::get_cr_buffer_pointer()
{
	return &Cr_;
}

std::vector<uint8_t>* YUVConteiner::get_cb_buffer_pointer()
{
	return &Cb_;
}

int YUVConteiner::set_size(size_t size)
{
	Y_.reserve(size);

	return 1;
}

void YUVConteiner::set_start(void)
{
	gettimeofday(&timeval_, NULL);
	Y_.clear();
}

void YUVConteiner::set_timeval(struct timeval timeval)
{
	timeval_ = timeval;
}

int YUVConteiner::add_byte(char b)
{

	Y_.push_back(b);


	return 1;
}

int YUVConteiner::reset(void)
{
	Y_.clear();

	return 1;
}

uint8_t *YUVConteiner::pointer(void)
{
	return Y_.data();
}

size_t YUVConteiner::getsize(void)
{
	return Y_.size();
}

void YUVConteiner::push_back_yuv_buffer(void)
{
	//yuv_buffer_p->put(this);
}

YUVConteiner::YUVConteiner(enum pix_fmt pix_fmt, size_t width, size_t height)
{
	gettimeofday(&timeval_, NULL);
	pix_fmt_ = pix_fmt;
	width_ = width;
	height_ = height;
	Y_.clear();
	Cb_.clear();
	Cr_.clear();
}
