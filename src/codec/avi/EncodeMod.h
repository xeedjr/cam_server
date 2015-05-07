/*
 * EncodeMod.h
 *
 *  Created on: Apr 23, 2014
 *      Author: root
 */

#ifndef ENCODEMOD_H_
#define ENCODEMOD_H_

#include <boost/thread.hpp>

#include "../../utilits/concurent_queue.h"
#include "../../conteiners/conteiner.h"
#include "encode_file.h"

class EncodeMod {
	int height_;
	int width_;
	int fps_;
	std::string file_name_;
	boost::thread* thread_;
	ConcurrentQueue<ConteinerShrPtr> cont_queue_;

	std::string form_name();
	int thread();
public:
	int encode_cont(ConteinerShrPtr conteiner);

	EncodeMod (int width, int height, int fps, std::string file_name);
	~EncodeMod ();
};

typedef boost::shared_ptr<EncodeMod> EncodeModShrPtr;
#define ENCODE_MOD_CALLBACK_FUNC(x)	boost::bind(&EncodeMod::encode_cont, x, _1)


#endif /* ENCODEMOD_H_ */
