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

#ifndef SCOPEMM_MOUSEADAPTER_H
#define SCOPEMM_MOUSEADAPTER_H

#include <gtkmm/main.h>
#include <gtkmm/widget.h>

namespace scopemm {

class MouseAdapter {
public:
	MouseAdapter();

	~MouseAdapter() { }

	void attach(Gtk::Widget *w);

	void mouse_motion();
	void mouse_clicked(int button);

	sigc::signal<void, int> signal_clicked;
	sigc::signal<void> signal_motion;

private:
	void updateState(gdouble evt_x, gdouble evt_y, guint evt_state);
	bool on_motion_notify_event(GdkEventMotion* event);
	bool on_enter_notify_event(GdkEventCrossing* event);
	bool on_leave_notify_event(GdkEventCrossing* event);
	bool on_button_press_event(GdkEventButton* event);

public:
	bool mouse_in;
	double mouse_x, mouse_y;
	int button_state;
};

} // namespace scopemm

#endif // SCOPEMM_MOUSEADAPTER_H
