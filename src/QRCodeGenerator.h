//
//
//  Created on 11/27/2015
//  Based on Brett Renfer aka Robotconscience's ofxQRCodeGenerator
//  https://github.com/robotconscience/ofxQRCodeGenerator
//

#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/Gl.h"

#include "qrencode.h"

using namespace std;
using namespace ci;
using namespace ci::app;

class QRCodeGenerator {
public:

	/**
	 * Generate an Surface8u from a string
	 * @param str   the string you want to convert
	 * @param width target width + height of the image you want to output (will resize proportionally to returned data)
	 * @param caseSensive (optional) worry about converting the case. tends to return all uppercase if this is left false
	 * @param usePadding Adds white padding to the image returned
	 * @return Surface8u with QRCode
	 */

	static Surface8u generate( string str, int width, bool caseSensitive = false, bool usePadding = false ) {


		// try to decode
		QRcode * q = QRcode_encodeString( str.c_str(), 0, QR_ECLEVEL_M, QR_MODE_8, caseSensitive ? 1 : 0 );

		if ( q != NULL ) {
			int goodWidth = width;

			// resize up to something the returned data will fit into nicely
			while ( goodWidth % q->width != 0 ) {
				goodWidth++;
			}

			// letting you know if i resized your image (sorry!)
			if ( goodWidth != width ) console() << "Making your width fit the QR code: " << goodWidth << endl;
			width = goodWidth;

			int ratio = width / q->width;
			int padding = usePadding ? ratio : 0;

			Surface8u result = Surface8u( width + padding * 2, width + padding * 2, false );

			for ( int y = 0; y < q->width; y++ ) {
				for ( int x = 0; x < q->width; x++ ) {
					//get least significant byte of the returned usigned char
					// 1 == black, 0 == white
					// (check out qrencode for more info)
					int c = (int) q->data[y*q->width + x] & 1;

					// make squares based on that data!
					for ( int y1 = 0; y1 < ratio; y1++ ) {
						for ( int x1 = 0; x1 < ratio; x1++ ) {

							result.setPixel( ivec2( padding + ( x*ratio + x1 ), padding + ( y*ratio + y1 ) ), c == 1 ? ColorA::black() : ColorA::white() );
						}
					}
				}
			}

			// free to encode another code!
			QRcode_free( q );

			return result;

			// hm, maybe you sent something weird?
		} else {
			console() << "Failed to encode" << endl;
			return Surface8u();
		}
	}

};
