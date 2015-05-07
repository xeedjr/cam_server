/*
 * alsa_audio_capture.h
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#ifndef alsa_AUDIO_CAPTURE_H_
#define alsa_AUDIO_CAPTURE_H_

#include <boost/thread.hpp>
#include <iostream>
#include <sstream>
#include <alsa/asoundlib.h>

#include "../../conteiners/conteiner.h"

class AlsaAudioCapture {
private:
	std::string dev_;
	char channels_;
	uint8_t sample_size_;
	uint32_t sampe_rate_;

	boost::thread* thread_;
	std::stringstream alsa_videostream2_;
	uint64_t recv_size_;

	snd_pcm_t *capture_handle_;
	snd_pcm_hw_params_t *hw_params_;

	ConteinerShrPtr conteiner;
	boost::function<void (ConteinerShrPtr cont)> next_module_;
public:
	void thread_exec(void);
	void set_alsa(char *dev, char chann);
	void start();

	~AlsaAudioCapture();
	AlsaAudioCapture(std::string dev,
					uint8_t channels,
					uint8_t sample_size,
					uint32_t sampe_rate,
					boost::function<void (ConteinerShrPtr cont)> next_module);
};

typedef boost::shared_ptr<AlsaAudioCapture> AlsaAudioCaptureShrPtr;

#endif /* alsa_AUDIO_CAPTURE_H_ */
