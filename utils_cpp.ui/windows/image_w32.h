/*
 * image_w32.h
 *
 *  Created on: 20/04/2015
 *      Author: Alan
 */

#ifndef IMAGE_W32_H_
#define IMAGE_W32_H_

#include <utils/ui/ui.hpp>
#include <utils/ui/image.hpp>
#include <windows.h>
#include "int_datat.h"

namespace utils {
namespace ui {

class Image_W32 : public Image {
public:
	HBITMAP hBitmap;
	HDC hdc;
	bool deletable;
	int width, height;
	ImageType type;

	Image_W32(HBITMAP hbmp, HDC _hdc, bool _deletable, int _width, int _height, const ImageType& _type);
	Image_W32(HBITMAP hbmp, bool _deletable);
	Image_W32(int _width, int _height, const ImageType& _type);
	Image_W32(int _width, int _height);
	Image_W32(Image* other);

	virtual ~Image_W32();

	virtual bool isComplete() const;

	virtual Size getSize() const;
	virtual ImageType getBitsPerPixel() const;

	virtual Graphics* getGraphics();

	virtual void readBits(void* bits, int left, int top, int right, int bottom, int scanWidth) const;
	virtual void writeBits(const void* bits, int left, int top, int right, int bottom, int scanWidth);

	static Image_W32* createImage(HBITMAP hbmp, bool _deletable);
	static Image_W32* createImage(HBITMAP hbmp, HDC _hdc, bool _deletable, int _width, int _height, const ImageType& _type);
};

}

}

#endif /* IMAGE_W32_H_ */
