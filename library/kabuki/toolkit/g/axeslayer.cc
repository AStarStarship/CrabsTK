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

class AxesLayerImpl : public PlotLayerImplBase {
public:
	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);

	void drawStripes(
		const PlotCanvas *parent,
		const Cairo::RefPtr<Cairo::Context> &cr,
		double from, double to, double step,
		bool vertical
	) const;

	AxesLayerImpl();

	bool draw_x_axis, draw_y_axis;
};

AxesLayerImpl::AxesLayerImpl() :
	draw_x_axis(true), draw_y_axis(true)
{ }

AxesLayer::AxesLayer() {
	setZOrder(ZORDER_AXES_LAYER);
}

AxesLayer &AxesLayer::setDrawXAxis(bool state) {
	impl->draw_x_axis = state;
	fireChangeEvent();
	return *this;
}

AxesLayer &AxesLayer::setDrawYAxis(bool state) {
	impl->draw_y_axis = state;
	fireChangeEvent();
	return *this;
}

void AxesLayerImpl::draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context> cr) {
	cr->save();
	cr->set_line_width(1);
	cr->set_source_rgb(0.3, 0.3, 0.3);
	cr->set_antialias(Cairo::ANTIALIAS_NONE);

	const Bbox bbox = parent->getBbox();
	const CoordXform affine = parent->getAffine();

	if(draw_x_axis) {
		double x1, y1, x2, y2;
		affine.fwd(0, bbox.ymin, x1, y1);
		affine.fwd(0, bbox.ymax, x2, y2);
		cr->move_to(x1, y1);
		cr->line_to(x2, y2);
		cr->stroke();
	}
	if(draw_y_axis) {
		double x1, y1, x2, y2;
		affine.fwd(bbox.xmin, 0, x1, y1);
		affine.fwd(bbox.xmax, 0, x2, y2);
		cr->move_to(x1, y1);
		cr->line_to(x2, y2);
		cr->stroke();
	}
	cr->restore();
}

} // namespace scopemm
