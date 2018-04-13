/*
 * graphics.cpp
 *
 *  Created on: 07/08/2014
 *      Author: Alan
 */

#include <utils/ui/ui.hpp>
#include <utils/ui/image.hpp>

namespace utils {

namespace ui {

int graphics_impl = 0;

void Graphics::setGraphicsImplementation(int graphicsImpl) {
	graphics_impl = graphicsImpl;
}
int Graphics::getGraphicsImplementation() {
	return graphics_impl;
}

}

}
