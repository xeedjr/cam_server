/*
 * UrlAudioCapture.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#include <cstdlib>
#include <sstream>
#include <thread>
#include <array>
#include <string>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

#include "url_audio_capture.h"


size_t UrlAudioCapture::calback(char* data_ptr, size_t data_size, size_t data_nmemb)
{
	  // Calculate the real size of the incoming buffer
	  size_t realsize = data_size * data_nmemb;
	  char *ptr = data_ptr;
	  size_t size = realsize;

	  if (cancel == 1)
	  {
		  return 0;
	  }

	  if (recv_size == 0)
	  {
		  /// recv header
		  std::memcpy(&waveheader, ptr, sizeof(waveheader));
		  ptr = ptr + sizeof(waveheader);
		  size = size - sizeof(waveheader);
	  };

	  if (recv_size >= sizeof(waveheader))
	  {
		  enum RIFFConteiner::sample_fmt sample_fmt_local;
		  //std::cout.write(ptr, size);
		  conteiner.reset(new Conteiner);
		  switch(waveheader.bitsPerSample)
		  {
		  case 8:
			  sample_fmt_local = RIFFConteiner::sample_fmt::SAMPLE_FMT_U8;
			  break;
		  case 16:
			  sample_fmt_local = RIFFConteiner::sample_fmt::SAMPLE_FMT_S16;
			  break;
		  case 32:
			  sample_fmt_local = RIFFConteiner::sample_fmt::SAMPLE_FMT_S32;
			  break;
		  }
		  conteiner->riff_.reset(new RIFFConteiner(waveheader.numChannels,
												  waveheader.sampleRate,
												  sample_fmt_local));

		  conteiner->riff_->raw_data_.assign(ptr, ptr + size);

			try {
				next_module_(conteiner);
			} catch (...) {
				/// error
				std::cout << "error\n";
			};

	  };

	  recv_size += realsize;

	  // return the real size of the buffer...
	  return realsize;
}


/*size_t UrlAudioCapture::calback(char* ptr, size_t size, size_t nmemb)
{
	  // Calculate the real size of the incoming buffer
	  size_t realsize = size * nmemb;
	  char *content_length_p;
	  char *data_start_p = ptr;
	  size_t copy_data_len = realsize;

	  if (data_start_is == false)
	  {
		  //// not find start
		  if ((content_length_p = std::strstr(ptr, "Content-length: ")) != NULL)
		  {
			  char *start_of_data = NULL;

			  std::sscanf(content_length_p + 16, "%d", &length);
			  if ((start_of_data = std::strstr(content_length_p, "\r\n\r\n")) != NULL)
			  {
				  /// start of data find
				  data_start_is = true;
				  data_start_p = start_of_data + 4;
				  recv_len = 0;
				  copy_data_len = realsize - (data_start_p - ptr);
				  index = 0;
			  }
		  }
	  }

	  if ()

	  if (data_start_is == true)
	  {
		  /// start is
		  /// copy data
		  memcpy(jpeg, data_start_p, copy_data_len);
		  recv_len += copy_data_len;
	  };

	  // return the real size of the buffer...
	  return realsize;
}
*/

void UrlAudioCapture::thread_exec(void)
{
	try {
		request.perform();
	} catch (...) {

	};
}

void UrlAudioCapture::start()
{
    thread = new boost::thread(&UrlAudioCapture::thread_exec, this);
}

UrlAudioCapture::~UrlAudioCapture()
{
	cancel = 1;

	thread->interrupt();
	thread->join();
}

UrlAudioCapture::UrlAudioCapture(std::string url, std::string login_paswd,
		char auth_method, boost::function<void (ConteinerShrPtr cont)> next_module)
{
    next_module_ = next_module;
    cancel = 0;

    using namespace curlpp::Options;
    request.setOpt(Verbose(false));
    request.setOpt(Header(false));
    request.setOpt(WriteStream(&url_videostream2));
    cURLpp::Types::WriteFunctionFunctor functor(this, &UrlAudioCapture::calback);
    cURLpp::Options::WriteFunction *test = new cURLpp::Options::WriteFunction(functor);
    request.setOpt(test);
    request.setOpt(Url(url));
    request.setOpt(UserPwd(login_paswd));
    if (auth_method == 1)
    {
    	request.setOpt(HttpAuth(CURLAUTH_DIGEST));
    };

	return;
}

