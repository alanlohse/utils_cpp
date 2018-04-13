/*
 * gdi_GdiGraphics.cpp
 *
 *  Created on: 07/08/2014
 *      Author: Alan
 */

#include <utils/ui/ui.hpp>
#include <utils/ui/image.hpp>
#include "gdi_graphics.h"
#include <windows.h>

namespace utils {

namespace ui {

t_dword compOper2ROP2(const CompositeOperation& go) {
	if (CompositeOperation::SOURCE_COPY == go) return R2_COPYPEN;
	if (CompositeOperation::SOURCE_AND == go) return R2_MASKPEN;
	if (CompositeOperation::SOURCE_OR == go) return R2_MERGEPEN;
	if (CompositeOperation::SOURCE_XOR == go) return R2_XORPEN;
	if (CompositeOperation::DEST_INVERT == go) return R2_NOT;
	if (CompositeOperation::WHITE == go) return R2_WHITE;
	if (CompositeOperation::BLACK == go) return R2_BLACK;
	return R2_COPYPEN;
}

const CompositeOperation& ROP2CompOper(t_dword rop2) {
	if (R2_COPYPEN == rop2) return CompositeOperation::SOURCE_COPY;
	if (R2_MASKPEN == rop2) return CompositeOperation::SOURCE_AND;
	if (R2_MERGEPEN == rop2) return CompositeOperation::SOURCE_OR;
	if (R2_XORPEN == rop2) return CompositeOperation::SOURCE_XOR;
	if (R2_NOT == rop2) return CompositeOperation::DEST_INVERT;
	if (R2_WHITE == rop2) return CompositeOperation::WHITE;
	if (R2_BLACK == rop2) return CompositeOperation::BLACK;
	return CompositeOperation::SOURCE_COPY;
}

t_dword compOper2BitBlt(const CompositeOperation& go) {
	if (CompositeOperation::SOURCE_COPY == go) return SRCCOPY;
	if (CompositeOperation::SOURCE_AND == go) return SRCAND;
	if (CompositeOperation::SOURCE_OR == go) return SRCPAINT;
	if (CompositeOperation::SOURCE_XOR == go) return SRCINVERT;
	if (CompositeOperation::DEST_INVERT == go) return DSTINVERT;
	if (CompositeOperation::WHITE == go) return WHITENESS;
	if (CompositeOperation::BLACK == go) return BLACKNESS;
	return SRCCOPY;
}

const CompositeOperation& bitBlt2CompOper(t_dword blt) {
	if (SRCCOPY == blt) return CompositeOperation::SOURCE_COPY;
	if (SRCAND == blt) return CompositeOperation::SOURCE_AND;
	if (SRCPAINT == blt) return CompositeOperation::SOURCE_OR;
	if (SRCINVERT == blt) return CompositeOperation::SOURCE_XOR;
	if (DSTINVERT == blt) return CompositeOperation::DEST_INVERT;
	if (WHITENESS == blt) return CompositeOperation::WHITE;
	if (BLACKNESS == blt) return CompositeOperation::BLACK;
	return CompositeOperation::SOURCE_COPY;
}

GdiGraphics* GdiGraphics::createGraphics(HDC _hdc, bool _compatible) {
	return new GdiGraphics(_hdc,_compatible);
}
void GdiGraphics::deleteGraphics(GdiGraphics* _graphics) {
	delete _graphics;
}

GdiGraphics::GdiGraphics(HDC _hdc, bool _compatible) :
		hdc(_hdc),
		compatible(_compatible),
		world_matrix(WorldMatrix::identity()),
		matrix_stack(1),
		currentFont(NULL),
		currentPattern(NULL),
		currentStroke(NULL),
		globalCompositeOperation(CompositeOperation::SOURCE_COPY),
		alpha(1),
		pathOpen(false)
		{
	SetGraphicsMode(hdc,GM_ADVANCED);
	currentFont = GdiFont::fromHDC(hdc);
	globalCompositeOperation = ROP2CompOper(GetROP2(hdc));
	lineColor = GetDCPenColor(hdc);
	fillColor = GetDCBrushColor(hdc);
	textColor = GetTextColor(hdc);
}

GdiGraphics::~GdiGraphics() {
	if (compatible && get_references() < 1) {
		DeleteDC(hdc);
	}
}

void GdiGraphics::save() {
	matrix_stack.push_back(world_matrix);
}
void GdiGraphics::restore() {
	world_matrix = matrix_stack.back();
	matrix_stack.pop_back();
	commit_world_matrix();
}

void GdiGraphics::commit_world_matrix() {
	const float* _m = world_matrix.c_matrix();
	XFORM xf;
	xf.eM11 = _m[0];
	xf.eM12 = _m[1];
	xf.eM21 = _m[3];
	xf.eM22 = _m[4];
	xf.eDx = _m[2];
	xf.eDy = _m[5];
	SetWorldTransform(hdc,&xf);
}

void GdiGraphics::scale(float x, float y) {
	world_matrix *= WorldMatrix(x, 0, 0, 0, y, 0, 0, 0, 1);
	commit_world_matrix();
}
void GdiGraphics::rotate(float angle) {
	world_matrix = world_matrix * WorldMatrix::rotation(0,0,angle);
	commit_world_matrix();
}
void GdiGraphics::translate(float x, float y) {
	world_matrix[0][2] += x;
	world_matrix[1][2] += y;
	commit_world_matrix();
}
void GdiGraphics::transform(float a, float b, float c, float d, float e, float f) {
	world_matrix *= WorldMatrix(a, c, e, b, d, f, 0, 0, 1);
	commit_world_matrix();
}
void GdiGraphics::setTransform(float a, float b, float c, float d, float e, float f) {
	world_matrix = WorldMatrix(a, c, e, b, d, f, 0, 0, 1);
	commit_world_matrix();
}


Gradient* GdiGraphics::createLinearGradient(float x0, float y0, float x1, float y1) {
	// TODO createLinearGradient
	return NULL;
}
Gradient* GdiGraphics::createRadialGradient(float x0, float y0, float r0, float x1, float y1, float r1) {
	// TODO createRadialGradient
	return NULL;
}
Pattern* GdiGraphics::createPattern(Image* image, long repetition) {
	return new GdiPattern(image,repetition);
}

Stroke* GdiGraphics::createStroke(const LineStyle& style, const LineJoin& join, const LineCap& cap, float width) {
	return new GdiStroke(style, join, cap, width, lineColor);
}

float GdiGraphics::getGlobalAlpha() {
	return alpha;
}
void GdiGraphics::setGlobalAlpha(float value) {
	alpha = value;
}

CompositeOperation GdiGraphics::getGlobalCompositeOperation() {
	return globalCompositeOperation;
}

void GdiGraphics::setGlobalCompositeOperation(const CompositeOperation& go) {
	if (SetROP2(hdc, compOper2ROP2(go)) != 0)
		globalCompositeOperation = go;
}

Color GdiGraphics::getLineColor() {
	return lineColor;
}
void GdiGraphics::setLineColor(const Color& value) {
//	if (SetDCPenColor(hdc,value.get_rgb()) != CLR_INVALID)
	lineColor = value;
}

Color GdiGraphics::getFillColor() {
	return fillColor;
}
void GdiGraphics::setFillColor(const Color& value) {
//	if (SetDCBrushColor(hdc,value.get_rgb()) != CLR_INVALID)
	SAFE_DELETE(currentPattern);
	setFillStyle(new GdiColorBrush(value));
	fillColor = value;
}

Color GdiGraphics::getTextColor() {
	return textColor;
}
void GdiGraphics::setTextColor(const Color& value) {
	if (SetTextColor(hdc, value.get_rgb()) != CLR_INVALID)
		textColor = value;
}

FillStyle* GdiGraphics::getFillStyle() {
	return currentPattern;
}
void GdiGraphics::setFillStyle(FillStyle* value) {
	SAFE_DELETE(currentPattern);
	currentPattern = value;
	value->add_reference();
}

Stroke* GdiGraphics::getStroke() {
	return currentStroke;
}
void GdiGraphics::setStroke(Stroke* value) {
	currentStroke = value;
}



void GdiGraphics::clearRect(float x, float y, float w, float h) {
	// TODO not implemented on GDI. But its possible.
}
void GdiGraphics::fillRect(float x, float y, float w, float h) {
	if (currentPattern) {
		if (currentPattern->isGradient()) {
			// TODO fill with gradient
		} else {
			RECT rc = {x,y,x+w,y+h};
			FillRect(hdc,&rc,(HBRUSH)dynamic_cast<GdiObject*>(currentPattern)->getHandle());
		}
	}
}
void GdiGraphics::strokeRect(float x, float y, float w, float h) {
	HBRUSH hbrNull = (HBRUSH) GetStockObject(NULL_BRUSH);
	HBRUSH hbrPrev = (HBRUSH) SelectObject(hdc, hbrNull);
	HPEN oldPen = NULL;
	if (currentStroke) {
		HPEN pen = static_cast<GdiStroke*>(currentStroke)->hpen;
		oldPen = (HPEN) SelectObject(hdc, pen);
//		SetDCPenColor(hdc,lineColor.get_rgb());
	}
	Rectangle(hdc, x, y, x + w, y + h);
	if (oldPen) {
		 SelectObject(hdc, oldPen);
	}
	SelectObject(hdc, hbrPrev);
}


void GdiGraphics::beginPath() {
	BeginPath(hdc);
	pathOpen = true;
}
void GdiGraphics::closePath() {
	CloseFigure(hdc);
	if (pathOpen) {
		EndPath(hdc);
		pathOpen = false;
	}
}
void GdiGraphics::fill() {
	if (pathOpen) {
		EndPath(hdc);
		pathOpen = false;
	}
	FillPath(hdc);
}
void GdiGraphics::stroke() {
	if (pathOpen) {
		EndPath(hdc);
		pathOpen = false;
	}
	GdiStroke* strk = dynamic_cast<GdiStroke*>(currentStroke);
	HGDIOBJ old = SelectObject(hdc, strk->hpen);
//	SelectObject(hdc, GetStockObject(DC_PEN));
//	SetDCBrushColor(hdc,lineColor.get_rgb());
//	SetDCPenColor(hdc,RGB(255,0,0));
	StrokePath(hdc);
	SelectObject(hdc, old);
}
void GdiGraphics::strokeAndFill() {
	if (pathOpen) {
		EndPath(hdc);
		pathOpen = false;
	}
	StrokeAndFillPath(hdc);
}
void GdiGraphics::clip() {
	if (pathOpen) {
		EndPath(hdc);
		pathOpen = false;
	}
	SelectClipPath(hdc, RGN_COPY);
}

void GdiGraphics::moveTo(float x, float y) {
	MoveToEx(hdc, x, y, NULL);
}
void GdiGraphics::lineTo(float x, float y) {
	LineTo(hdc, x, y);
}
void GdiGraphics::quadraticCurveTo(float cpx, float cpy, float x, float y) {
	POINT pt;
	GetCurrentPositionEx(hdc, &pt);
	POINT pts[] {
			{(cpx*2 + pt.x)/2, (cpy*2 + pt.y)/2},
			{(cpx*2 + x)/2, (cpy*2 + y)/2},
			{x, y}
	};
	PolyBezierTo(hdc,pts,3);
}
void GdiGraphics::bezierCurveTo(float cp1x, float cp1y, float cp2x, float cp2y, float x, float y) {
	POINT pts[] {
			{cp1x, cp1y},
			{cp2x, cp2y},
			{x, y}
	};
	PolyBezierTo(hdc,pts,3);
}
void GdiGraphics::arcTo(float x1, float y1, float x2, float y2, float radius) {

//	ArcTo(hdc,x1,y2,x2,y2,
}
void GdiGraphics::rect(float x, float y, float w, float h) {
	Rectangle(hdc, x, y, x + w, y + h);
}
void GdiGraphics::arc(float x, float y, float radius, float startAngle, float endAngle, bool anticlockwise = false) {
	AngleArc(hdc, x, y, radius, startAngle, (anticlockwise ? 1 : -1)*(endAngle - startAngle));
}


Font* GdiGraphics::getFont() {
	return currentFont;
}
void GdiGraphics::setFont(Font* font) {
	GdiFont* old = currentFont;
	currentFont = dynamic_cast<GdiFont*>(font);
	if (currentFont) {
		if (old->rem_reference()->get_references() < 1) delete old;
		currentFont->add_reference();
		SelectObject(hdc,currentFont->hfont);
	} else {
		currentFont = old;
	}
}
void GdiGraphics::fillText(const char* text, float x, float y, float maxWidth) {
	BeginPath(hdc);
	TextOutA(hdc,x,y,text,strlen(text));
	EndPath(hdc);
	FillPath(hdc);
}
void GdiGraphics::strokeText(const char* text, float x, float y, float maxWidth) {
	BeginPath(hdc);
	TextOutA(hdc,x,y,text,strlen(text));
	EndPath(hdc);
	StrokePath(hdc);
}
void GdiGraphics::fillText(const wchar_t* text, float x, float y, float maxWidth) {
	BeginPath(hdc);
	TextOutW(hdc,x,y,text,wcslen(text));
	EndPath(hdc);
	FillPath(hdc);
}
void GdiGraphics::strokeText(const wchar_t* text, float x, float y, float maxWidth) {
	BeginPath(hdc);
	TextOutW(hdc,x,y,text,wcslen(text));
	EndPath(hdc);
	StrokePath(hdc);
}
float GdiGraphics::measureText(const char* text) {
	SIZE size;
	GetTextExtentPoint32A(hdc,text,strlen(text),&size);
	return size.cx;
}
float GdiGraphics::measureText(const wchar_t* text) {
	SIZE size;
	GetTextExtentPoint32W(hdc,text,wcslen(text),&size);
	return size.cx;
}


void GdiGraphics::drawImage(Image* image, float dx, float dy) {
	GdiGraphics* imgg = dynamic_cast<GdiGraphics*>(image->getGraphics());
	BitBlt(hdc,dx,dy,image->getSize().width,image->getSize().height,imgg->hdc,0,0,SRCCOPY);
}
void GdiGraphics::drawImage(Image* image, float dx, float dy, float dw, float dh) {
	GdiGraphics* imgg = dynamic_cast<GdiGraphics*>(image->getGraphics());
	BitBlt(hdc,dx,dy,dw,dh,imgg->hdc,0,0,SRCCOPY);
}
void GdiGraphics::drawImage(Image* image, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) {
	GdiGraphics* imgg = dynamic_cast<GdiGraphics*>(image->getGraphics());
	StretchBlt(hdc,dx,dy,dw,dh,imgg->hdc,sx,sy,sw,sh,SRCCOPY);
}


Image* GdiGraphics::createImage(float sw, float sh) { return NULL; }
Image* GdiGraphics::createImage(Image* imagedata) { return NULL; }
Image* GdiGraphics::getImage(float sx, float sy, float sw, float sh) { return NULL; }

/**********************************************************************************************************
 *
 * @param _hpen
 **********************************************************************************************************/

DWORD lineStyle2PenStyle(const LineStyle& _style) {
	if (_style == LineStyle::SOLID) return PS_SOLID;
	if (_style == LineStyle::DASH) return PS_DASH;
	if (_style == LineStyle::DOT) return PS_DOT;
	if (_style == LineStyle::DASHDOT) return PS_DASHDOT;
	if (_style == LineStyle::DASHDOTDOT) return PS_DASHDOTDOT;
	if (_style == LineStyle::NULL_LINE) return PS_NULL;
	return 0;
}

DWORD lineJoin2PenStyle(const LineJoin& _join) {
	if (_join == LineJoin::BEVEL) return PS_JOIN_BEVEL;
	if (_join == LineJoin::MITER) return PS_JOIN_MITER;
	if (_join == LineJoin::ROUND) return PS_JOIN_ROUND;
	return 0;
}
DWORD lineCap2PenStyle(const LineCap& _cap) {
	if (_cap == LineCap::ROUND) return PS_ENDCAP_ROUND;
	if (_cap == LineCap::SQUARE) return PS_ENDCAP_SQUARE;
	if (_cap == LineCap::FLAT) return PS_ENDCAP_FLAT;
	return 0;
}

const LineStyle& penStyle2LineStyle(const DWORD _style) {
	switch(_style) {
	case PS_SOLID: return LineStyle::SOLID;
	case PS_DASH: return LineStyle::DASH;
	case PS_DOT: return LineStyle::DOT;
	case PS_DASHDOT: return LineStyle::DASHDOT;
	case PS_DASHDOTDOT: return LineStyle::DASHDOTDOT;
	case PS_NULL: return LineStyle::NULL_LINE;
	}
	throw utils_exception("Invalid style.");
}

const LineJoin& penStyle2LineJoin(const DWORD _join) {
	switch(_join) {
	case PS_JOIN_BEVEL: return LineJoin::BEVEL;
	case PS_JOIN_MITER: return LineJoin::MITER;
	case PS_JOIN_ROUND: return LineJoin::ROUND;
	}
	throw utils_exception("Invalid style.");
}
const LineCap& penStyle2LineCap(const DWORD _cap) {
	switch(_cap) {
	case PS_ENDCAP_ROUND: return LineCap::ROUND;
	case PS_ENDCAP_SQUARE: return LineCap::SQUARE;
	case PS_ENDCAP_FLAT: return LineCap::FLAT;
	}
	throw utils_exception("Invalid style.");
}

GdiStroke::GdiStroke(HPEN _hpen) : hpen(_hpen), deletable(false) {
	LOGPEN lp;
	EXTLOGPEN elp;
	if (GetObject(hpen, sizeof(LOGPEN), &lp)) {
		style = lp.lopnStyle;
		width = MAX(lp.lopnWidth.x,lp.lopnWidth.y);
	} else if (GetObject(hpen, sizeof(EXTLOGPEN), &elp)) {
		style = elp.elpPenStyle;
		width = elp.elpWidth;
	}
}
GdiStroke::GdiStroke(const LineStyle& _style, const LineJoin& _join, const LineCap& _cap, float _width, const Color& color) : deletable(true) {
	style = PS_GEOMETRIC |
			lineStyle2PenStyle(_style) |
			lineJoin2PenStyle(_join) |
			lineCap2PenStyle(_cap);
	width = _width;
	LOGBRUSH lb;
	lb.lbColor = RGB(color.get_red(), color.get_green(), color.get_blue());
	lb.lbHatch = BS_SOLID;
	lb.lbStyle = 0;
	hpen = ExtCreatePen(style,
			width,
			&lb,0,NULL);
//	LOGPEN lp;
//	lp.lopnWidth.x = _width;
//	lp.lopnWidth.y = _width;
//	lp.lopnColor = RGB(color.get_red(), color.get_green(), color.get_blue());
//	lp.lopnStyle = PS_SOLID;
//	hpen = CreatePenIndirect(&lp);
}
GdiStroke::~GdiStroke() {
	if (deletable) {
		DeleteObject(hpen);
	}
}

LineStyle GdiStroke::getStyle() const {
	return penStyle2LineStyle(style & PS_STYLE_MASK);
}
LineJoin GdiStroke::getJoin() const {
	return penStyle2LineJoin(style & PS_JOIN_MASK);
}
LineCap GdiStroke::getCap() const {
	return penStyle2LineCap(style & PS_ENDCAP_MASK);
}
float GdiStroke::getWidth() const {
	return width;
}

/***************************************************************************************************************
 * GdiFont
 ***************************************************************************************************************/

GdiFont::GdiFont(const char* facename, int _size, int _style) : size(_size), style(_style), deletable(true) {
	LOGFONTA lf = { size, 0, 0, 0, ((style & BOLD) == BOLD ? FW_BOLD : FW_DONTCARE),
			(style & ITALIC) == ITALIC, (style & UNDERLINE) == UNDERLINE,
			(style & STRIKE_OUT) == STRIKE_OUT, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE};
	strncpy(lf.lfFaceName, facename, 31);
	strncpy(fontface, facename, 31);
	hfont = CreateFontIndirectA(&lf);
}
GdiFont::GdiFont(HFONT _hfont) {
	hfont = _hfont;
	LOGFONT lf;
	GetObject(hfont,sizeof(LOGFONT), &lf);
	size = lf.lfHeight;
	style = 0;
	style |= lf.lfUnderline ? Font::UNDERLINE : 0;
	style |= lf.lfStrikeOut ? Font::STRIKE_OUT : 0;
	style |= lf.lfItalic ? Font::ITALIC : 0;
	style |= lf.lfWeight >= FW_BOLD ? Font::BOLD : 0;
	deletable = false;
}
GdiFont::~GdiFont() {
	if (deletable)
		DeleteObject(hfont);
}

const char* GdiFont::getFace() const {
	return fontface;
}
int GdiFont::getSize() const {
	return size;
}
int GdiFont::getStyle() const {
	return style;
}

GdiFont* GdiFont::fromHDC(HDC hdc) {
	HFONT hfont = (HFONT)SelectObject(hdc, (HFONT)GetStockObject(SYSTEM_FONT));
	SelectObject(hdc, hfont);
	return new GdiFont(hfont);
}
GdiFont* GdiFont::createFontImpl(const char* facename, int style, int size) {
	return new GdiFont(facename, size, style);
}
void GdiFont::destroyFontImpl(GdiFont* obj) {
	delete obj;
}


/******************************************************************************************************************
 * GdiPattern
 ******************************************************************************************************************/
//HBRUSH hbrush;
//Image* image;
//bool deletale;
//int repeat;
GdiPattern::GdiPattern(HBRUSH _hbrush) : hbrush(_hbrush), image(NULL), deletale(false), repeat(0) {
	// TODO load data from hbrush
}
GdiPattern::GdiPattern(Image* _image, int _repeat) : hbrush(NULL), image(NULL), deletale(false), repeat(_repeat) {
	// TODO create hbrush from image
}
GdiPattern::~GdiPattern() {
	// TODO delete data
}

Image* GdiPattern::getImage() {
	return image;
}

int GdiPattern::getRepeat() const {
	return repeat;
}


//HBRUSH hbrush;
//bool deletable;
GdiColorBrush::GdiColorBrush(const Color& color) : hbrush(), deletable(true) {
	hbrush = CreateSolidBrush(RGB(color.get_red(),color.get_green(),color.get_blue()));
}
GdiColorBrush::GdiColorBrush(HBRUSH _hbrush, bool _deletable) : hbrush(_hbrush), deletable(_deletable) {
}
GdiColorBrush::~GdiColorBrush() {
	if (deletable)
		DeleteObject(hbrush);
}

Color GdiColorBrush::getColor() const {
	LOGBRUSH lb;
	GetObject(hbrush,sizeof(LOGBRUSH),&lb);
	return Color(lb.lbColor);
}


}

}



