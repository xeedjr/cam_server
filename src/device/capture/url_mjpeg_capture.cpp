/*
 * UrlMjpegCapture.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#include <cstdlib>
#include <sstream>
#include <thread>
#include <array>
#include <string>
#include <stdio.h>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

#include "url_mjpeg_capture.h"

char* UrlMjpegCapture::find_content_length(char* ptr, size_t size, size_t& content_legth)
{
	char *content_length_p;
	int length;

	if (((content_length_p = std::strstr(ptr, "Content-length: ")) != NULL) ||
		((content_length_p = std::strstr(ptr, "Content-Length: ")) != NULL))
	{
		  char *start_of_data = NULL;

		  sscanf(content_length_p + 16, "%d", &length);

		  content_legth = length;

		  return content_length_p;
	};

	content_legth = 0;

	return NULL;
}

char* UrlMjpegCapture::find_start_content(char* ptr, size_t size)
{
	char *content;
	int length;

	  if ((content = std::strstr(ptr, "\r\n\r\n")) != NULL)
	  {
		  /// start of data find
		  return (content + 4);
	  }

	return NULL;
}

size_t UrlMjpegCapture::calback(char* ptr, size_t size, size_t nmemb)
{
	  // Calculate the real size of the incoming buffer
	  size_t realsize = size * nmemb;
	  char byte = ptr[0];
	  char *start_ptr = ptr;
	  size_t i;

	  char *data_start_p = ptr;

	  if (cancel == 1)
	  {
		  return 0;
	  }

	  if (content_length_ == 0)
	  {
		  /// in header
		  if (find_content_length(ptr, realsize, content_length_) != NULL)
		  {
			  if ((start_ptr = find_start_content(ptr, realsize)) != NULL)
			  {
				  /// we find length  and start content
				  in_content_ = true;
				  cont_.reset(new Conteiner);
				  cont_->jpeg_.reset(new(JpegConteiner));
				  cont_->jpeg_->reset();
				  cont_->jpeg_->get_buffer_pointer()->insert(cont_->jpeg_->get_buffer_pointer()->end(),
						  	  	  	  	  	  	  	  	  	  start_ptr,
						  	  	  	  	  	  	  	  	  	  ptr + realsize);
				  content_length_ -= ((ptr + realsize) - start_ptr);

				  if (content_length_ == 0)
				  {
						try {
							next_module_(cont_);
						} catch (...) {
							/// error
							std::cout << "decode JPEG error\n";
						};
				  };
			  };
		  };
	  }
	  else
	  {
		  /// in content
		  if (content_length_ >= realsize)
		  {
			  /// oll info in buffer is content
			  cont_->jpeg_->get_buffer_pointer()->insert(cont_->jpeg_->get_buffer_pointer()->end(),
					  	  	  	  	  	  	  	  	  	  ptr,
					  	  	  	  	  	  	  	  	  	  ptr + realsize);
			  content_length_ -= realsize;

			  if (content_length_ == 0)
			  {
					try {
						next_module_(cont_);
					} catch (...) {
						/// error
						std::cout << "decode JPEG error\n";
					};
			  };
		  }
		  else
		  {
			  cont_->jpeg_->get_buffer_pointer()->insert(cont_->jpeg_->get_buffer_pointer()->end(),
					  	  	  	  	  	  	  	  	  	  ptr,
					  	  	  	  	  	  	  	  	  	  ptr + content_length_);
			  content_length_ -= content_length_;

			  if (content_length_ == 0)
			  {
					try {
						next_module_(cont_);
					} catch (...) {
						/// error
						std::cout << "decode JPEG error\n";
					};
			  };

			  /// in header
			  if (find_content_length(ptr, realsize, content_length_) != NULL)
			  {
				  if ((start_ptr = find_start_content(ptr, realsize)) != NULL)
				  {
					  /// we find length  and start content
					  in_content_ = true;
					  cont_.reset(new Conteiner);
					  cont_->jpeg_.reset(new(JpegConteiner));
					  cont_->jpeg_->reset();
					  cont_->jpeg_->get_buffer_pointer()->insert(cont_->jpeg_->get_buffer_pointer()->end(),
							  	  	  	  	  	  	  	  	  	  start_ptr,
							  	  	  	  	  	  	  	  	  	  ptr + realsize);
					  content_length_ -= ((ptr + realsize) - start_ptr);

					  if (content_length_ == 0)
					  {
							try {
								next_module_(cont_);
							} catch (...) {
								/// error
								std::cout << "decode JPEG error\n";
							};
					  };
				  };
			  };
		  }
	  }

	  // return the real size of the buffer...
	  return realsize;
}

/*size_t UrlMjpegCapture::calback(char* ptr, size_t size, size_t nmemb)
{
	  // Calculate the real size of the incoming buffer
	  size_t realsize = size * nmemb;
	  char byte = ptr[0];
	  size_t i;

	  if (cancel == 1)
	  {
		  return 0;
	  }

	  for (i = 0; i < realsize; i++)
	  {
		  byte = ptr[i];

		  switch(state)
		  {
		  case 0: /// find stert jpeg frame
			  if ((prev_byte == (char)0xFF) && (byte == (char)0xD8))
			  {
				  /// find start
				  //jpeg_cont = jpeg_buffer_p->get();
				  //jpeg_cont->set_start();
				  //jpeg_cont->source_id = source_id;
				  cont_.reset(new Conteiner);
				  cont_->jpeg_.reset(new(JpegConteiner));
				  cont_->jpeg_->reset();
				  cont_->jpeg_->add_byte(prev_byte);
				  cont_->jpeg_->add_byte(byte);

				  state = 5;
			  }
			  break;
		  case 5: /// collect data and find end jpeg frame
			  if ((prev_byte == (char)0xFF) && (byte == (char)0xD9))
			  {
				  /// find end
				  cont_->jpeg_->add_byte(byte);

				  next_module_(cont_);

				  state = 0;
			  }
			  else
			  {
				  /// it is data
				  /// add it in array
				  cont_->jpeg_->add_byte(byte);
			  };
			  break;
		  case 10: /// execute
			  //std::cout.write(jpeg_cont->pointer(), jpeg_cont->getindex());
			  //std::cout.flush();

			  break;
		  case 15: /// execute

			  break;
		  default:
			  state = 0;
			  break;
		  }

		  prev_byte = byte;
	  };

	  // return the real size of the buffer...
	  return realsize;
}
*/

