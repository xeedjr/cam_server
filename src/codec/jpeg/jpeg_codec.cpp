/*
 * jpeg_codec.cpp
 *
 *  Created on: Feb 16, 2014
 *      Author: boy
 */

#include <stddef.h>

#include <cstring>
#include "jpeg_codec.h"
#include "JPEGReader.h"

int JpegCodec::get_info(std::vector<uint8_t> *jpeg, int *height, int *width)
{
	JPEGReader loader;

	loader.header_buffer(*jpeg);

    *width = loader.width();
    *height = loader.height();

    return 0;
}

int JpegCodec::decode_to_yuv2(std::vector<uint8_t> *jpeg,
								int out_width,
								int out_height,
								std::vector<uint8_t> *Y,
								std::vector<uint8_t> *Cr,
								std::vector<uint8_t> *Cb)
{
	JPEGReader loader;

	loader.header_buffer(*jpeg);

    if ((loader.width() != out_width) && (loader.height() != out_height))
    {
    	/// error format
    	return -1;
    };

	loader.setColorSpace(JPEG::COLOR_YCC);

    std::vector<unsigned char> buffer(loader.width() * loader.height() * loader.components());
    std::vector<unsigned char*> rows(loader.height(), NULL);
    for (unsigned i = 0; i < loader.height(); ++i)
        rows[i] = &buffer[i * loader.width() * loader.components()];

    loader.load(rows.begin());

//	for( int row = 0; row < (int)loader.height(); row += cinfo.max_v_samp_factor * DCTSIZE )
//	{
		//jpeg_read_raw_data( &cinfo, yuvptr, cinfo.max_v_samp_factor * DCTSIZE );
		/// 4:4:4
		for( int i = 0; i != loader.height(); ++i)
		{
			Y->insert(Y->end(), (uint8_t*)loader.colormap()[ 1 * i ], (uint8_t*)loader.colormap()[ 1 * i ] + loader.width());
			Cb->insert(Cb->end(), (uint8_t*)loader.colormap()[ 2 * i ], (uint8_t*)loader.colormap()[ 2 * i ] + loader.width());
			Cr->insert(Cr->end(), (uint8_t*)loader.colormap()[ 3 * i ], (uint8_t*)loader.colormap()[ 3 * i ] + loader.width());
		}
//	}

	/*for( int i = 1; i <= loader.height(); i++ )
	{
		for( int j = 1; j <= loader.width() * 3; j = j + 3 )
		{
			Y->push_back(buffer[ i * j]);
			Cr->push_back(buffer[ i * j + 1]);
			Cb->push_back(buffer[ i * j + 2]);
		}
	};
	*/
}


/*int JpegCodec::get_info(unsigned char *buffer, size_t length, int *height, int *width)
{
    struct jpeg_error_mgr err;
    struct jpeg_decompress_struct cinfo = {0};

    // create decompressor
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&err);
    cinfo.do_fancy_upsampling = FALSE;

    // set source buffer
    jpeg_mem_src(&cinfo, buffer, length);

    // read jpeg header
    jpeg_read_header(&cinfo, (boolean)1);
    *width = cinfo.image_width;
    *height = cinfo.image_height;
    switch (cinfo.jpeg_color_space)
    {
        case JCS_YCbCr:
          //mjpeg_info("YUV colorspace detected.\n");
          cinfo.out_color_space = JCS_YCbCr;
          break;
        case JCS_GRAYSCALE:
          //mjpeg_info("Grayscale colorspace detected.\n");
          cinfo.out_color_space = JCS_GRAYSCALE;
          break;
        default:
          //mjpeg_error("Unsupported colorspace detected.\n"); break;
        	break;
    };

    jpeg_destroy_decompress(&cinfo);

    return 0;
}
*/

