/*
 * edimax_camera.h
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#ifndef EDIMAX_CAMERA_H_
#define EDIMAX_CAMERA_H_

#include <string>

#include "capture/url_mjpeg_capture.h"

class EdimaxCamera {
	UrlMjpegCaptureShrPtr url_camera_capture;
public:
	EdimaxCamera(std::string ip, int port, std::string login,
				std::string pasword, boost::function<void (ConteinerShrPtr cont)> next_jpeg_mod);
};


#endif /* EDIMAX_CAMERA_H_ */
