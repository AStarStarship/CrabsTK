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

#include <boost/foreach.hpp>
#include <algorithm>

#include "scopemm/scopemm.h"
#include "scopemm/layerimpl.h"

namespace scopemm {

class PlotTraceImpl : public PlotLayerImplBase {
public:
	PlotTraceImpl() {
		for(int i=0; i<3; i++) rgb[i] = 0;
	}

	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);
	virtual bool hasMinMax() const { return !xpts.empty(); }
	virtual Bbox getBbox() const;

	std::vector<double> xpts;
	std::vector<double> ypts;
	double rgb[3];
};

void PlotTraceImpl::draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_line_width(1);
	cr->set_source_rgb(rgb[0], rgb[1], rgb[2]);

	const CoordXform affine = parent->getAffine();

	assert(xpts.size() == ypts.size());
	size_t npts = xpts.size();
	//std::cout << "npts=" << npts << std::endl;
	for(size_t i=0; i<npts; ++i) {
		double x, y;
		affine.fwd(xpts[i], ypts[i], x, y);
		if(!i) {
			cr->move_to(x, y);
		} else {
			cr->line_to(x, y);
		}
	}
	cr->stroke();
	cr->restore();
}

Bbox PlotTraceImpl::getBbox() const { 
	assert(!xpts.empty());
	assert(!ypts.empty());
	return Bbox(
		*std::min_element(xpts.begin(), xpts.end()),
		*std::min_element(ypts.begin(), ypts.end()),
		*std::max_element(xpts.begin(), xpts.end()),
		*std::max_element(ypts.begin(), ypts.end())
	);
}

PlotTrace::PlotTrace() { 
	setZOrder(ZORDER_LINE_PLOT);
	setColor(1, 0, 0);
}

PlotTrace& PlotTrace::setColor(double r, double g, double b) {
	impl->rgb[0] = r;
	impl->rgb[1] = g;
	impl->rgb[2] = b;
	fireChangeEvent();
	return *this;
}

std::vector<double> &PlotTrace::getXPts() { return impl->xpts; }

std::vector<double> &PlotTrace::getYPts() { return impl->ypts; }

} // namespace scopemm