/*int JpegCodec::decode_to_yuv2(std::vector<char> *jpeg,
								int out_width,
								int out_height,
								std::vector<uint8_t> *Y,
								std::vector<uint8_t> *Cr,
								std::vector<uint8_t> *Cb)
{
	jpeg_error_mgr err = {};
	jpeg_decompress_struct cinfo = {};

    // create decompressor
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&err);
    cinfo.do_fancy_upsampling = FALSE;

    // set source buffer
    jpeg_mem_src(&cinfo, (unsigned char *)jpeg->data(), jpeg->size());

	jpeg_read_header( &cinfo, TRUE );

    if ((cinfo.image_width != out_width) && (cinfo.image_height != out_height))
    {
    	/// error format
    	return -1;
    };

	cinfo.out_color_space = JCS_YCbCr;
	cinfo.raw_data_out = TRUE;

	jpeg_start_decompress( &cinfo );

	int const lines = cinfo.max_v_samp_factor * DCTSIZE;

	std::vector< JSAMPLE > rows( lines * cinfo.output_width * cinfo.out_color_components );
	std::vector< JSAMPROW > cols[ 3 ];

	JSAMPLE * ptr = &rows[ 0 ];

	for( int c = 0; c != 3; ++c )
	{
	    cols[ c ].resize( lines );

	    for( int i = 0; i != lines; ++i, ptr += cinfo.output_width )
	    {
	        cols[ c ][ i ] = ptr;
	    }
	}

	JSAMPARRAY yuvptr[ 3 ] = {
	    &cols[ 0 ][ 0 ],
	    &cols[ 1 ][ 0 ],
	    &cols[ 2 ][ 0 ]
	};

	std::vector< JSAMPLE > dst( cinfo.output_width * cinfo.output_height * 3 / 2 );

	int const len = cinfo.output_width * cinfo.output_height;

	JSAMPLE * y = &dst[ 0 ];
	JSAMPLE * u = y + len;
	JSAMPLE * v = u + len / 4;

	for( int row = 0; row < (int) cinfo.output_height; row += cinfo.max_v_samp_factor * DCTSIZE )
	{
	    jpeg_read_raw_data( &cinfo, yuvptr, cinfo.max_v_samp_factor * DCTSIZE );

	    /// 4:4:4


	    for( int i = 0; i != lines; ++i, y += cinfo.output_width )
	    {
		    Y->insert(Y->end(), (uint8_t*)cols[ 0 ][ i ], (uint8_t*)cols[ 0 ][ i ] + cinfo.output_width);
		    Cb->insert(Cb->end(), (uint8_t*)cols[ 1 ][ i ], (uint8_t*)cols[ 1 ][ i ] + cinfo.output_width);
		    Cr->insert(Cr->end(), (uint8_t*)cols[ 2 ][ i ], (uint8_t*)cols[ 2 ][ i ] + cinfo.output_width);
		    //Cb->assign(cols[ 1 ], cols[ 0 ] + cinfo.output_width);
		    //Cr->assign(cols[ 2 ], cols[ 0 ] + cinfo.output_width);


	        //memcpy( y, cols[ 0 ][ i ], cinfo.output_width );
*/
	  /*      for( int j = 0; j != cinfo.output_width; ++j, ++u )
	        {
	            //*u = cols[ 1 ][ i ][ j ];
	            Y->push_back(cols[ 0 ][ i ][ j ]);
	            Cb->push_back(cols[ 1 ][ i ][ j ]);
	            Cr->push_back(cols[ 2 ][ i ][ j ]);
	        }
	        */
/*
	    }

	}

	jpeg_finish_decompress( &cinfo );
	jpeg_destroy_decompress( &cinfo );
}

*/


int JpegCodec::decode_to_yuv(std::vector<char> *jpeg,
								int out_width,
								int out_height,
								std::vector<uint8_t> *Y,
								std::vector<uint8_t> *Cr,
								std::vector<uint8_t> *Cb)
{
    int n_samples;
    struct jpeg_error_mgr err;
    struct jpeg_decompress_struct cinfo = {0};
    JDIMENSION width, height;
    JSAMPARRAY      line;           /* Array of decomp data lines */
    unsigned char  *wline;          /* Will point to line[0] */
    int             linesize;

    /* create decompressor */
    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&err);
    cinfo.do_fancy_upsampling = FALSE;

    /* set source buffer */
    jpeg_mem_src(&cinfo, (unsigned char *)jpeg->data(), jpeg->size());

    /* read jpeg header */
    jpeg_read_header(&cinfo, (boolean)1);
    //cinfo.jpeg_color_space = JCS_RGB;
    width = cinfo.image_width;
    height = cinfo.image_height;

    if ((width != out_width) && (height != out_height))
    {
    	/// error format
    	return -1;
    };

    /* decompress */
    jpeg_start_decompress(&cinfo);

    switch (cinfo.jpeg_color_space)
    {
        case JCS_YCbCr:
          //mjpeg_info("YUV colorspace detected.\n");
          cinfo.out_color_space = JCS_YCbCr;
          linesize = cinfo.image_width * 3;
          break;
        case JCS_RGB:
          //mjpeg_info("YUV colorspace detected.\n");
          cinfo.out_color_space = JCS_RGB;
          linesize = cinfo.image_width * 3;
          break;
        case JCS_GRAYSCALE:
          //mjpeg_info("Grayscale colorspace detected.\n");
          cinfo.out_color_space = JCS_GRAYSCALE;
          break;
        default:
          //mjpeg_error("Unsupported colorspace detected.\n"); break;
        	break;
    };

    /* Allocate space for one line. */
    //line = (cinfo.mem->alloc_sarray)((j_common_ptr) cinfo, JPOOL_IMAGE,
    //                                   cinfo.output_width * cinfo.output_components, 1);
    line = (JSAMPARRAY)new (char *);
    line[0] = (JSAMPROW)new char[cinfo.output_width * cinfo.output_components];

    wline = line[0];
    /* read scanlines */
    while (cinfo.output_scanline < cinfo.output_height) {
        n_samples = jpeg_read_scanlines(&cinfo, line, 1);
        //samples += n_samples * cinfo.image_width * cinfo.num_components;
        for (int i = 0; i < linesize; i += 3) {
        	int zY, zCb, zCr;
            int R, G, B;
            zY =  wline[i];
            zCb = wline[i+1];
            zCr = wline[i+2];
            R = zY + 1,402*(zCr - 128);
            G = zY - 0,34414*(zCb-128) - 0,71414*(zCr - 128);
            B = zY - 1,772*(zCb-128);

            Y->push_back(wline[i]);
            //if (i & 1) {
                Cr->push_back(wline[i + 1]);
                Cb->push_back(wline[i + 2]);
            //}
                //Y++;
                //Cb++;
                //Cr++;



        }
    }

    /* clean up */
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

