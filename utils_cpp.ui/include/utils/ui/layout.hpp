/*
 * layout.hpp
 *
 *  Created on: 26/04/2015
 *      Author: Alan
 */

#ifndef LAYOUT_HPP_
#define LAYOUT_HPP_

#include <utils/ui/component.hpp>

namespace utils {

namespace ui {

class Layout : public UIObject {
public:
	virtual ~Layout() { }

	virtual void apply(Container* container) = 0;

	virtual Size getMinimumSize(const Container* container) const = 0;

	virtual bool checkConstraints(const Container* container,const Component* child, int constraint);
};

class BasicLayout : public Layout {
private:
	int _vpadding,_hpadding;
public:
	BasicLayout(): _vpadding(),_hpadding() { }
	BasicLayout(int vpadding, int hpadding): _vpadding(vpadding),_hpadding(hpadding) { }
	virtual ~BasicLayout() { }

	virtual void apply(Container* container);

	virtual Size getMinimumSize(const Container* container) const;

	int vpadding() const {
		return _vpadding;
	}
	int hpadding() const {
		return _hpadding;
	}

	void setVpadding(int vpadding) {
		_vpadding = vpadding;
	}
	void setHpadding(int hpadding) {
		_hpadding = hpadding;
	}

	MAKE_CONSTANT(int,CENTER,0);
	MAKE_CONSTANT(int,TOP,1);
	MAKE_CONSTANT(int,BOTTOM,2);
	MAKE_CONSTANT(int,LEFT,3);
	MAKE_CONSTANT(int,RIGHT,4);
};

} // ui

} // utils


#endif /* LAYOUT_HPP_ */
