/*
 * jpeg_file_writer.cpp
 *
 *  Created on: Feb 8, 2014
 *      Author: boy
 */

#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "jpeg_file_writer.h"

using namespace std;

jpeg_file_writer::jpeg_file_writer(void)
{
	count_files = 0;
	in_disk_bytes = 0;
	count = 0;
}

jpeg_file_writer::~jpeg_file_writer(void)
{
	file.close();
}

int jpeg_file_writer::create(const char * file_nameIn)
{

}

int jpeg_file_writer::write_jpeg(JpegConteinerShrPtr jpeg_conteiner)
{
	size_t size = jpeg_conteiner->getsize();
	string file_name;
	stringstream str;
	uint64_t in_disk_bytesMax = 25;

	in_disk_bytesMax *= 1024 * 1024 * 1024;

	if (in_disk_bytes < in_disk_bytesMax)
	{
		file_name.clear();
		//std::sprintf((char *)file_name.data(), "/disks/raid0/video_cam/cam_%lu_%lu",
		//		jpeg_conteiner_p->timeval.tv_sec,
		//		jpeg_conteiner_p->timeval.tv_usec);
		std::sprintf((char *)file_name.data(), "/disks/raid0/video_cam/source_%d_%lu.jpg",
				0,
						count_files++);
		//file_name = "/disks/raid0/video_cam/cam";
		//str << jpeg_conteiner_p->timeval.tv_sec;

		//+ jpeg_conteiner_p->timeval.tv_usec;

		try {
			file.open(file_name.data(), std::ios::out|std::ios::binary);
		}
		catch (...) {
			std::cout << "Error Open File" << std::endl;
		};

		file.write((const char*)jpeg_conteiner->pointer(), (long int)jpeg_conteiner->getsize());
		file.flush();
		in_disk_bytes += size;

		file.close();
	};

	jpeg_conteiner->push_back_jpeg_buffer();

	return 1;
}

int jpeg_file_writer::put_jpeg(JpegConteinerShrPtr jpeg_conteiner)
{
	boost::mutex::scoped_lock lock(write_jpeg_mutex);

	//write_jpeg(jpeg_conteiner_p);
	//encode.encode_yuvcont(jpeg_conteiner_p);
}
