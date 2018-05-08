/*
	Copyright 2009 Daniel Stahlke

	This file is part of scopemm.
	
	scopemm is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	scopemm is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with scopemm.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scopemm/mouseadapter.h"

namespace scopemm {

MouseAdapter::MouseAdapter() : 
	mouse_in(false),
	mouse_x(0),
	mouse_y(0),
	button_state(0)
{ }

void MouseAdapter::attach(Gtk::Widget *w) {
	w->set_events(
		w->get_events() | 
		Gdk::POINTER_MOTION_MASK |
		Gdk::POINTER_MOTION_HINT_MASK |
		Gdk::ENTER_NOTIFY_MASK |
		Gdk::LEAVE_NOTIFY_MASK |
		Gdk::BUTTON_PRESS_MASK |
		Gdk::BUTTON_RELEASE_MASK
	);

	w->signal_motion_notify_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_motion_notify_event));
	w->signal_enter_notify_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_enter_notify_event));
	w->signal_leave_notify_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_leave_notify_event));
	w->signal_button_press_event().connect(sigc::mem_fun(
		*this, &MouseAdapter::on_button_press_event));
}

void MouseAdapter::updateState(gdouble evt_x, gdouble evt_y, guint evt_state) {
	mouse_x = evt_x;
	mouse_y = evt_y;

	button_state =
		(evt_state & Gdk::BUTTON1_MASK ? 1 : 0) |
		(evt_state & Gdk::BUTTON2_MASK ? 2 : 0) |
		(evt_state & Gdk::BUTTON3_MASK ? 4 : 0);
}

bool MouseAdapter::on_motion_notify_event(GdkEventMotion* event) {
	//std::cout << "Motion " << event->state << std::endl;
	updateState(event->x, event->y, event->state);
	mouse_motion();
	return true;
}

bool MouseAdapter::on_enter_notify_event(GdkEventCrossing* event) {
	//std::cout << "enter_notify" << event->state << std::endl;
	mouse_in = true;
	updateState(event->x, event->y, event->state);
	mouse_motion();
	return true;
}

bool MouseAdapter::on_leave_notify_event(GdkEventCrossing* event) {
	//std::cout << "leave_notify" << event->state << std::endl;
	mouse_in = false;
	updateState(event->x, event->y, event->state);
	mouse_motion();
	return true;
}

bool MouseAdapter::on_button_press_event(GdkEventButton* event) {
	//std::cout << "state " << event->state << "," << event->button << std::endl;
	updateState(event->x, event->y, event->state);
	mouse_clicked(event->button);
	return true;
}

void MouseAdapter::mouse_motion() {
//	std::cout
//		<< "mouse: " << mouse_in << "," << mouse_x << "," << mouse_y
//		<< " / " << button1 << "," << button2 << "," << button3
//		<< std::endl;
	signal_motion();
}

void MouseAdapter::mouse_clicked(int button) {
	//std::cout << "click: " << mouse_x << "," << mouse_y << "," << button << "," << button_state << std::endl;
	signal_clicked(button);
}

} // namespace scopemm