/*size_t UrlMjpegCapture::calback(char* ptr, size_t size, size_t nmemb)
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

void UrlMjpegCapture::thread_exec(void)
{
	try {
		request.perform();
	} catch (...) {

	};
}

void UrlMjpegCapture::start()
{
    thread = new boost::thread(&UrlMjpegCapture::thread_exec, this);
}

int UrlMjpegCapture::set_source_id(uint16_t source_idIn)
{
	source_id = source_idIn;
}

UrlMjpegCapture::~UrlMjpegCapture()
{
	cancel = 1;

	thread->interrupt();
	thread->join();
}

UrlMjpegCapture::UrlMjpegCapture(std::string url, std::string login_paswd,
		char auth_method, boost::function<void (ConteinerShrPtr cont)> next_module)
{
    state = 0;
    temp = 0;
    cancel = 0;
    content_length_ = 0;

    next_module_ = next_module;

    using namespace curlpp::Options;
    request.setOpt(Verbose(false));
    request.setOpt(Header(false));
    request.setOpt(WriteStream(&url_videostream2));
    cURLpp::Types::WriteFunctionFunctor functor(this, &UrlMjpegCapture::calback);
    cURLpp::Options::WriteFunction *test = new cURLpp::Options::WriteFunction(functor);
    request.setOpt(test);

    using namespace curlpp::Options;
    request.setOpt(Url(url));
    request.setOpt(UserPwd(login_paswd));
    if (auth_method == 1)
    {
    	request.setOpt(HttpAuth(CURLAUTH_DIGEST));
    };
}

