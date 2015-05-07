/*
 * AlsaAudioCapture.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#include <cstdlib>
#include <sstream>
#include <thread>
#include <array>
#include <string>

#include "alsa_audio_capture.h"


void AlsaAudioCapture::thread_exec(void)
{
	int i;
	int err;
	void* buffchnls[5];

	if ((err = snd_pcm_prepare (capture_handle_)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	while (1)
	{
		  conteiner.reset(new Conteiner);
		  conteiner->riff_.reset(new RIFFConteiner(channels_,
				  	  	  	  	  	  	  	  	  11025,
				  	  	  	  	  	  	RIFFConteiner::sample_fmt::SAMPLE_FMT_S16));
		  conteiner->riff_->raw_data_.resize(2000);

		if ((err = snd_pcm_readi (capture_handle_,
									conteiner->riff_->raw_data_.data(),
									conteiner->riff_->raw_data_.size()/sample_size_)) !=
											conteiner->riff_->raw_data_.size()/sample_size_)
		{
			fprintf (stderr, "read from audio interface failed (%s)\n",
				 snd_strerror (err));
			//throw 1;
		}
		else
		{

			try {
				next_module_(conteiner);
			} catch (...) {
				/// error
				std::cout << "error\n";
			};

		//std::cout.write((char*)buf, err*sizeof(uint16_t));
		};
	}

	snd_pcm_close (capture_handle_);
}

void AlsaAudioCapture::set_alsa(char *in_dev, char chann)
{

}

void AlsaAudioCapture::start()
{

}

AlsaAudioCapture::~AlsaAudioCapture()
{
	thread_->interrupt();
	thread_->join();
}

AlsaAudioCapture::AlsaAudioCapture(std::string dev,
									uint8_t channels,
									uint8_t sample_size,
									uint32_t sampe_rate,
									boost::function<void (ConteinerShrPtr cont)> next_module)
{
	int err;

	dev_ = dev;
	channels_ = channels;
	sample_size_ = sample_size;
	sampe_rate_ = sampe_rate;
	next_module_ = next_module;

	if ((err = snd_pcm_open(&capture_handle_, dev_.data(), SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n",
				dev_.data(),
			 snd_strerror (err));
		throw 1;
	}

	if ((err = snd_pcm_hw_params_malloc (&hw_params_)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	if ((err = snd_pcm_hw_params_any(capture_handle_, hw_params_)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	if ((err = snd_pcm_hw_params_set_access(capture_handle_, hw_params_, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	snd_pcm_format_t sample_format;

	switch(sample_size_)
	{
	case 1:
		sample_format = SND_PCM_FORMAT_S8;
		break;
	case 2:
		sample_format = SND_PCM_FORMAT_S16_LE;
		break;
	default:
		sample_format = SND_PCM_FORMAT_S16_LE;
		break;
	}

	if ((err = snd_pcm_hw_params_set_format(capture_handle_, hw_params_, sample_format)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	unsigned int rate = sampe_rate_; //11025;

	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle_, hw_params_, &rate, NULL)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	if ((err = snd_pcm_hw_params_set_channels (capture_handle_, hw_params_, channels)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	if ((err = snd_pcm_hw_params(capture_handle_, hw_params_)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		throw 1;
	}

	snd_pcm_hw_params_free (hw_params_);

    thread_ = new boost::thread(&AlsaAudioCapture::thread_exec, this);
}

