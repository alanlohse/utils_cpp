/*
 * test_ui.cpp
 *
 *  Created on: 21/04/2015
 *      Author: Alan
 */

#include <utils/ui/image.hpp>
#include <utils/ui/component.hpp>
#include <utils/ui/window.hpp>
#include <utils/ui/button.hpp>
#include <utils/ui/event.hpp>
#include <utils/test/test_tools.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;
using namespace utils;
using namespace utils::ui;

struct TestPaintListener : EventListener {

	virtual ~TestPaintListener(){}
	virtual void operator() (Event* evt) {
		Graphics* g = dynamic_cast<PaintEvent*>(evt)->graphics();
		g->setLineColor(Color::RED);
		g->setStroke(g->createStroke(LineStyle::SOLID,LineJoin::MITER,LineCap::ROUND,8));
		g->beginPath();
		g->moveTo(10,10);
		g->bezierCurveTo(10,90,10,90,90,90);
		g->lineTo(90,10);
		g->stroke();
	}

};

Window* showDialogTest(Window* parent) {
	Window* wnd = new Window(parent,"Dialogo",-1,-1,300,200);
	wnd->setType(WindowType::DIALOG);
	wnd->setModal(true);
	wnd->setCloseAction(CloseAction::HIDE);
	wnd->setShowCloseButton(true);
	Component* comp = new Component(wnd,0,0,100,100);
	comp->setMinimumSize(Size(100,100));
	comp->setMaximumSize(Size(200,200));
	TestPaintListener pl;
	comp->addPaintListener(&pl);
	comp->setVisible(true);
	wnd->setVisible(true);
	return wnd;
}

void test_ui() {

	basic_ifstream<char, char_traits<char> > in;
	in.open ("media\\teste.bmp", ios::binary | ios::in);

	Image* img = Image::loadImage(in);

//	Image* img = Image::createCompatibleImage(100,100);
	if (img == NULL) {
		cout << "Not loaded." << endl;
		return;
	}

	cout << "Width: " << img->getSize().width << endl;
	cout << "Height: " << img->getSize().height << endl;
	cout << "BPP: " << img->getBitsPerPixel().ordinal() << endl;

	Window* wnd = new Window(NULL,"Teste",-1,-1,600,400);

	struct ClickListener : EventListener {
		Window* _parent;
		ClickListener(Window* parent): _parent(parent){}
		virtual ~ClickListener(){}
		virtual void operator() (Event* evt) {
			if(evt->id() == MouseEvent::CLICK) {
				Window* dlg = showDialogTest(_parent);
				stringstream str;
				str << "Mostrou ";
				str << ((MouseEvent*)evt)->x();
				str << ", ";
				str << ((MouseEvent*)evt)->y();
//				MessageBoxA(NULL,str.str().c_str(),"Ok",MB_OK);
				delete dlg;
			}
		}

	} ml(wnd);

	struct CommandListener : EventListener {
		virtual void operator() (Event* evt) {
			cout << "Button clicked" << endl;
//			MessageBoxA(NULL,"Button clicked","Ok",MB_OK);
		}

	} cl;

	wnd->setCloseAction(CloseAction::QUIT);
	Component* comp = new Component(wnd,0,0,100,100);
	Button* btn = new Button(NULL,100,100,80,20,0);
	btn->setText("Teste");
	comp->setMinimumSize(Size(100,100));
	comp->setMaximumSize(Size(200,200));
	TestPaintListener pl;
	comp->addPaintListener(&pl);
	comp->addMouseListener(&ml);
	comp->setVisible(true);
	wnd->addChild(btn);
	btn->addCommandListener(&cl);
	btn->setImage(img);
	btn->setVisible(true);
	wnd->setVisible(true);
	delete wnd;
}

