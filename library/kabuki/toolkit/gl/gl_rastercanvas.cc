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
#include <vector>
#include <assert.h>
#include <math.h>

#include "scopemm/scopemm.h"
#include "scopemm/layerimpl.h"

namespace scopemm {

class RasterAreaImpl : public PlotLayerImplBase {
public:
	RasterAreaImpl() :
		bbox(0, 0, 1, 1),
		swap_axes(false),
		bilinear(false)
	{ }

	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>);
	virtual bool hasMinMax() const { return true; }
	virtual Bbox getBbox() const { return bbox.normalize(); }

	CoordXform getAffine();

	Bbox bbox;
	CoordXform affine;
	bool swap_axes;
	bool bilinear;
	RawRGB data_buf;
	RawRGB draw_buf;
};

static void transform(
	const RawRGB &in, RawRGB &out, 
	AffineTransform affine, bool bilinear,
	size_t clip_xmin, size_t clip_xmax, 
	size_t clip_ymin, size_t clip_ymax
) {
	assert(clip_xmax <= out.w);
	assert(clip_ymax <= out.h);
	for(size_t out_y=clip_ymin; out_y<clip_ymax; out_y++) {
	for(size_t out_x=clip_xmin; out_x<clip_xmax; out_x++) {
		double in_x, in_y;
		affine(out_x, out_y, in_x, in_y);
		if(bilinear) {
			if(
				in_x>=0 && 
				in_y>=0 && 
				in_x<=double(in.w) && 
				in_y<=double(in.h)
			) {
				in_x -= 0.5;
				in_y -= 0.5;
				int i = int(floor(in_x));
				int j = int(floor(in_y));
				double dx = in_x-i;
				double dy = in_y-j;
				for(size_t band=0; band<3; band++) {
					int i0 = (i>=0) ? i : i+1;
					int i1 = (i+1<int(in.w)) ? i+1 : i;
					int j0 = (j>=0) ? j : j+1;
					int j1 = (j+1<int(in.h)) ? j+1 : j;
					double v0 = (1.0-dx)*in(i0, j0, band) + dx*in(i1, j0, band);
					double v1 = (1.0-dx)*in(i0, j1, band) + dx*in(i1, j1, band);
					double v  = (1.0-dy)*v0 + dy*v1;
					out(out_x, out_y, band) = v;
				}
			} else {
				// FIXME - make transparent
				for(size_t band=0; band<3; band++) {
					out(out_x, out_y, band) = 0;
				}
			}
		} else {
			int i = int(floor(in_x));
			int j = int(floor(in_y));
			if(i>=0 && j>=0 && size_t(i)<in.w && size_t(j)<in.h) {
				for(size_t band=0; band<3; band++) {
					out(out_x, out_y, band) = in(i, j, band);
				}
			} else {
				// FIXME - make transparent
				for(size_t band=0; band<3; band++) {
					out(out_x, out_y, band) = 0;
				}
			}
		}
	} } // out_x, out_y
}

RasterArea::RasterArea() {
	setZOrder(ZORDER_RASTER_AREA);
}

RasterArea &RasterArea::setSwapAxes(bool state) {
	impl->swap_axes = state;
	fireChangeEvent();
	return *this;
}

RasterArea &RasterArea::setBilinear(bool state) {
	impl->bilinear = state;
	fireChangeEvent();
	return *this;
}

RasterArea &RasterArea::setBbox(Bbox bbox) {
	impl->bbox = bbox;
	fireChangeEvent();
	return *this;
}

RasterArea &RasterArea::setBboxFromDataSize() {
	if(impl->swap_axes) {
		setBbox(Bbox(0, impl->data_buf.w, impl->data_buf.h, 0));
	} else {
		setBbox(Bbox(0, impl->data_buf.h, impl->data_buf.w, 0));
	}
	return *this;
}

void RasterAreaImpl::draw(
	PlotCanvas *parent,
	Cairo::RefPtr<Cairo::Context> cr __attribute__((unused))
) {
	Glib::RefPtr<Gdk::Window> window = parent->get_window();

	if(window && data_buf.w && data_buf.h) {
		const int w = parent->get_allocation().get_width();
		const int h = parent->get_allocation().get_height();

		AffineTransform screen_to_data = 
			getAffine().inv * parent->getAffine().inv;

		int clip_xmin, clip_xmax, clip_ymin, clip_ymax;
		{
			double x, y;
			parent->getAffine().fwd(bbox.xmin, bbox.ymin, x, y);
			Bbox clip_bbox(x, y, x, y);              
			parent->getAffine().fwd(bbox.xmin, bbox.ymax, x, y);
			clip_bbox |= Bbox(x, y, x, y) ;          
			parent->getAffine().fwd(bbox.xmax, bbox.ymin, x, y);
			clip_bbox |= Bbox(x, y, x, y) ;          
			parent->getAffine().fwd(bbox.xmax, bbox.ymax, x, y);
			clip_bbox |= Bbox(x, y, x, y);

			clip_xmin = std::max(int(ceil(clip_bbox.xmin)), 0);
			clip_xmax = std::min(int(floor(clip_bbox.xmax)), w);
			clip_ymin = std::max(int(ceil(clip_bbox.ymin)), 0);
			clip_ymax = std::min(int(floor(clip_bbox.ymax)), h);
		}

		draw_buf.resize(w, h);
		transform(
			data_buf, draw_buf, screen_to_data, bilinear,
			clip_xmin, clip_xmax, clip_ymin, clip_ymax
		);

		window->draw_rgb_image(
			parent->get_style()->get_fg_gc(Gtk::STATE_NORMAL),
			clip_xmin, clip_ymin, clip_xmax-clip_xmin, clip_ymax-clip_ymin,
			Gdk::RGB_DITHER_NONE,
			&draw_buf(clip_xmin, clip_ymin, 0),
			w*3
		);
	}
}

CoordXform RasterAreaImpl::getAffine() {
	if(swap_axes) {
		// A special case needs to be made since we normally flip
		// the Y axis of the data array.  With swap_axes, we need
		// to flip the X axis here so that Y will be flipped after
		// the axis swap.
		return CoordXform::boxToBox(
			Bbox(data_buf.w, 0, 0, data_buf.h), bbox, swap_axes);
	} else {
		// Flip Y axis so that data_buf(0,0) corresponds to the top
		// left corner, as is usually expected with raster data.
		return CoordXform::boxToBox(
			Bbox(0, data_buf.h, data_buf.w, 0), bbox, swap_axes);
	}
}

RawRGB &RasterArea::getDataBuf() { return impl->data_buf; }

CoordXform RasterArea::getAffine() { return impl->getAffine(); }

} // namespace scopemm
