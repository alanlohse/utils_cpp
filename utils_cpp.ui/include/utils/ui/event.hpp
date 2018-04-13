/*
 * event.hpp
 *
 *  Created on: 03/08/2014
 *      Author: Alan
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <utils/utils_defs.hpp>
#include <utils/ui/component.hpp>

namespace utils {
namespace ui {

class Window;

// ----------------------------------------------------------------------------------------------------------------------------
// Basic Event

class Event {
private:
	friend class UIController;
	Component* _source;
	t_bigint _time;
	int _event_id;
	bool _prevent_default;
public:
	Event(Component* source, t_bigint time, int event_id) : _source(source), _time(time), _event_id(event_id), _prevent_default(false) { }
	Event(const Event& other) : _source(other._source), _time(other._time), _event_id(other._event_id), _prevent_default(false) { }
	virtual ~Event(){}
	t_bigint time() const { return _time; }
	int id() const { return _event_id; }
	Component* source() const { return _source; }

	void preventDefault() {
		_prevent_default = true;
	}

	bool defaultCanceled() const {
		return _prevent_default;
	}

};

struct EventListener {

	virtual ~EventListener(){}
	virtual void operator() (Event* evt) {
	}

};

// ---------------------------------------------------------------------------------------------------------------------------
// Input Events

class InputEvent : public Event {
public:

	virtual ~InputEvent() { }

	MAKE_CONSTANT(int,CONTROL,0x0008);
	MAKE_CONSTANT(int,SHIFT,0x0004);
	MAKE_CONSTANT(int,MOUSE_BUTTON1,0x0001);
	MAKE_CONSTANT(int,MOUSE_BUTTON2,0x0002);
	MAKE_CONSTANT(int,MOUSE_MBUTTON,0x0010);
	MAKE_CONSTANT(int,MOUSE_XBUTTON1,0x0020);
	MAKE_CONSTANT(int,MOUSE_XBUTTON2,0x0040);

};


// ---------------------------------------------------------------------------------------------------------------------------
// Mouse Events

class MouseEvent : public Event {
private:
	int _x, _y, _button, _clicks;
	int _shift_state;
public:
	MouseEvent(Component* source, t_bigint time, int event_id, int x, int y, int button, int clicks, int shift_state) :
		Event(source, time, event_id),
		_x(x), _y(y), _button(button), _clicks(clicks), _shift_state(shift_state) { }
	MouseEvent(Component* source, t_bigint time, int event_id, int x, int y, int shift_state) :
		Event(source, time, event_id),
		_x(x), _y(y), _button(0), _clicks(0), _shift_state(shift_state) { }
	MouseEvent(Component* source, t_bigint time, int event_id, int x, int y, int button, int shift_state) :
		Event(source, time, event_id),
		_x(x), _y(y), _button(button), _clicks(0), _shift_state(shift_state) { }
	MouseEvent(const MouseEvent& other) : Event(other.source(), other.time(), other.id()),
		_x(other._x), _y(other._y), _button(other._button), _clicks(other._clicks), _shift_state(other._shift_state) { }
	virtual ~MouseEvent(){}

	int x() const {
		return _x;
	}
	int y() const {
		return _y;
	}
	int button() const {
		return _button;
	}
	int shiftState() const {
		return _shift_state;
	}
	int clicks() const {
		return _clicks;
	}

	MAKE_CONSTANT(int,MOUSE_DOWN,0x00010001);
	MAKE_CONSTANT(int,MOUSE_UP,0x00010002);
	MAKE_CONSTANT(int,MOUSE_MOVE,0x00010003);
	MAKE_CONSTANT(int,MOUSE_ENTER,0x00010004);
	MAKE_CONSTANT(int,MOUSE_LEAVE,0x00010005);
	MAKE_CONSTANT(int,CLICK,0x00010006);

};

class MouseWheelEvent : public MouseEvent {
private:
	int _wheel, _mode;
public:
	MouseWheelEvent(Component* source, t_bigint time, int event_id, int x, int y, int button, int clicks, int shift_state, int wheel, int mode) :
		MouseEvent(source, time, event_id, x, y, button, clicks, shift_state), _wheel(wheel), _mode(mode) { }
	MouseWheelEvent(const MouseWheelEvent& other) :
		MouseEvent(other.source(), other.time(), other.id(), other.x(), other.y(), other.button(), other.clicks(), other.shiftState()),
		_wheel(other._wheel), _mode(other._mode) { }
	virtual ~MouseWheelEvent(){}

	int wheel() const {
		return _wheel;
	}

	int mode() const {
		return _mode;
	}

	MAKE_CONSTANT(int,MOUSE_WHEEL,0x00010007);
	MAKE_CONSTANT(int,MODE_UNIT,1);
	MAKE_CONSTANT(int,MODE_PAGE,1);

};

// ---------------------------------------------------------------------------------------------------------------------------
// Keyboard Events

class KeyEvent : public Event {
private:
	int _key_code;
	int _key_char;
	int _shift_state;
public:
	KeyEvent(Component* source, t_bigint time, int event_id, int key_code, int key_char, int shift_state) :
		Event(source, time, event_id),
			_key_code(key_code), _key_char(key_char), _shift_state(shift_state) { }
	KeyEvent(const KeyEvent& other) : Event(other.source(), other.time(), other.id()),
			_key_code(other._key_code), _key_char(other._key_char), _shift_state(other._shift_state) { }
	virtual ~KeyEvent(){}

	int keyCode() const {
		return _key_code;
	}
	int keyChar() const {
		return _key_char;
	}
	int shiftState() const {
		return _shift_state;
	}

	MAKE_CONSTANT(int,KEY_DOWN,0x00020001);
	MAKE_CONSTANT(int,KEY_UP,0x00020002);
	MAKE_CONSTANT(int,KEY_CHAR,0x00020003);
};

class FocusEvent : public Event {
private:
	Component* _other_component;
public:
	FocusEvent(Component* source, t_bigint time, int event_id, Component* other_component) :
		Event(source, time, event_id),
		_other_component(other_component) { }
	FocusEvent(const FocusEvent& other) :
		Event(other.source(), other.time(), other.id()),
		_other_component(other._other_component) { }
	virtual ~FocusEvent(){}

	Component* otherComponent() const {
		return _other_component;
	}

	MAKE_CONSTANT(int,GOT_FOCUS,0x00020004);
	MAKE_CONSTANT(int,LOST_FOCUS,0x00020005);
};

// ---------------------------------------------------------------------------------------------------------------------------
// Timer Event

class TimerEvent : public Event {
private:
	int _timer_id;
public:
	TimerEvent(Component* source, t_bigint time, int event_id, int timer_id) :
		Event(source, time, event_id),
		_timer_id(timer_id) { }
	TimerEvent(const TimerEvent& other) :
		Event(other.source(), other.time(), other.id()),
		_timer_id(other._timer_id) { }
	virtual ~TimerEvent(){}

	bool timerId() const {
		return _timer_id;
	}

	MAKE_CONSTANT(int,TIMER,0x00040001);
};

// ---------------------------------------------------------------------------------------------------------------------------
// Component Event

class ComponentEvent : public Event {
public:
	ComponentEvent(Component* source, t_bigint time, int event_id) :
		Event(source, time, event_id) { }
	ComponentEvent(const ComponentEvent& other) :
		Event(other.source(), other.time(), other.id()) { }
	virtual ~ComponentEvent(){}

	MAKE_CONSTANT(int,SHOW,0x00050007);
	MAKE_CONSTANT(int,HIDE,0x00050008);
	MAKE_CONSTANT(int,RESIZE,0x00050009);
	MAKE_CONSTANT(int,MOVE,0x00050010);
	MAKE_CONSTANT(int,RESIZING,0x00050011);
	MAKE_CONSTANT(int,MOVING,0x00050012);
	MAKE_CONSTANT(int,CREATE,0x00050013);
	MAKE_CONSTANT(int,DESTROY,0x00050014);
	MAKE_CONSTANT(int,PAINT,0x00050015);
	MAKE_CONSTANT(int,ERASE_BACKGROUND,0x00050016);
};

class ResizeEvent : public ComponentEvent {
protected:
	Size _new_size;
public:
	ResizeEvent(Component* source, t_bigint time, int event_id, const Size& new_size) :
		ComponentEvent(source, time, event_id), _new_size(new_size) { }
	ResizeEvent(const ResizeEvent& other) :
		ComponentEvent(other.source(), other.time(), other.id()), _new_size(other._new_size) { }
	virtual ~ResizeEvent(){}

	Size newSize() const {
		return _new_size;
	}

};

class ResizingEvent : public ComponentEvent {
protected:
	Rect* _rect;
	int _dir;
public:
	ResizingEvent(Component* source, t_bigint time, int event_id, Rect* rect, int dir) :
		ComponentEvent(source, time, event_id), _rect(rect), _dir(dir) { }
	ResizingEvent(const ResizingEvent& other) :
		ComponentEvent(other.source(), other.time(), other.id()), _rect(other._rect), _dir(other._dir) { }
	virtual ~ResizingEvent(){}

	Rect* getRect() {
		return _rect;
	}

	int getResizingDir() const {
		return _dir;
	}

	MAKE_CONSTANT(int,DIR_LEFT,1);
	MAKE_CONSTANT(int,DIR_RIGHT,2);
	MAKE_CONSTANT(int,DIR_TOP,3);
	MAKE_CONSTANT(int,DIR_TOPLEFT,4);
	MAKE_CONSTANT(int,DIR_TOPRIGHT,5);
	MAKE_CONSTANT(int,DIR_BOTTOM,6);
	MAKE_CONSTANT(int,DIR_BOTTOMLEFT,7);
	MAKE_CONSTANT(int,DIR_BOTTOMRIGHT,8);
};

class MoveEvent : public ComponentEvent {
protected:
	Point _new_position;
public:
	MoveEvent(Component* source, t_bigint time, int event_id, const Point& new_position) :
		ComponentEvent(source, time, event_id), _new_position(new_position) { }
	MoveEvent(const MoveEvent& other) :
		ComponentEvent(other.source(), other.time(), other.id()), _new_position(other._new_position) { }
	virtual ~MoveEvent(){}

	Point newPosition() const {
		return _new_position;
	}

};

class MovingEvent : public ComponentEvent {
protected:
	Point* _pos;
public:
	MovingEvent(Component* source, t_bigint time, int event_id, Point* pos) :
		ComponentEvent(source, time, event_id), _pos(pos) { }
	MovingEvent(const MovingEvent& other) :
		ComponentEvent(other.source(), other.time(), other.id()), _pos(other._pos) { }
	virtual ~MovingEvent(){}

	Point* getPosition() {
		return _pos;
	}

};

class PaintEvent : public ComponentEvent {
protected:
	Graphics* _graphics;
public:
	PaintEvent(Component* source, t_bigint time, int event_id, Graphics* new_size) :
		ComponentEvent(source, time, event_id), _graphics(new_size) { }
	PaintEvent(const PaintEvent& other) :
		ComponentEvent(other.source(), other.time(), other.id()), _graphics(other._graphics) { }
	virtual ~PaintEvent(){}

	Graphics* graphics() {
		return _graphics;
	}

};

class WindowEvent : public ComponentEvent {
public:
	WindowEvent(Window* source, t_bigint time, int event_id) :
		ComponentEvent((Component*)source, time, event_id) { }
	WindowEvent(const WindowEvent& other) :
		ComponentEvent(other.source(), other.time(), other.id()) { }
	virtual ~WindowEvent(){}

	MAKE_CONSTANT(int,ACTIVATED,0x00060001);
	MAKE_CONSTANT(int,DEACTIVATED,0x00060002);
	MAKE_CONSTANT(int,CLOSE,0x00060003);
	MAKE_CONSTANT(int,OPEN,0x00060004);
	MAKE_CONSTANT(int,ICONIFIED,0x00060005);
	MAKE_CONSTANT(int,DEICONIFIED,0x00060006);
};

// ---------------------------------------------------------------------------------------------------------------------------
// Scroll Bar Event

class ScrollBarEvent : public Event {
private:
	int _position;
public:
	ScrollBarEvent(Component* source, t_bigint time, int event_id, int position) :
		Event(source, time, event_id),
		_position(position) { }
	ScrollBarEvent(const ScrollBarEvent& other) :
		Event(other.source(), other.time(), other.id()),
		_position(other._position) { }
	virtual ~ScrollBarEvent(){}

	bool position() const {
		return _position;
	}

	MAKE_CONSTANT(int,V_SCROLL,0x00070001);
	MAKE_CONSTANT(int,H_SCROLL,0x00070002);
};

// ---------------------------------------------------------------------------------------------------------------------------
// Command Event

class CommandEvent : public Event {
private:
	int _command_id;
public:
	CommandEvent(Component* source, t_bigint time, int event_id, int command_id) :
		Event(source, time, event_id),
		_command_id(command_id) { }
	CommandEvent(const CommandEvent& other) :
		Event(other.source(), other.time(), other.id()),
		_command_id(other._command_id) { }
	virtual ~CommandEvent(){}

	bool commandId() const {
		return _command_id;
	}

	MAKE_CONSTANT(int,COMMAND,0x00080001);
};

// ---------------------------------------------------------------------------------------------------------------------------
// Menu Event

class MenuEvent : public Event {
private:
	int _x,_y;
public:
	MenuEvent(Component* source, t_bigint time, int event_id, int x, int y) :
		Event(source, time, event_id),
		_x(x), _y(y) { }
	MenuEvent(const MenuEvent& other) :
		Event(other.source(), other.time(), other.id()),
		_x(other._x), _y(other._y) { }
	virtual ~MenuEvent(){}

	int x() const {
		return _x;
	}

	int y() const {
		return _y;
	}

	MAKE_CONSTANT(int,CONTEXT_MENU,0x00090001);
	MAKE_CONSTANT(int,MENU_SELECTED,0x00090002);
};


} // ui
} // utils


#endif /* EVENT_HPP_ */
