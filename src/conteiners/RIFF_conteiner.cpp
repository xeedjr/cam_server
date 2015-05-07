/*
 * RIFF_conteiner.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: boy
 */


#include "RIFF_conteiner.h"

enum RIFFConteiner::sample_fmt RIFFConteiner::get_sample_fmt()
{
	return sample_fmt_;
}

uint16_t RIFFConteiner::get_channels()
{
	return channels_;
}

uint32_t RIFFConteiner::get_sample_rate()
{
	return sample_rate_;
}

int RIFFConteiner::set_start()
{
	gettimeofday(&timeval_, NULL);
}

int RIFFConteiner::add_byte(char b)
{
	raw_data_.push_back(b);

	return 1;
}

int RIFFConteiner::reset(void)
{
	raw_data_.resize(0);

	return 1;
}

uint8_t *RIFFConteiner::pointer(void)
{
	return (uint8_t*)raw_data_.data();
}

size_t RIFFConteiner::getindex(void)
{
	return raw_data_.size();
}

struct timeval RIFFConteiner::get_timeval()
{
	return timeval_;
}

RIFFConteiner::~RIFFConteiner()
{

}

RIFFConteiner::RIFFConteiner(uint16_t channels,
								uint32_t sample_rate,
								enum sample_fmt sample_fmt)
{
	gettimeofday(&timeval_, NULL);
	channels_ = channels;
	sample_rate_ = sample_rate;
	sample_fmt_ = sample_fmt;
}
