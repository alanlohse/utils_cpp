/*
 * icon.hpp
 *
 *  Created on: 03/08/2014
 *      Author: Alan
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <utils/ui/graphics.hpp>
#include <utils/ui/ui_allocator.hpp>
#include <istream>

namespace utils {

namespace ui {

class ImageType {
	MAKE_ENUMERATION(ImageType)
	DECLARE_CONSTANT(ImageType,MONOCROME);
	DECLARE_CONSTANT(ImageType,TYPE_4BIT);
	DECLARE_CONSTANT(ImageType,TYPE_8BIT);
	DECLARE_CONSTANT(ImageType,TYPE_16BIT);
	DECLARE_CONSTANT(ImageType,TYPE_24BIT);
	DECLARE_CONSTANT(ImageType,TYPE_32BIT);
};

class Image : public UIObject {
public:

	virtual ~Image() { }

	virtual bool isComplete() const = 0;

	virtual Size getSize() const = 0;
	virtual ImageType getBitsPerPixel() const = 0;

	virtual Graphics* getGraphics() = 0;

	virtual void readBits(void* bits, int left, int top, int right, int bottom, int scanWidth) const = 0;
	virtual void writeBits(const void* bits, int left, int top, int right, int bottom, int scanWidth) = 0;

	static Image* createImage(int width, int height, const ImageType& type);
	static Image* createCompatibleImage(int width, int height);
	static Image* createImage(Image* image);
	static void deleteImage(Image* image);

	static Image* loadImage(std::basic_istream<char>& input);
};

} // ui

} // utils

#endif /* IMAGE_HPP_ */
