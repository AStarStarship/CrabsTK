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

#ifndef SCOPEMM_AFFINE_H
#define SCOPEMM_AFFINE_H

#include <assert.h>

#include "scopemm/bbox.h"

namespace scopemm {

class AffineTransform {
public:
	static const AffineTransform IDENTITY;
	static const AffineTransform SWAP_AXES;

	AffineTransform(
		double _m00, double _m10,
		double _m01, double _m11,
		double _m02, double _m12
	) :
		m00(_m00), m10(_m10),
		m01(_m01), m11(_m11),
		m02(_m02), m12(_m12)
	{ }
		
	void operator()(double xi, double yi, double &xo, double &yo) const {
		xo = m00*xi + m01*yi + m02;
		yo = m10*xi + m11*yi + m12;
	}

	AffineTransform &operator*=(const AffineTransform &b) {
		double c00 = m00*b.m00 + m01*b.m10;
		double c10 = m10*b.m00 + m11*b.m10;
		double c01 = m00*b.m01 + m01*b.m11;
		double c11 = m10*b.m01 + m11*b.m11;
		double c02, c12;
		(*this)(b.m02, b.m12, c02, c12);
		m00 = c00; m10 = c10;
		m01 = c01; m11 = c11;
		m02 = c02; m12 = c12;
		return *this;
	}

	AffineTransform operator*(const AffineTransform &b) const {
		return AffineTransform(*this) *= b;
	}

	AffineTransform inverse() {
		double det = m00*m11 - m10*m01;
		assert(det);
		double i00 = m11/det;
		double i11 = m00/det;
		double i10 = -m10/det;
		double i01 = -m01/det;
		double i02 = -i00*m02 -i01*m12;
		double i12 = -i10*m02 -i11*m12;
		return AffineTransform(i00, i10, i01, i11, i02, i12);
	}

	static AffineTransform boxToBox(Bbox from, Bbox to) {
		double sx = 
			(from.xmax-from.xmin)
			? (to.xmax-to.xmin) / (from.xmax-from.xmin)
			: 1;
		double sy = 
			(from.ymax-from.ymin)
			? (to.ymax-to.ymin) / (from.ymax-from.ymin)
			: 1;
		double dx = to.xmin - from.xmin*sx;
		double dy = to.ymin - from.ymin*sy;
		return AffineTransform(sx, 0.0, 0.0, sy, dx, dy);
	}

	double m00, m10, m01, m11, m02, m12;
};

class CoordXform {
public:
	CoordXform() :
		fwd(AffineTransform::IDENTITY),
		inv(AffineTransform::IDENTITY)
	{ }

//	CoordXform(AffineTransform _fwd) :
//		fwd(_fwd),
//		inv(fwd.inverse())
//	{ }

	CoordXform(AffineTransform _fwd, AffineTransform _inv) :
		fwd(_fwd),
		inv(_inv)
	{ }

	static CoordXform boxToBox(Bbox from, Bbox to, bool swap_axes=false) {
		// NOTE: if either of the bounding boxes is degenerate, this will
		// still produce usable transforms, although they will not really
		// be inverses of each other
		if(swap_axes) {
			const AffineTransform sa = AffineTransform::SWAP_AXES;
			return CoordXform(
				AffineTransform::boxToBox(from.transpose(), to) * sa,
				sa * AffineTransform::boxToBox(to, from.transpose())
			);
		} else {
			return CoordXform(
				AffineTransform::boxToBox(from, to),
				AffineTransform::boxToBox(to, from)
			);
		}
	}

	AffineTransform fwd;
	AffineTransform inv;
};

} // namespace scopemm

#endif // SCOPEMM_AFFINE_H
