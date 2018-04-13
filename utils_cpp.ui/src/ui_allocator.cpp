/*
 * ui_allocator.cpp
 *
 *  Created on: 05/08/2014
 *      Author: Alan
 */

#include <utils/ui/ui_defs.hpp>
#include <utils/ui/ui_allocator.hpp>
#include <utils/ui/image.hpp>
#include <cstdlib>

namespace utils {
namespace ui {

struct DefaultUIAllocator : public UIAllocator {
	DefaultUIAllocator() { }
	virtual ~DefaultUIAllocator() { }
	virtual void* malloc(size_t size)  {
		return ::malloc(size);
	}
	virtual void free(void* mem, size_t size) {
		return ::free(mem);
	}
};

static UIAllocator* actual_ui_allocator = new DefaultUIAllocator();

UIAllocator* getUIAllocator() {
	return actual_ui_allocator;
}
UIAllocator* setUIAllocator(UIAllocator* new_alloc) {
	UIAllocator* old = actual_ui_allocator;
	actual_ui_allocator = new_alloc;
	return old;
}

void* UIObject::operator new(size_t nbytes) {
	return actual_ui_allocator->malloc(nbytes);
}
void UIObject::operator delete(void* p, size_t nbytes) {
	actual_ui_allocator->free(p,nbytes);
}

}
}

