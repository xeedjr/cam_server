/*
 * yuv_conteiner.h
 *
 *  Created on: Feb 1, 2014
 *      Author: boy
 */

#ifndef YUV_CONTEINER_H_
#define YUV_CONTEINER_H_

#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>
#include <sys/time.h>


class YUVConteiner {
public:
	enum pix_fmt {
		YUV444P,
		YUV422P
	};
private:
	std::vector<uint8_t> Y_;
	std::vector<uint8_t> Cr_;
	std::vector<uint8_t> Cb_;

	size_t height_;
	size_t width_;
	struct timeval timeval_;
	enum pix_fmt pix_fmt_;
public:
	enum pix_fmt get_pix_fmt();
	int get_height();
	int get_width();
	struct timeval get_timeval();
	void set_height(int height);
	void set_width(int width);
	std::vector<uint8_t>* get_y_buffer_pointer();
	std::vector<uint8_t>* get_cr_buffer_pointer();
	std::vector<uint8_t>* get_cb_buffer_pointer();
	int set_size(size_t size);
	void set_start(void);
	int add_byte(char b);
	int reset(void);
	uint8_t *pointer(void);
	size_t getsize(void);
	void push_back_yuv_buffer(void);
	void set_timeval(struct timeval timeval);

	YUVConteiner(enum pix_fmt pix_fmt, size_t width, size_t height);
};

typedef boost::shared_ptr<YUVConteiner> YUVConteinerShrPtr;

#endif /* YUV_CONTEINER_H_ */
