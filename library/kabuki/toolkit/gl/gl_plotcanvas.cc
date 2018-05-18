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
#include "scopemm/mouseadapter.h"

namespace scopemm {

/// PlotCanvasImpl ////////////////////////////////////

class PlotCanvasImpl {
public:
	PlotCanvasImpl();

	void recalcAutoRange();
	void recalcAffine();

	bool x_auto, y_auto;
	double border_x, border_y;
	int screen_w, screen_h;
	Bbox bbox;
	CoordXform affine;
	bool swap_axes;
	std::set<PlotLayerImplPtr> layers;
	GridLayer grid_layer;
	AxesLayer axes_layer;
	MouseAdapter mouse;
};

PlotCanvasImpl::PlotCanvasImpl() : 
	x_auto(true), y_auto(true),
	border_x(0.02), border_y(0.02),
	screen_w(0), screen_h(0),
	bbox(0, 0, 1, 1), swap_axes(false)
{ }

void PlotCanvasImpl::recalcAutoRange() {
	if(x_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
			if(!layer->hasMinMax()) continue;
			Bbox sub_bbox = layer->getBbox();
			if(first || sub_bbox.xmin < min) min = sub_bbox.xmin;
			if(first || sub_bbox.xmax > max) max = sub_bbox.xmax;
			first = false;
		}
		double delta = max-min;
		min -= delta * border_x;
		max += delta * border_x;
		// if no layers had min/max
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		bbox.xmin = min;
		bbox.xmax = max;
	}
	if(y_auto) {
		bool first = true;
		double min=0, max=0;
		BOOST_FOREACH(const PlotLayerImplPtr &layer, layers) {
			if(!layer->hasMinMax()) continue;
			Bbox sub_bbox = layer->getBbox();
			if(first || sub_bbox.ymin < min) min = sub_bbox.ymin;
			if(first || sub_bbox.ymax > max) max = sub_bbox.ymax;
			first = false;
		}
		double delta = max-min;
		min -= delta * border_y;
		max += delta * border_y;
		// if no layers had min/max
		if(first) { min = 0; max = 1; }
		if(min == max) { min -= 1; max += 1; }
		bbox.ymin = min;
		bbox.ymax = max;
	}
}

void PlotCanvasImpl::recalcAffine() {
	Bbox screen_bbox(0, screen_h, screen_w, 0);
	affine = CoordXform::boxToBox(bbox, screen_bbox, swap_axes);
}

/// PlotCanvas ////////////////////////////////////////

PlotCanvas::PlotCanvas() :
	impl(new PlotCanvasImpl())
{
	impl->mouse.attach(this);
}

PlotCanvas::~PlotCanvas() {
	// now that our object no longer exists, it must not receive
	// any more events
	BOOST_FOREACH(const PlotLayerImplPtr &layer, impl->layers) {
		layer->change_listeners.erase(this);
	}
}

