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
#include <iostream>

#include "scopemm/scopemm.h"

class Sinewave : public scopemm::PlotCanvas {
public:
	Sinewave() : 
		timeval(0)
	{
		addLayer(raster);
		addLayer(x_trace.setColor(1, 1, 1));
		addLayer(y_trace.setColor(1, 1, 1));

		// changing these will affect the resolution and the
		// position of the pattern, but the red dot should
		// always line up with the mouse cursor
		setBbox(scopemm::Bbox(-1, -1, 1, 1));
		raster.setBbox(scopemm::Bbox(-1, -1, 1, 1));
		// this controls whether the first or second index of data_buf
		// corresponds to the X axis
		raster.setSwapAxes(false);
		// this will flip the entire plot across the x=y line
		setSwapAxes(false);

		// bilinear interpolation, makes it look nicer
		raster.setBilinear(true);

		//setDrawAxes();

		Glib::signal_idle().connect(
			sigc::mem_fun(*this, &Sinewave::on_timeout));
	}

	virtual bool on_timeout() {
		timeval += 0.1;

		int w = 100;
		int h = 100;

		scopemm::RawRGB &data_buf = raster.getDataBuf();
		// resize changes the size of the data_buf array.  This must be done
		// before writing pixel data and before calling getAffine.
		data_buf.resize(w, h);
		scopemm::CoordXform affine = raster.getAffine();

		double mx = mouseX();
		double my = mouseY();

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x, y;
				// It is necessary to add 0.5 in order to get the coords of the
				// pixel center rather than the pixel edge.  This really only
				// matters when the pixels are big.
				affine.fwd(i+0.5, j+0.5, x, y);
				double vb = sin(sqrt((x*x*4.0+y*y)*200.0) + timeval);
				double vg = cos(sqrt((x*x*4.0+y*y)*200.0) + timeval);
				double vr;
				if(mouseIn()) {
					vr = exp(-((x-mx)*(x-mx)+(y-my)*(y-my))*50.0);
				} else {
					vr = 0;
				}
				// The blitz interface allows pushing of floating-point data
				// and handles either manual or automatic scaling of data.
				// Since STL doesn't have a container for 2D arrays, there is
				// no interface provided for pushing 2D floating-point data and
				// as a consequence it is necessary to manually convert pixel
				// data to uint8_t.  Perhaps it would be worth adding support
				// for boost::multi_array.
				data_buf(i, j, 0) = uint8_t(vr * 255.0);
				data_buf(i, j, 1) = uint8_t((vg+1.0)/2.0 * 255.0);
				data_buf(i, j, 2) = uint8_t((vb+1.0)/2.0 * 255.0);
			}
		}

		std::vector<double> xpts;
		std::vector<double> ypts;
		if(mouseIn()) {
			for(double alpha=0; alpha<=1; alpha+=0.01) {
				double x = getBbox().xmin + 
					(getBbox().xmax-getBbox().xmin)*alpha;
				double y = my;

				double v = sin(sqrt((x*x*4.0+y*y)*200.0) + timeval);

				xpts.push_back(x);
				ypts.push_back(y + v*0.1);
			}
		}
		x_trace.setXYData(xpts, ypts);

		xpts.clear();
		ypts.clear();
		if(mouseIn()) {
			for(double alpha=0; alpha<=1; alpha+=0.01) {
				double y = getBbox().ymin + 
					(getBbox().ymax-getBbox().ymin)*alpha;
				double x = mx;

				double v = sin(sqrt((x*x*4.0+y*y)*200.0) + timeval);

				xpts.push_back(x + v*0.1);
				ypts.push_back(y);
			}
		}
		y_trace.setXYData(xpts, ypts);

		fireChangeEvent();

		return true;
	}

	double timeval;
	scopemm::RasterArea raster;
	scopemm::PlotTrace x_trace;
	scopemm::PlotTrace y_trace;
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
