/*
 * conteiner.h
 *
 *  Created on: Apr 3, 2014
 *      Author: root
 */

#ifndef CONTEINER_H_
#define CONTEINER_H_

#include <boost/smart_ptr.hpp>

#include "jpeg_conteiner.h"
#include "yuv_conteiner.h"
#include "RIFF_conteiner.h"

class Conteiner {
public:
	JpegConteinerShrPtr jpeg_;
	YUVConteinerShrPtr  yuv_;
	RIFFConteinerShrPtr riff_;
};

typedef boost::shared_ptr<Conteiner> ConteinerShrPtr;

#endif /* CONTEINER_H_ */
