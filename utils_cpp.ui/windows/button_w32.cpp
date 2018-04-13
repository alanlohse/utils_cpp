/*
 * button_w32.cpp
 *
 *  Created on: 23/01/2016
 *      Author: Alan
 */

#include <utils/ui/button.hpp>
#include "gdi_graphics.h"
#include <windows.h>
#include "utils_w32.h"

namespace utils {

namespace ui {

const char* Button::getClassName() const {
	return "BUTTON";
}

void Button::createButton(Container* parent, int left, int top, int width, int height) {

	createWindow(parent,0,"", parent ? WS_CHILD | WS_TABSTOP | WS_VISIBLE : WS_POPUP,
			left, top, width, height, &Component::componentWindowProc);

	setCursor(Cursor::getSystemCursor(SystemCursor::ARROW));
	setFont(GdiFont::createFontImpl("Sans Serif",Font::PLAIN,13));
	setBackground(new GdiColorBrush((HBRUSH)(COLOR_BTNFACE+1), false));
}

Button::Button(Container* parent) :	Component(), _image(NULL) {
	createButton(parent,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT);
}

Button::Button() : Component(), _image(NULL) {
	createButton(NULL,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT);
}

void Button::click() {
	PostMessageA(getHWnd(), WM_RBUTTONDOWN, MK_LBUTTON, 0);
	PostMessageA(getHWnd(), WM_RBUTTONUP, MK_LBUTTON, 0);
}

void ButtonUIController::paint(Component* component, Graphics* graphics) {
	GdiGraphics* g = dynamic_cast<GdiGraphics*>(graphics);
	RECT rc = {0,0,component->getSize().width,component->getSize().height};
	DrawFrameControl(g->hdc, &rc, DFC_BUTTON, DFCS_BUTTONPUSH);
	g->drawImage(dynamic_cast<Button*>(component)->_image,0,0);
}

} // ui

} // utils
