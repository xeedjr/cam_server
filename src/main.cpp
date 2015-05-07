//============================================================================
// Name        : cam_server.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <unistd.h>
#include <iostream>
using namespace std;
#include <boost/asio/signal_set.hpp>

#include "main.h"

MainClassShrPtr mainclass;

int MainClass::record_line1()
{
	line1.encode_mod.reset(new EncodeMod(640, 480,
											30,
											"/home/boy/video_cam/face_cam"));

	line1.jpeg_to_yuv_.reset(new JpegToYUV(ENCODE_MOD_CALLBACK_FUNC(line1.encode_mod.get())));

	line1.face_cam.reset(new FaceCam("/dev/video0",
								JPEG_TO_YUV_CALLBACK_FUNC(line1.jpeg_to_yuv_.get()),
								ENCODE_MOD_CALLBACK_FUNC(line1.encode_mod.get())));

}

int MainClass::record_line2()
{
	line2.encode_mod.reset(new EncodeMod(640, 480,
											30,
											"/home/boy/video_cam/dcs_933l"));

	line2.jpeg_to_yuv_.reset(new JpegToYUV(ENCODE_MOD_CALLBACK_FUNC(line2.encode_mod.get())));

	line2.dcs_933l_.reset(new DlinkCamera("192.168.56.30", 8030,
						"admin", "229rfvthf",
						JPEG_TO_YUV_CALLBACK_FUNC(line2.jpeg_to_yuv_.get()),
						ENCODE_MOD_CALLBACK_FUNC(line2.encode_mod.get())));
}

int MainClass::record_line3()
{
	line3.encode_mod.reset(new EncodeMod(1280, 960,
											30,
											"/home/boy/video_cam/ic_3015"));

	line3.jpeg_to_yuv_.reset(new JpegToYUV(ENCODE_MOD_CALLBACK_FUNC(line3.encode_mod.get())));

	line3.ic_3015_.reset(new EdimaxCamera("192.168.56.31", 80,
									"admin", "1234",
									JPEG_TO_YUV_CALLBACK_FUNC(line3.jpeg_to_yuv_.get())));
}

MainClass::~MainClass()
{
	line1.face_cam.reset();
	line1.jpeg_to_yuv_.reset();
	line1.encode_mod.reset();

	line2.dcs_933l_.reset();
	line2.jpeg_to_yuv_.reset();
	line2.encode_mod.reset();

	line3.ic_3015_.reset();
	line3.jpeg_to_yuv_.reset();
	line3.encode_mod.reset();
}

MainClass::MainClass()
{
	record_line1();
	//record_line2();
	//record_line3();
}


static bool run = true;

void handler(
    const boost::system::error_code& error,
    int signal_number)
{
  if (!error)
  {
      std::cout << "signal" << std::endl;
      run = false;
  }
}

int main() {
	boost::asio::io_service signal_io_service;
	// Construct a signal set registered for process termination.
	boost::asio::signal_set signals(signal_io_service, SIGTERM, SIGTERM);

	try {
		mainclass.reset(new MainClass);
	} catch(std::exception& e) {
		std::cerr << "exception caught: " << e.what() << '\n';
		throw;
	};

	// Start an asynchronous wait for one of the signals to occur.
	signals.async_wait(handler);

	signal_io_service.run();

	while(run)
	{
		boost::this_thread::yield();
	}

	return 0;
}
