/*
 * v4l_mjpeg_capture.h
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#ifndef V4L_MJPEG_CAPTURE_H_
#define V4L_MJPEG_CAPTURE_H_

#include <boost/thread.hpp>
#include <iostream>
#include <sstream>

#include "../../conteiners/conteiner.h"

extern "C" {
	#include "v4l2uvc.h"
}

class V4LMjpegCapture {
	boost::function<void (ConteinerShrPtr cont)> next_module_;
	ConteinerShrPtr cont_;
	struct vdIn videoIn;
public:
	char jpeg[10000];
	int index;
	int length;
	char prev_byte;
	bool data_start_is;
	size_t recv_len;
	int state;
	int temp;
	boost::thread* thread;
	std::stringstream v4l_videostream;
	std::stringstream v4l_videostream2;
	void thread_exec(void);
	size_t calback(char* ptr, size_t size, size_t nmemb);
	void set_url(char *url, char *login_paswd, char auth_method);
	void start();

	~V4LMjpegCapture();
	V4LMjpegCapture(std::string dev, boost::function<void (ConteinerShrPtr cont)> next_module);
};

typedef boost::shared_ptr<V4LMjpegCapture> V4LMjpegCaptureShrPtr;

#endif /* V4L_MJPEG_CAPTURE_H_ */
