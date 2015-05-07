/*
 * edimax_camera.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#include "edimax_camera.h"


using namespace std;

EdimaxCamera::EdimaxCamera(std::string ip, int port,
			std::string login, std::string pasword,
			boost::function<void (ConteinerShrPtr cont)> next_jpeg_mod)
{
	stringstream set_url_stream;
	string set_url;
	string set_login_pasword;

	set_url_stream << "http://" << ip << ":" << port << "/snapshot.cgi";
	set_url = set_url_stream.str();
	set_login_pasword = login + ':' + pasword;

	url_camera_capture.reset(new UrlMjpegCapture(set_url, set_login_pasword, 0, next_jpeg_mod));

	url_camera_capture->set_source_id(1);
	url_camera_capture->start();
}

