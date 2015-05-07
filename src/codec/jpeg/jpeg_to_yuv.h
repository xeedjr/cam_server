/*
 * jpeg_to_yuv.h
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */

#ifndef JPEG_TO_YUV_H_
#define JPEG_TO_YUV_H_

#include <boost/function.hpp>

#include "../../conteiners/conteiner.h"

class JpegToYUV {
	boost::function<void (ConteinerShrPtr cont)> next_module_;
public:
	void callback_jpeg_in(ConteinerShrPtr cont);

	JpegToYUV(boost::function<void (ConteinerShrPtr cont)> next_module);
};

typedef boost::shared_ptr<JpegToYUV> JpegToYUVShrPtr;
#define JPEG_TO_YUV_CALLBACK_FUNC(x)	boost::bind(&JpegToYUV::callback_jpeg_in, x, _1)

#endif /* JPEG_TO_YUV_H_ */
