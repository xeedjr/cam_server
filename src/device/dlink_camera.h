/*
 * dlink_camera.h
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#ifndef DLINK_CAMERA_H_
#define DLINK_CAMERA_H_

#include <string>

#include "capture/url_mjpeg_capture.h"
#include "capture/url_audio_capture.h"

class DlinkCamera {
	UrlMjpegCaptureShrPtr url_camera_capture;
	UrlAudioCaptureShrPtr url_audio_capture;
public:
	DlinkCamera(std::string ip, int port,
			std::string login,  std::string pasword,
			boost::function<void (ConteinerShrPtr cont)> next_jpeg_mod,
			boost::function<void (ConteinerShrPtr cont)> next_riff_mod);
};



#endif /* DLINK_CAMERA_H_ */
