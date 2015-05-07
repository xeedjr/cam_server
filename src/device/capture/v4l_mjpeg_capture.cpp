/*
 * V4LMjpegCapture.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#include <cstdlib>
#include <sstream>
#include <thread>
#include <array>
#include <string>

#include "v4l_mjpeg_capture.h"

size_t V4LMjpegCapture::calback(char* ptr, size_t size, size_t nmemb)
{

}


void V4LMjpegCapture::thread_exec(void)
{
	while (1)
	{
		if (uvcGrab (&videoIn) < 0)
		{
			fprintf (stderr, "Error grabbing\n");
			close_v4l2 (&videoIn);
			throw 1;
		}
		  cont_.reset(new Conteiner);
		  cont_->jpeg_.reset(new(JpegConteiner));
		  cont_->jpeg_->reset();
		  cont_->jpeg_->get_buffer_pointer()->assign((char*)videoIn.tmpbuffer,
				  	  	  	  	  	  	  	  	  	 (char*)(videoIn.tmpbuffer + videoIn.buf.bytesused + DHT_SIZE));
		try {
			next_module_(cont_);
		} catch (...) {
			/// error
			std::cout << "decode JPEG error\n";
		};
	};
}

void V4LMjpegCapture::set_url(char *url, char *login_paswd, char auth_method)
{
}

void V4LMjpegCapture::start()
{

}

V4LMjpegCapture::~V4LMjpegCapture()
{
	thread->interrupt();
	thread->join();
}

V4LMjpegCapture::V4LMjpegCapture(std::string dev, boost::function<void (ConteinerShrPtr cont)> next_module)
{
    state = 0;
    next_module_ = next_module;

    if (init_videoIn(&videoIn, (char *) dev.data(), 640, 480, V4L2_PIX_FMT_MJPEG, 1) < 0)
    {
    	throw 1;
    }


    thread = new boost::thread(&V4LMjpegCapture::thread_exec, this);

	return;
}

