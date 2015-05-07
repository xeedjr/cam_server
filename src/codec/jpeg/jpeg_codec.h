/*
 * jpeg_codec.h
 *
 *  Created on: Feb 16, 2014
 *      Author: boy
 */

#ifndef JPEG_CODEC_H_
#define JPEG_CODEC_H_

#include <vector>

extern "C" {
#include <stdio.h>
#include <jpeglib.h>
}

#include <stdint.h>

class JpegCodec {
public:
	int get_info(std::vector<uint8_t> *jpeg, int *height, int *width);
	int decode_to_yuv2(std::vector<uint8_t> *jpeg,
			int out_width,
			int out_height,
			std::vector<uint8_t> *Y,
			std::vector<uint8_t> *Cr,
			std::vector<uint8_t> *Cb);
	int decode_to_yuv(std::vector<char> *jpeg,
			int out_width,
			int out_height,
			std::vector<uint8_t> *Y,
			std::vector<uint8_t> *Cr,
			std::vector<uint8_t> *Cb);
};




#endif /* JPEG_CODEC_H_ */
