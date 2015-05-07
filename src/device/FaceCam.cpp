/*
 * FaceCam.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: boy
 */

#include "FaceCam.h"

FaceCam::FaceCam(std::string dev,
		boost::function<void (ConteinerShrPtr cont)> next_jpeg_mod,
		boost::function<void (ConteinerShrPtr cont)> next_riff_mod)
{
	// TODO Auto-generated constructor stub

	v4l_mjpeg_capture_.reset(new V4LMjpegCapture(dev, next_jpeg_mod));
	alsa_raw_capture_.reset(new AlsaAudioCapture("hw:1,0", 1, 2, 11025, next_riff_mod));
}

FaceCam::~FaceCam() {
	// TODO Auto-generated destructor stub
}

