/*
 * FaceCam.h
 *
 *  Created on: Apr 14, 2014
 *      Author: boy
 */

#ifndef FACECAM_H_
#define FACECAM_H_

#include "capture/v4l_mjpeg_capture.h"
#include "capture/alsa_audio_capture.h"

class FaceCam {
	V4LMjpegCaptureShrPtr v4l_mjpeg_capture_;
	AlsaAudioCaptureShrPtr alsa_raw_capture_;
public:
	FaceCam(std::string dev,
			boost::function<void (ConteinerShrPtr cont)> next_jpeg_mod,
			boost::function<void (ConteinerShrPtr cont)> next_riff_mod);
	~FaceCam();
};

typedef boost::shared_ptr<FaceCam> FaceCamShrPtr;

#endif /* FACECAM_H_ */
