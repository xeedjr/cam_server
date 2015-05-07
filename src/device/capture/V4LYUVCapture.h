/*
 * V4LYUVCapture.h
 *
 *  Created on: Apr 20, 2014
 *      Author: root
 */

#ifndef V4LYUVCAPTURE_H_
#define V4LYUVCAPTURE_H_

#include <boost/thread.hpp>
#include <iostream>
#include <sstream>

#include "../../conteiners/conteiner.h"

extern "C" {
	#include "v4l2uvc.h"
}

class V4LYUVCapture {
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

	~V4LYUVCapture();
	V4LYUVCapture(std::string dev, boost::function<void (ConteinerShrPtr cont)> next_module);
};

typedef boost::shared_ptr<V4LYUVCapture> V4LYUVCaptureShrPtr;

#endif /* V4LYUVCAPTURE_H_ */
