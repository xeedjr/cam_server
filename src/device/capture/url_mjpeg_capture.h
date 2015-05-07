/*
 * url_mjpeg_capture.h
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#ifndef URL_MJPEG_CAPTURE_H_
#define URL_MJPEG_CAPTURE_H_

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <sstream>

#include "../../conteiners/conteiner.h"

class UrlMjpegCapture {
	bool in_content_;
	size_t content_length_;
	int cancel;
	curlpp::Cleanup cleaner;
    curlpp::Easy request;
	int index;

	char prev_byte;
	bool data_start_is;
	size_t recv_len;
	int state;
	int temp;
	uint16_t source_id;
	boost::thread* thread;
	std::stringstream url_videostream;
	std::stringstream url_videostream2;

	ConteinerShrPtr cont_;
	boost::function<void (ConteinerShrPtr cont)> next_module_;
public:
	char* find_content_length(char* ptr, size_t size, size_t& content_legth);
	char* find_start_content(char* ptr, size_t size);
	int set_source_id(uint16_t source_idIn);
	void thread_exec(void);
	size_t calback(char* ptr, size_t size, size_t nmemb);
	void start();

	~UrlMjpegCapture();
	UrlMjpegCapture(std::string url, std::string login_paswd,
			char auth_method, boost::function<void (ConteinerShrPtr cont)> next_module);
};

typedef boost::shared_ptr<UrlMjpegCapture> UrlMjpegCaptureShrPtr;

#endif /* URL_MJPEG_CAPTURE_H_ */
