/*
 * jpeg_conteiner.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: boy
 */

#include "jpeg_conteiner.h"


JpegConteiner::JpegConteiner(void)
{
	gettimeofday(&timeval_, NULL);
	jpeg_.clear();
}

std::vector<uint8_t>* JpegConteiner::get_buffer_pointer()
{
	return &jpeg_;
}

int JpegConteiner::set_size(size_t size)
{
	jpeg_.reserve(size);

	return 1;
}

void JpegConteiner::set_start(void)
{
	gettimeofday(&timeval_, NULL);
	jpeg_.clear();
}

int JpegConteiner::add_byte(uint8_t b)
{

	jpeg_.push_back(b);


	return 1;
}

int JpegConteiner::reset(void)
{
	jpeg_.clear();

	return 1;
}

uint8_t *JpegConteiner::pointer(void)
{
	return jpeg_.data();
}

size_t JpegConteiner::getsize(void)
{
	return jpeg_.size();
}

void JpegConteiner::push_back_jpeg_buffer(void)
{
	//peg_buffer_p->put(this);
}


