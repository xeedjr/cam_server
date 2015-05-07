/*
 * EncodeMod.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: root
 */

#include "EncodeMod.h"

int EncodeMod::encode_cont(ConteinerShrPtr conteiner)
{
	if (cont_queue_.count() > 10)
	{
		/// full
		std::cout << "EncodeFile::encode_cont error full" << std::endl;

		throw 1;
	}

	cont_queue_.push(conteiner);
}

std::string EncodeMod::form_name()
{
    struct timeval timeval_;
    std::string time_;
    std::stringstream filename_time;

    gettimeofday(&timeval_, NULL);
    time_.resize(100);
    time_.resize(strftime((char*)time_.data(),
    						time_.size(),
    						"%d%m%Y_%H%M%S",
    						localtime(&timeval_.tv_sec)));

    filename_time << file_name_ << "_" << time_ << "_" << timeval_.tv_usec << ".avi";

    return filename_time.str();
}

int EncodeMod::thread()
{
	ConteinerShrPtr conteiner;

	EncodeFileShrPtr encode_file(new EncodeFile(width_, height_, fps_, form_name()));

	while(1)
	{
		cont_queue_.wait_and_pop(conteiner);

		if (encode_file->get_encoded_time() > 5 * 60 * 1000 * 1000)
		{
			encode_file.reset(new EncodeFile(width_, height_, fps_, form_name()));
		}

		if (conteiner->yuv_.get() != NULL)
		{
			encode_file->add_video_cont(conteiner);
		};
		if (conteiner->riff_.get() != NULL)
		{
			encode_file->add_audio_cont(conteiner);
		};
	};
}

EncodeMod::EncodeMod (int width, int height, int fps, std::string file_name)
{
  // TODO Auto-generated constructor stub
	width_ = width;
	height_ = height;
	fps_ = fps;
	file_name_ = file_name;

    thread_ = new boost::thread(&EncodeMod::thread, this);
}

EncodeMod::~EncodeMod ()
{
  // TODO Auto-generated destructor stub
}

