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
#include <blitz/array.h>

#define SCOPEMM_ENABLE_BLITZ
#include "scopemm/scopemm.h"

class Sinewave : public scopemm::PlotCanvas {
public:
	Sinewave() : 
		timeval(0)
	{
		addLayer(raster);

		// changing these will affect the resolution and the
		// position of the pattern, but the red dot should
		// always line up with the mouse cursor
		//setBbox(scopemm::Bbox(-2, -2, 3, 3));
		raster.setBbox(scopemm::Bbox(-1, -1, 1, 1));
		// this controls whether the first or second index of the data arrays
		// correspond to the X axis
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
		// before writing pixel data and before calling getAffine.  setData
		// will call resize itself, but we still need to do it here in order
		// for the affine to be valid.
		data_buf.resize(w, h);
		scopemm::CoordXform affine = raster.getAffine();

		if(data_r.shape()[0] != w || data_r.shape()[1] != h) {
			data_r.resize(w, h);
			data_g.resize(w, h);
			data_b.resize(w, h);
		}

		for(int i=0; i<w; i++) {
			for(int j=0; j<h; j++) {
				double x, y;
				// It is necessary to add 0.5 in order to get the coords of the
				// pixel center rather than the pixel edge.  This really only
				// matters when the pixels are big.
				affine.fwd(i+0.5, j+0.5, x, y);
				data_b(i, j) = sin(sqrt((x*x*4.0+y*y)*200.0) + timeval);
				data_g(i, j) = cos(sqrt((x*x*4.0+y*y)*200.0) + timeval);
				x -= mouseX();
				y -= mouseY();
				if(mouseIn()) {
					data_r(i, j) = exp(-(x*x+y*y)*10.0);
				} else {
					data_r(i, j) = 0;
				}
			}
		}

		// Autoscale:
		// setData(data_r, data_g, data_b);
		// Same scale for each band:
		// setData(data_r, data_g, data_b, -1.0, 1.0);
		// Different scale for each band:
		raster.setData(
			data_r,  0.0, 1.0,
			data_g, -1.0, 1.0,
			data_b, -1.0, 1.0
		);
		// Grascale:
		//setData(data_r, 0.0, 1.0);

		return true;
	}

	double timeval;
	blitz::Array<double, 2> data_r;
	blitz::Array<double, 2> data_g;
	blitz::Array<double, 2> data_b;
	scopemm::RasterArea raster;
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
