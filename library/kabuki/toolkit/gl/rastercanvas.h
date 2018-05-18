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

#ifndef SCOPEMM_GRIDCANVAS_H
#define SCOPEMM_GRIDCANVAS_H

#include <algorithm>
#include <vector>
#include <stdint.h>

#ifdef SCOPEMM_ENABLE_BLITZ
#include <blitz/array.h>
#endif // SCOPEMM_ENABLE_BLITZ

#include "scopemm/plotcanvas.h"

namespace scopemm {

class RawRGB {
public:
	RawRGB() : w(0), h(0) { }

	void resize(size_t _w, size_t _h) {
		w = _w;
		h = _h;
		data.resize(w*h*3);
	}

	uint8_t &operator()(size_t x, size_t y, size_t band) {
		return data[(y*w+x)*3+band];
	}

	const uint8_t &operator()(size_t x, size_t y, size_t band) const {
		return data[(y*w+x)*3+band];
	}

	size_t w;
	size_t h;
	std::vector<uint8_t> data;
};

class RasterAreaImpl;

class RasterArea : public PlotLayerSub<RasterAreaImpl> {
public:
	RasterArea();

	~RasterArea() { }

	RasterArea &setBbox(Bbox bbox);
	RasterArea &setBboxFromDataSize();
	RasterArea &setSwapAxes(bool state=true);
	RasterArea &setBilinear(bool state=true);
	RawRGB &getDataBuf();
	CoordXform getAffine();

#ifdef SCOPEMM_ENABLE_BLITZ
	template <class T>
	RasterArea &setData(blitz::Array<T, 2> data);

	template <class T>
	RasterArea &setData(blitz::Array<T, 2> data, T min, T max);

	template <class T>
	RasterArea &setData(
		blitz::Array<T, 2> data_r,
		blitz::Array<T, 2> data_g,
		blitz::Array<T, 2> data_b
	);

	template <class T>
	RasterArea &setData(
		blitz::Array<T, 2> data_r,
		blitz::Array<T, 2> data_g,
		blitz::Array<T, 2> data_b,
		T min, T max
	);

	template <class T>
	RasterArea &setData(
		blitz::Array<T, 2> data_r, T min_r, T max_r,
		blitz::Array<T, 2> data_g, T min_g, T max_g,
		blitz::Array<T, 2> data_b, T min_b, T max_b
	);
#endif // SCOPEMM_ENABLE_BLITZ
};

#ifdef SCOPEMM_ENABLE_BLITZ
template <class T>
RasterArea &RasterArea::setData(
	blitz::Array<T, 2> data
) {
	setData(data, blitz::min(data), blitz::max(data));
	return *this;
}

template <class T>
RasterArea &RasterArea::setData(
	blitz::Array<T, 2> data,
	T min, T max
) {
	setData(
		data, min, max,
		data, min, max,
		data, min, max
	);
	return *this;
}

template <class T>
RasterArea &RasterArea::setData(
	blitz::Array<T, 2> data_r,
	blitz::Array<T, 2> data_g,
	blitz::Array<T, 2> data_b
) {
	setData(
		data_r, blitz::min(data_r), blitz::max(data_r),
		data_g, blitz::min(data_g), blitz::max(data_g),
		data_b, blitz::min(data_b), blitz::max(data_b)
	);
	return *this;
}

template <class T>
RasterArea &RasterArea::setData(
	blitz::Array<T, 2> data_r,
	blitz::Array<T, 2> data_g,
	blitz::Array<T, 2> data_b,
	T min, T max
) {
	setData(
		data_r, min, max,
		data_g, min, max,
		data_b, min, max
	);
	return *this;
}

template <class T>
RasterArea &RasterArea::setData(
	blitz::Array<T, 2> data_r, T min_r, T max_r,
	blitz::Array<T, 2> data_g, T min_g, T max_g,
	blitz::Array<T, 2> data_b, T min_b, T max_b
) {
	blitz::Array<T, 2> data[3] = { data_r, data_g, data_b };
	const size_t w = data[0].shape()[0];
	const size_t h = data[0].shape()[1];
	blitz::TinyVector<T, 3> min_vals(min_r, min_g, min_b);
	blitz::TinyVector<T, 3> max_vals(max_r, max_g, max_b);

	RawRGB &data_buf = getDataBuf();
	data_buf.resize(w, h);

	for(int band=0; band<3; band++) {
		T min = min_vals[band];
		T max = max_vals[band];
		for(size_t y=0; y<h; y++) {
			for(size_t x=0; x<w; x++) {
				T v = data[band](int(x), int(y));
				v = std::max(min, std::min(max, v));
				data_buf(x, y, band) = (min==max) ? uint8_t(0) : 
					uint8_t(255.0 * (v-min) / (max-min));
			}
		}
	}

	fireChangeEvent();
	return *this;
}
#endif // SCOPEMM_ENABLE_BLITZ

} // namespace scopemm

#endif // SCOPEMM_GRIDCANVAS_H
