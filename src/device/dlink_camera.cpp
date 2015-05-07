/*
 * dlink_cam.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#include "dlink_camera.h"

using namespace std;

DlinkCamera::DlinkCamera(std::string ip, int port,
		std::string login,  std::string pasword,
		boost::function<void (ConteinerShrPtr cont)> next_jpeg_mod,
		boost::function<void (ConteinerShrPtr cont)> next_riff_mod)
{
	stringstream set_url_stream_video;
	stringstream set_url_stream_audio;
	//string set_url_video, set_url_audio;
	string set_login_pasword;


	set_url_stream_video << "http://" << ip << ":" << port << "/dgvideo.cgi";
	set_url_stream_audio << "http://" << ip << ":" << port << "/dgaudio.cgi";
	//set_url_video = set_url_stream_video.str();
	//set_url_audio = set_url_stream_audio.str();
	set_login_pasword = login + ':' + pasword;

	url_camera_capture.reset(new UrlMjpegCapture(set_url_stream_video.str(), set_login_pasword, 1, next_jpeg_mod));

	url_camera_capture->set_source_id(1);
	url_camera_capture->start();

	url_audio_capture.reset(new UrlAudioCapture(set_url_stream_audio.str(), set_login_pasword, 1, next_riff_mod));
	url_audio_capture->start();
}
