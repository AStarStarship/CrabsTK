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

#ifndef SCOPEMM_PLOTCANVAS_H
#define SCOPEMM_PLOTCANVAS_H

#include <set>

#include <gtkmm/main.h>
#include <gtkmm/drawingarea.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace scopemm {

class PlotCanvasImpl;
typedef boost::shared_ptr<PlotCanvasImpl> PlotCanvasImplPtr;

class PlotLayerImplBase;
typedef boost::shared_ptr<PlotLayerImplBase> PlotLayerImplPtr;
class PlotLayerBase;

// This is noncopyable because Gtk::Widget is noncopyable.  If there were a way
// to copy widgets, then this class could be made copyable (using the default
// copy constructor), although copies would have to be inserted into
// PlotLayerImpl::change_listeners for each layer in order to receive
// queue_draw events.
class PlotCanvas : 
	public Gtk::DrawingArea,
	public boost::noncopyable 
{
public:
	PlotCanvas();
	~PlotCanvas();

	PlotCanvas &addLayer(const PlotLayerBase &layer);
	PlotCanvas &removeLayer(const PlotLayerBase &layer);

	PlotCanvas &setXAutoRange();
	PlotCanvas &setYAutoRange();
	PlotCanvas &setAutoRangeBorder(double x, double y);
	PlotCanvas &setXRange(double min, double max);
	PlotCanvas &setYRange(double min, double max);
	PlotCanvas &setBbox(Bbox new_bbox);
	PlotCanvas &setSwapAxes(bool state=true);
	PlotCanvas &setDrawAxes(bool state=true);
	PlotCanvas &setDrawAxes(bool xaxis, bool yaxis);
	PlotCanvas &setDrawGrids(bool state=true);
	PlotCanvas &setDrawGrids(bool xgrid, bool ygrid);
	PlotCanvas &setDrawXGrid(bool state=true);
	PlotCanvas &setDrawYGrid(bool state=true);

	const Bbox &getBbox() const;
	const CoordXform &getAffine() const;
	bool getSwapAxes() const;

	double mouseX() const;
	double mouseY() const;
	bool mouseIn() const;
	bool mouseButton1() const;
	bool mouseButton2() const;
	bool mouseButton3() const;

	sigc::signal<void, int> &signal_plot_clicked();
	sigc::signal<void> &signal_plot_motion();

	bool on_expose_event(GdkEventExpose* event);
	PlotCanvas &fireChangeEvent();

private:
	PlotCanvasImplPtr impl;
};

class PlotLayerBase {
	friend class PlotCanvas;
public:
	void setZOrder(double z);

protected:
	void fireChangeEvent();

	PlotLayerImplPtr impl_base;
};

template <class T>
class PlotLayerSub : public PlotLayerBase {
public:
	PlotLayerSub() : impl(new T()) {
		impl_base = PlotLayerImplPtr(impl);
	}

protected:
	T *impl;
};

} // namespace scopemm

#endif // SCOPEMM_PLOTCANVAS_H
