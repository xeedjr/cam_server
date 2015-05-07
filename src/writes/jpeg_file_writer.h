/*
 * jpeg_file_writer.h
 *
 *  Created on: Feb 8, 2014
 *      Author: boy
 */

#ifndef JPEG_FILE_WRITER_H_
#define JPEG_FILE_WRITER_H_

#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <fstream>
#include <sqlite3.h>

#include "../conteiners/jpeg_conteiner.h"
#include "../conteiners/yuv_conteiner.h"
#include "../codec/encode.h"

class jpeg_file_writer {
	boost::mutex write_jpeg_mutex;
	int write_jpeg(JpegConteinerShrPtr jpeg_conteiner);
public:
	std::fstream file;
	uint64_t count;
	uint64_t in_disk_bytes;
	uint64_t count_files;

	jpeg_file_writer(void);
	~jpeg_file_writer(void);
	int create(const char * file_nameIn);
	int put_jpeg(JpegConteinerShrPtr jpeg_conteiner);
};



#endif /* JPEG_FILE_WRITER_H_ */
