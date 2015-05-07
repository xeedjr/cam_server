/*
 * V4LYUVCapture.cpp
 *
 *  Created on: Apr 20, 2014
 *      Author: root
 */

#include <cstdlib>
#include <sstream>
#include <thread>
#include <array>
#include <string>

#include "V4LYUVCapture.h"

size_t V4LYUVCapture::calback(char* ptr, size_t size, size_t nmemb)
{

}


void V4LYUVCapture::thread_exec(void)
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

		  try {
			//next_module_(cont_);
		} catch (...) {
			/// error
			std::cout << "decode JPEG error\n";
		};
	};
}

void V4LYUVCapture::set_url(char *url, char *login_paswd, char auth_method)
{
}

void V4LYUVCapture::start()
{

}

V4LYUVCapture::~V4LYUVCapture()
{
	thread->interrupt();
	thread->join();
}

V4LYUVCapture::V4LYUVCapture(std::string dev, boost::function<void (ConteinerShrPtr cont)> next_module)
{
    state = 0;
    next_module_ = next_module;

    if (init_videoIn(&videoIn, (char *) dev.data(), 640, 480, V4L2_PIX_FMT_YUV444, 1) < 0)
    {
    	throw 1;
    }


    thread = new boost::thread(&V4LYUVCapture::thread_exec, this);

	return;
}
