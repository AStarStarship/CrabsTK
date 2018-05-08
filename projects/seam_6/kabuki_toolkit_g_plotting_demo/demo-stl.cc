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

#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <math.h>

#include "scopemm/scopemm.h"

class Sinewave : public scopemm::PlotCanvas {
public:
	Sinewave() {
		setXAutoRange();
		setYAutoRange();
		//setSwapAxes();
		setDrawAxes();
		setDrawGrids();

		addLayer(t1);
		addLayer(t2);
		addLayer(t3);

		t1.setColor(1, 0, 0);
		t2.setColor(0, 1, 0);
		t3.setColor(0, 0, 1);

		alpha = 0;

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		alpha += 10.0;

		double scale = exp(-alpha/1000.0);

		int npts = 1000;

		std::vector<double> xpts;
		std::vector<double> ypts;
		std::vector<double> zpts;
		std::vector<std::pair<double, double> > xypts;

		for(int i=0; i<npts; i++) {
			double x = cos(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha) * scale;

			double y = sin(10.0 * 2.0 * 3.14159 * (i-npts/2) / npts + alpha / 200.0) *
				exp(-(i-npts/2)*(i-npts/2)/alpha) * scale;

			double z = 2.0 * (i-npts/2) / double(npts) * scale;

			xpts.push_back(x);
			ypts.push_back(y);
			zpts.push_back(z);
			xypts.push_back(std::make_pair(x, y));
		}

		t1.setXYData(xypts);
		t2.setXYData(xpts, zpts);
		t3.setXYData(zpts, ypts);

		// autoscale would work too, but this is smoother and keeps the origin
		// right in the middle
		setXRange(-scale, scale);
		setYRange(-scale, scale);

		return true;
	}

	double alpha;
	scopemm::PlotTrace t1;
	scopemm::PlotTrace t2;
	scopemm::PlotTrace t3;
};

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	Sinewave sw;
	win.add(sw);
	sw.show();

	Gtk::Main::run(win);

	return 0;
}
