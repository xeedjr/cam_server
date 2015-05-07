/*
 * main.h
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <boost/make_shared.hpp>
#include <boost/asio.hpp>

#include "device/dlink_camera.h"
#include "device/edimax_camera.h"
#include "device/FaceCam.h"
#include "codec/jpeg/jpeg_to_yuv.h"
#include "codec/encode.h"
#include "codec/avi/EncodeMod.h"

class MainClass {
	struct {
		EncodeModShrPtr encode_mod;
		JpegToYUVShrPtr jpeg_to_yuv_;
		FaceCamShrPtr face_cam;
	} line1;
	struct {
		EncodeModShrPtr encode_mod;
		JpegToYUVShrPtr jpeg_to_yuv_;
		boost::shared_ptr<DlinkCamera> dcs_933l_;
	} line2;
	struct {
		EncodeModShrPtr encode_mod;
		JpegToYUVShrPtr jpeg_to_yuv_;
		boost::shared_ptr<EdimaxCamera> ic_3015_;
	} line3;
public:
	int record_line1();
	int record_line2();
	int record_line3();

	~MainClass();
	MainClass();
};

typedef boost::shared_ptr<MainClass> MainClassShrPtr;

#endif /* MAIN_H_ */
