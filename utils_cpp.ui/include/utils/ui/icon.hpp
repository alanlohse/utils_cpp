/*
 * icon.hpp
 *
 *  Created on: 03/08/2014
 *      Author: Alan
 */

#ifndef _ICON_HPP_
#define _ICON_HPP_

#include <utils/ui/graphics.hpp>

namespace utils {

namespace ui {

class Icon : public UIObject {
protected:
	friend class Component;
	friend class Window;
	void* internal_data;
public:
	Icon(): internal_data() {}
	virtual ~Icon(){}
};

} // ui

} // utils

#endif /* _ICON_HPP_ */
