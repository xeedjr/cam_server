/*
 * url_audio_capture.h
 *
 *  Created on: Jan 30, 2014
 *      Author: boy
 */

#ifndef URL_AUDIO_CAPTURE_H_
#define URL_AUDIO_CAPTURE_H_

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <sstream>

#include "../../conteiners/conteiner.h"

// Структура, описывающая заголовок WAV файла.
typedef struct
{
	// WAV-формат начинается с RIFF-заголовка:

	// Содержит символы "RIFF" в ASCII кодировке
	// (0x52494646 в big-endian представлении)
	uint8_t chunkId[4];

	// 36 + subchunk2Size, или более точно:
	// 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
	// Это оставшийся размер цепочки, начиная с этой позиции.
	// Иначе говоря, это размер файла - 8, то есть,
	// исключены поля chunkId и chunkSize.
	uint32_t chunkSize;

	// Содержит символы "WAVE"
	// (0x57415645 в big-endian представлении)
	uint8_t format[4];

	// Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
	// Подцепочка "fmt " описывает формат звуковых данных:
	// Содержит символы "fmt "
	// (0x666d7420 в big-endian представлении)
	uint8_t subchunk1Id[4];

	// 16 для формата PCM.
	// Это оставшийся размер подцепочки, начиная с этой позиции.
	uint32_t subchunk1Size;

	// Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
	// Для PCM = 1 (то есть, Линейное квантование).
	// Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
	uint16_t audioFormat;

	// Количество каналов. Моно = 1, Стерео = 2 и т.д.
	uint16_t numChannels;

	// Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
	uint32_t sampleRate;

	// sampleRate * numChannels * bitsPerSample/8
	uint32_t byteRate;

	// numChannels * bitsPerSample/8
	// Количество байт для одного сэмпла, включая все каналы.
	uint16_t blockAlign;

	// Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
	uint16_t bitsPerSample;

	// Подцепочка "data" содержит аудио-данные и их размер.

	// Содержит символы "data"
	// (0x64617461 в big-endian представлении)
	uint8_t subchunk2Id[4];

	// numSamples * numChannels * bitsPerSample/8
	// Количество байт в области данных.
	uint32_t subchunk2Size;

	// Далее следуют непосредственно Wav данные.
} url_audio_capture_WAVHEADER;

class UrlAudioCapture {
	bool data_start_is;
	int cancel;
	curlpp::Cleanup cleaner;
    curlpp::Easy request;
	boost::thread* thread;
	std::stringstream url_videostream2;
	uint64_t recv_size;
	url_audio_capture_WAVHEADER waveheader;

	ConteinerShrPtr conteiner;
	boost::function<void (ConteinerShrPtr cont)> next_module_;
public:
	void thread_exec(void);
	size_t calback(char* ptr, size_t size, size_t nmemb);
	void start();

	~UrlAudioCapture();
	UrlAudioCapture(std::string url, std::string login_paswd,
			char auth_method, boost::function<void (ConteinerShrPtr cont)> next_module);
};

typedef boost::shared_ptr<UrlAudioCapture> UrlAudioCaptureShrPtr;

#endif /* URL_AUDIO_CAPTURE_H_ */
