/*
 * image_w32.cpp
 *
 *  Created on: 06/08/2014
 *      Author: Alan
 */

#include <utils/ui/image.hpp>
#include <utils/ui/ui.hpp>
#include <windows.h>
#include "int_datat.h"
#include "gdi_graphics.h"
#include "image_w32.h"

namespace utils {

namespace ui {

ui_allocator<Image_W32> imagew32_allocator;
extern int graphics_impl;

int type2bpp(const ImageType& type) {
	if (type == ImageType::MONOCROME) return 1;
	if (type == ImageType::TYPE_4BIT) return 4;
	if (type == ImageType::TYPE_8BIT) return 8;
	if (type == ImageType::TYPE_16BIT) return 16;
	if (type == ImageType::TYPE_24BIT) return 24;
	if (type == ImageType::TYPE_32BIT) return 32;
	return 24;
}

Image_W32* Image_W32::createImage(HBITMAP hbmp, bool _deletable) {
	return imagew32_allocator.allocate(Image_W32(hbmp,_deletable));
}
Image_W32* Image_W32::createImage(HBITMAP hbmp, HDC _hdc, bool _deletable, int _width, int _height, const ImageType& _type) {
	return imagew32_allocator.allocate(Image_W32(hbmp,_hdc,_deletable,_width,_height,_type));
}

Image_W32::Image_W32(HBITMAP hbmp, HDC _hdc, bool _deletable, int _width, int _height, const ImageType& _type): hBitmap(hbmp),
		hdc(_hdc),
		deletable(_deletable),
		width(_width), height(_height),
		type(_type) {
}

ImageType bpp2type(int bpp) {
	switch (bpp) {
		case 1:
			return ImageType::MONOCROME;
		case 4:
			return ImageType::TYPE_4BIT;
		case 8:
			return ImageType::TYPE_8BIT;
		case 16:
			return ImageType::TYPE_16BIT;
		case 24:
			return ImageType::TYPE_24BIT;
		case 32:
			return ImageType::TYPE_32BIT;
	}
			return ImageType::MONOCROME;
}

Image_W32::Image_W32(HBITMAP hbmp, bool _deletable) :
		hBitmap(),
		hdc(),
		deletable(),
		width(), height(),
		type(ImageType::TYPE_24BIT) {
	BITMAPINFO bm;
	memset(&bm,0,sizeof(BITMAPINFO));
	bm.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	HDC xdc = GetDC(NULL);
	char buf[1];
	::GetDIBits(xdc,hbmp,0,0,(void*)buf,&bm,DIB_RGB_COLORS);
	hBitmap = hbmp;
	width = bm.bmiHeader.biWidth;
	height = bm.bmiHeader.biHeight;
	type = bpp2type(bm.bmiHeader.biBitCount);
	ReleaseDC(NULL,xdc);
	deletable = false;
	hdc = NULL;
}
Image_W32::Image_W32(int _width, int _height) :
		hBitmap(),
		hdc(),
		deletable(true),
		width(), height(),
		type(ImageType::TYPE_24BIT) {
	HDC ddc = GetDC(NULL);
	hBitmap = CreateCompatibleBitmap(ddc,_width,_height);
	BITMAP bm;
	memset(&bm,0,sizeof(bm));
	GetObject(hBitmap,sizeof(bm),&bm);
	type = bpp2type(bm.bmBitsPixel);
	width = bm.bmWidth;
	height = bm.bmHeight;
	ReleaseDC(NULL,ddc);
}

Image_W32::Image_W32(int _width, int _height, const ImageType& _type): hBitmap(NULL),
		hdc(NULL),
		deletable(true),
		width(_width), height(_height),
		type(_type) {
	BITMAP bmp;
	bmp.bmType = 0;
	bmp.bmWidth = _width;
	bmp.bmHeight = _height;
	bmp.bmBitsPixel = type2bpp(_type);
	bmp.bmPlanes = 1;
	int bpp = bmp.bmBitsPixel/8;
	if (bpp == 0) bpp = 1;
	bmp.bmWidthBytes = bpp*_width;
	bmp.bmBits = NULL;
	hBitmap = CreateBitmapIndirect(&bmp);
}
Image_W32::Image_W32(Image* other): hBitmap(),
		hdc(NULL),
		deletable(true),
		width(), height(),
		type(ImageType::TYPE_24BIT) {
	Image_W32* w32img = dynamic_cast<Image_W32*>(other);
	if (w32img) {
		width = w32img->width;
		height = w32img->height;
		type = w32img->type;
		// TODO hBitmap =
	}
}

Image_W32::~Image_W32() {
	if (hdc != NULL)
		DeleteDC(hdc);
	if (deletable)
		DeleteObject(hBitmap);
}

bool Image_W32::isComplete() const {
	return true;
}

Size Image_W32::getSize() const {
	return Size(width,height);
}
ImageType Image_W32::getBitsPerPixel() const {
	return type;
}

Graphics* Image_W32::getGraphics() {
	if (graphics_impl == GRAPHICS_GDI) {
		if (hdc == NULL) {
			hdc = CreateCompatibleDC(NULL);
			SelectObject(hdc,hBitmap);
		}
		return GdiGraphics::createGraphics(hdc,true);
	}
	return NULL;
}

void Image_W32::readBits(void* bits, int left, int top, int right, int bottom, int scanWidth) const {
	// TODO
/*
    HDC dcBitmap = CreateCompatibleDC ( NULL );
    SelectObject( dcBitmap, hBitmap );

    BITMAPINFO bmpInfo;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = bm.bmWidth;
    bmpInfo.bmiHeader.biHeight = -bm.bmHeight;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = 0;

    COLORREF* pixel = new COLORREF [ bm.bmWidth * bm.bmHeight ];
    GetDIBits( dcBitmap , hBitmap , 0 , bm.bmHeight , pixel , &bmpInfo , DIB_RGB_COLORS );
 */
}
void Image_W32::writeBits(const void* bits, int left, int top, int right, int bottom, int scanWidth) {
	// TODO
}


// -------------------------------------------------------------------------------------------------------------------
Image* Image::createImage(int width, int height, const ImageType& type) {
	return imagew32_allocator.allocate(Image_W32(width,height,type));
}
Image* Image::createCompatibleImage(int width, int height) {
	return imagew32_allocator.allocate(Image_W32(width,height));
}
Image* Image::createImage(Image* image) {
	return imagew32_allocator.allocate(Image_W32(image));
}
void Image::deleteImage(Image* image) {
	SAFE_DELETE(image);
}

static char BITMAP_MAGIC[] = "BM";
static char PNG_MAGIC[] = {(char)137, 80, 78, 71, 13, 10, 26, 10};
static char JPEG_MAGIC[] = {(char)0xff, (char)0xd8, (char)0xff, (char)0xe0};

Image* loadBitmap(std::basic_istream<char>& input);
Image* loadJpeg(std::basic_istream<char>& input);
Image* loadPng(std::basic_istream<char>& input);

Image* Image::loadImage(std::basic_istream<char>& input) {
/*
 * BMP - BM
 * PNG - 137  80  78  71  13  10  26  10 - http://www.w3.org/TR/PNG/
 * JPEG - FF D8 FF E0 - http://www.ecma-international.org/publications/files/ECMA-TR/TR-098.pdf
 */

	char magic_bytes[10];

	if (!input.read(magic_bytes,2)) return NULL;

	if (strncmp(BITMAP_MAGIC,magic_bytes,2) == 0) {
		return loadBitmap(input);
	}

	if (!input.read(magic_bytes+2,2)) return NULL;

	if (strncmp(JPEG_MAGIC,magic_bytes,4) == 0) {
		return loadJpeg(input);
	}

	if (!input.read(magic_bytes+4,4)) return NULL;

	if (strncmp(PNG_MAGIC,magic_bytes,8) == 0) {
		return loadPng(input);
	}

	return NULL;

}

Image* loadBitmap(std::basic_istream<char>& input) {


	BITMAPFILEHEADER bmfHeader;

    bmfHeader.bfType = 0x4D42; //BM
    int headerSize = sizeof(BITMAPFILEHEADER)-2;
	if (!input.read((char*)&bmfHeader + 2,headerSize))
		return NULL;

	BITMAPINFOHEADER bih;
	if (!input.read((char*)&bih,sizeof(BITMAPINFOHEADER)))
		return NULL;

	int dwBmpSize = bih.biCompression == BI_RGB ? ((bih.biWidth * bih.biBitCount + 31) / 32) * 4 * bih.biHeight :
			bih.biSizeImage;
	int palSize = (1 << bih.biBitCount)*sizeof(RGBQUAD);
	int binfoSize = sizeof(BITMAPINFO) + (bih.biBitCount == 8 || bih.biBitCount == 4?
			((1 << bih.biBitCount) - 1)*sizeof(RGBQUAD) : 0);
	BITMAPINFO* binfo = (BITMAPINFO*) getUIAllocator()->malloc(binfoSize);

	if (bih.biBitCount == 8 || bih.biBitCount == 4) {
		if (!input.read((char*)&binfo->bmiColors[0],palSize)) {
			getUIAllocator()->free(binfo,binfoSize);
			return NULL;
		}
	}

	char* buf = (char*) getUIAllocator()->malloc(dwBmpSize);

	if (!input.read(buf,dwBmpSize)) {
		getUIAllocator()->free(binfo,binfoSize);
		getUIAllocator()->free(buf,dwBmpSize);
		return NULL;
	}

	binfo->bmiHeader = bih;
	HDC hdc = CreateCompatibleDC(NULL);
	HBITMAP hbmp = CreateDIBitmap(hdc,&bih,CBM_INIT,(void*)buf,binfo,bih.biBitCount == 8  || bih.biBitCount == 4 ? DIB_PAL_COLORS : DIB_RGB_COLORS);
	getUIAllocator()->free(binfo,binfoSize);
	getUIAllocator()->free(buf,dwBmpSize);
	DeleteDC(hdc);
	return Image_W32::createImage(hbmp,NULL,true,bih.biWidth,bih.biHeight,bpp2type(bih.biBitCount));
}
Image* loadJpeg(std::basic_istream<char>& input) {
	// TODO loadJpeg
	return NULL;
}
Image* loadPng(std::basic_istream<char>& input) {
	// TODO loadPng
	return NULL;
}


}

}
