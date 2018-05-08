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

#include <algorithm>
#include <gtkmm/main.h>
#include <gtkmm/window.h>
#include <math.h>

#include "scopemm/scopemm.h"

int main(int argc, char *argv[]) {
	Gtk::Main kit(argc, argv);

	Gtk::Window win;
	win.set_title("Test");

	scopemm::PlotCanvas plot;
	plot.setXAutoRange()
		.setYAutoRange()
		.setDrawAxes(true)
		.setDrawGrids(true);
	//plot.setSwapAxes();

	scopemm::PlotTrace t1;
	scopemm::PlotTrace t2;
	scopemm::PlotTrace t3;
	t1.setColor(1, 0, 0);
	t2.setColor(0, 1, 0);
	t3.setColor(0, 0, 1);
	plot.addLayer(t1).addLayer(t2).addLayer(t3);

	const int nsamps = 100;

	std::vector<double> ypts;
	for(int i=0; i<nsamps; i++) {
		double theta = double(i) / (nsamps-1) * 2.0 * M_PI;
		ypts.push_back(sin(theta));
	}
	//t1.setYData(std::make_pair(ypts.begin(), ypts.end()));
	t1.setYData(ypts);

	std::vector<float> xpts;
	ypts.clear();
	for(int i=0; i<nsamps; i++) {
		double theta = double(i) / (nsamps-1) * 2.0 * M_PI;
		// The call to setYData above implicitly used an X range
		// of [0, nsamps-1].  We multiply X by nsamps here in order
		// to occupy the same range.
		xpts.push_back(cos(theta) * nsamps);
		ypts.push_back(sin(theta));
	}
	//t2.setXYData(
	//	std::make_pair(xpts.begin(), xpts.end()),
	//	std::make_pair(ypts.begin(), ypts.end())
	//);
	t2.setXYData(xpts, ypts);

	std::vector<std::pair<double, double> > xypts;
	for(int i=0; i<nsamps; i++) {
		double theta = double(i) / (nsamps-1) * 2.0 * M_PI;
		// The call to setYData above implicitly used an X range
		// of [0, nsamps-1].  We multiply X by nsamps here in order
		// to occupy the same range.
		double x = 0.2 * theta * cos(theta) * nsamps;
		double y = 0.2 * theta * sin(theta);
		xypts.push_back(std::make_pair(x, y));
	}
	//t3.setXYData(std::make_pair(xypts.begin(), xypts.end()));
	t3.setXYData(xypts);

	win.add(plot);
	plot.show();

	Gtk::Main::run(win);

	return 0;
}
