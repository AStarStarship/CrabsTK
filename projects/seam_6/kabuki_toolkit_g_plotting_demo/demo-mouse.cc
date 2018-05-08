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

#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <math.h>

#include "scopemm/scopemm.h"

class DemoWidget : public scopemm::PlotCanvas {
public:
	DemoWidget() :
		click_x(0),
		click_y(0)
	{
		setXRange(-10, 10);
		setYRange(-10, 10);
		setDrawAxes(true);
		setDrawGrids(true);

		addLayer(t1);
		addLayer(t2);

		// These are convenience hooks that give an interface much
		// simpler than the Gtk interface.  You are also free to
		// just use the normal Gtk mouse hooks, in which case
		// PlotCanvas::getAffine().inv can be used to transform
		// from screen coordinates to plot coordinates.
		signal_plot_clicked().connect(sigc::mem_fun(
			this, &DemoWidget::mouse_clicked));
		signal_plot_motion().connect(sigc::mem_fun(
			this, &DemoWidget::mouse_motion));
	}

	void mouse_clicked(int button) {
		click_x = mouseX();
		click_y = mouseY();

		std::cout << "x=" << click_x << ", y=" << click_y << std::endl;

		double r = 1.0;
		const int nsamps = 100;
		std::vector<double> xpts;
		std::vector<double> ypts;
		for(int i=0; i<nsamps; i++) {
			double theta = double(i) / (nsamps-1) * 2.0 * M_PI;
			xpts.push_back(cos(theta)*r + click_x);
			ypts.push_back(sin(theta)*r + click_y);
		}
		t1.setXYData(xpts, ypts);
		t1.setColor(
			button==1 ? 1 : 0,
			button==2 ? 1 : 0,
			button==3 ? 1 : 0
		);
	}

	void mouse_motion() {
		std::vector<double> xpts;
		std::vector<double> ypts;
		if(mouseIn()) {
			xpts.push_back(click_x);
			xpts.push_back(mouseX());
			ypts.push_back(click_y);
			ypts.push_back(mouseY());
		}
		t2.setXYData(xpts, ypts);
		t2.setColor(
			mouseButton1() ? 1 : 0.5,
			mouseButton2() ? 1 : 0.5,
			mouseButton3() ? 1 : 0.5
		);
	}

	scopemm::PlotTrace t1;
	scopemm::PlotTrace t2;
	double click_x;
	double click_y;
};

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	DemoWidget demo;
	win.add(demo);
	demo.show();

	Gtk::Main::run(win);

	return 0;
}
