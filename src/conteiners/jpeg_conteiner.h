/*
 * jpeg_conteiner.h
 *
 *  Created on: Feb 1, 2014
 *      Author: boy
 */

#ifndef JPEG_CONTEINER_H_
#define JPEG_CONTEINER_H_

#include <boost/smart_ptr.hpp>
#include <iostream>
#include <vector>
#include <sys/time.h>

class JpegConteiner {
private:
	std::vector<uint8_t> jpeg_;
public:
	struct timeval timeval_;
public:
	std::vector<uint8_t>* get_buffer_pointer();
	int set_size(size_t size);
	void set_start(void);
	int add_byte(uint8_t b);
	int reset(void);
	uint8_t *pointer(void);
	size_t getsize(void);
	void push_back_jpeg_buffer(void);

	JpegConteiner(void);
};

typedef boost::shared_ptr<JpegConteiner> JpegConteinerShrPtr;

#endif /* JPEG_CONTEINER_H_ */
