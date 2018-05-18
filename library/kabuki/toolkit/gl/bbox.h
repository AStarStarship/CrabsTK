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

#ifndef SCOPEMM_BBOX_H
#define SCOPEMM_BBOX_H

#include <algorithm>
#include <assert.h>

namespace scopemm {

class Bbox {
public:
	Bbox(
		double _xmin, double _ymin, 
		double _xmax, double _ymax
	) :
		xmin(_xmin), ymin(_ymin), 
		xmax(_xmax), ymax(_ymax)
	{ }

	Bbox transpose() const {
		return Bbox(ymin, xmin, ymax, xmax);
	}

	Bbox normalize() const {
		return Bbox(
			std::min(xmin, xmax),
			std::min(ymin, ymax),
			std::max(xmin, xmax),
			std::max(ymin, ymax)
		);
	}

	Bbox &operator|=(const Bbox &b) {
		xmin = std::min(xmin, b.xmin);
		ymin = std::min(ymin, b.ymin);
		xmax = std::max(xmax, b.xmax);
		ymax = std::max(ymax, b.ymax);
		return *this;
	}

	Bbox operator|(const Bbox &b) const {
		return Bbox(*this) |= b;
	}

	double xmin, ymin, xmax, ymax;
};

} // namespace scopemm

#endif // SCOPEMM_BBOX_H