PlotCanvas &PlotCanvas::addLayer(const PlotLayerBase &layer) {
	layer.impl_base->change_listeners.insert(this);
	impl->layers.insert(layer.impl_base);
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::removeLayer(const PlotLayerBase &layer) {
	layer.impl_base->change_listeners.erase(this);
	impl->layers.erase(layer.impl_base);
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setXAutoRange() {
	impl->x_auto = true;
	impl->recalcAutoRange();
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setYAutoRange() {
	impl->y_auto = true;
	impl->recalcAutoRange();
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setAutoRangeBorder(double x, double y) {
	assert(x >= 0 && y >= 0);
	impl->border_x = x;
	impl->border_y = y;
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setXRange(double min, double max) {
	impl->bbox.xmin = min;
	impl->bbox.xmax = max;
	impl->x_auto = false;
	impl->recalcAffine();
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setYRange(double min, double max) {
	impl->bbox.ymin = min;
	impl->bbox.ymax = max;
	impl->y_auto = false;
	impl->recalcAffine();
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setBbox(Bbox new_bbox) {
	impl->bbox = new_bbox;
	impl->x_auto = false;
	impl->y_auto = false;
	impl->recalcAffine();
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setSwapAxes(bool state) {
	impl->swap_axes = state;
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setDrawAxes(bool state) {
	setDrawAxes(state, state);
	return *this;
}

PlotCanvas &PlotCanvas::setDrawAxes(bool xaxis, bool yaxis) {
	if(xaxis || yaxis) {
		impl->axes_layer.setDrawXAxis(xaxis);
		impl->axes_layer.setDrawYAxis(yaxis);
		addLayer(impl->axes_layer);
	} else {
		removeLayer(impl->axes_layer);
	}
	fireChangeEvent();
	return *this;
}

PlotCanvas &PlotCanvas::setDrawGrids(bool state) {
	setDrawGrids(state, state);
	return *this;
}

PlotCanvas &PlotCanvas::setDrawGrids(bool xgrid, bool ygrid) {
	if(xgrid || ygrid) {
		impl->grid_layer.setDrawXGrid(xgrid);
		impl->grid_layer.setDrawYGrid(ygrid);
		addLayer(impl->grid_layer);
	} else {
		removeLayer(impl->grid_layer);
	}
	fireChangeEvent();
	return *this;
}

const Bbox &PlotCanvas::getBbox() const { return impl->bbox; }

const CoordXform &PlotCanvas::getAffine() const { return impl->affine; }

bool PlotCanvas::getSwapAxes() const { return impl->swap_axes; }

double PlotCanvas::mouseX() const {
	double plot_x, plot_y;
	getAffine().inv(
		impl->mouse.mouse_x, impl->mouse.mouse_y,
		plot_x, plot_y
	);
	return plot_x;
}

double PlotCanvas::mouseY() const {
	double plot_x, plot_y;
	getAffine().inv(
		impl->mouse.mouse_x, impl->mouse.mouse_y,
		plot_x, plot_y
	);
	return plot_y;
}

bool PlotCanvas::mouseIn() const { return impl->mouse.mouse_in; }

bool PlotCanvas::mouseButton1() const { return impl->mouse.button_state & 1; }

bool PlotCanvas::mouseButton2() const { return impl->mouse.button_state & 2; }

bool PlotCanvas::mouseButton3() const { return impl->mouse.button_state & 4; }

sigc::signal<void, int> &PlotCanvas::signal_plot_clicked() {
	return impl->mouse.signal_clicked;
}

sigc::signal<void> &PlotCanvas::signal_plot_motion() {
	return impl->mouse.signal_motion;
}

bool PlotCanvas::on_expose_event(GdkEventExpose* event) {
	Glib::RefPtr<Gdk::Window> window = get_window();

	if(!window) return true;

	impl->screen_w = get_allocation().get_width();
	impl->screen_h = get_allocation().get_height();
	impl->recalcAffine();

	Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	if(event) {
		cr->rectangle(event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();
	}

	cr->save();
	cr->set_source_rgb(1, 1, 1);
	cr->paint();
	cr->restore();

	typedef std::pair<double, PlotLayerImplPtr> zbuf_element;
	std::set<zbuf_element> zsorted;
	BOOST_FOREACH(const PlotLayerImplPtr &layer, impl->layers) {
		zsorted.insert(zbuf_element(layer->zorder, layer));
	}
	BOOST_FOREACH(const zbuf_element &layer, zsorted) {
		layer.second->draw(this, cr);
	}

	return true;
}

PlotCanvas &PlotCanvas::fireChangeEvent() {
	impl->recalcAutoRange();
	queue_draw();
	return *this;
}

/// PlotLayerBase ////////////////////////////////////

void PlotLayerBase::setZOrder(double z) {
	impl_base->zorder = z;
	fireChangeEvent();
}

void PlotLayerBase::fireChangeEvent() {
	BOOST_FOREACH(PlotCanvas *l, impl_base->change_listeners) {
		l->fireChangeEvent();
	}
}

} // namespace scopemm
