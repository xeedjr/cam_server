/*
 * RIFF_conteiner.h
 *
 *  Created on: Feb 1, 2014
 *      Author: boy
 */

#ifndef RIFF_CONTEINER_H_
#define RIFF_CONTEINER_H_

#include <boost/smart_ptr.hpp>
#include <iostream>
#include <sys/time.h>

class RIFFConteiner {
public:
	enum sample_fmt {
	    SAMPLE_FMT_U8,          ///< unsigned 8 bits
	    SAMPLE_FMT_S16,         ///< signed 16 bits
	    SAMPLE_FMT_S32,         ///< signed 32 bits
	    SAMPLE_FMT_FLT,         ///< float
	    SAMPLE_FMT_DBL,         ///< double
	};
	std::vector<char> raw_data_;
private:
	struct timeval timeval_;
	uint16_t channels_;
	uint32_t sample_rate_;
	enum sample_fmt sample_fmt_;
public:
	enum sample_fmt get_sample_fmt();
	uint16_t get_channels();
	uint32_t get_sample_rate();
	int set_start();
	int add_byte(char b);
	int add_sample(char *sampleIn);
	int reset(void);
	uint8_t *pointer(void);
	size_t getindex(void);
	struct timeval get_timeval();

	~RIFFConteiner();
	RIFFConteiner(uint16_t channels,
			uint32_t sample_rate,
			enum sample_fmt sample_fmt);
};

typedef boost::shared_ptr<RIFFConteiner> RIFFConteinerShrPtr;

#endif /* RIFF_CONTEINER_H_ */
