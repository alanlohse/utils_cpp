/*
 * graphics_w32.cpp
 *
 *  Created on: 07/08/2014
 *      Author: Alan
 */

#include <utils/ui/ui.hpp>
#include <utils/ui/image.hpp>
#include <windows.h>
#include "gdi_graphics.h"

namespace utils {

namespace ui {

extern int graphics_impl;

Font* Font::createFont(const char* facename, int style, int size) {

	if (graphics_impl == GRAPHICS_GDIPLUS)
		return NULL;

	return GdiFont::createFontImpl(facename, style, size);
}

void Font::destroyFont(Font* obj) {
	GdiFont::destroyFontImpl(static_cast<GdiFont*>(obj));
}

}

}
