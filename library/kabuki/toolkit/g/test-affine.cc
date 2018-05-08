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

#include <iostream>

#include "scopemm/affine.h"
using namespace scopemm;

int main() {
	AffineTransform a(
		2, 1.2,
		3.5, 5,
		10, 20
	);
	AffineTransform b(
		2.7, 6.2,
		3.1, 2.3,
		18, 13.5
	);

	double x=1.23, y=7;
	std::cout << x << "," << y << std::endl;
	a(x, y, x, y);
	std::cout << x << "," << y << std::endl;
	b(x, y, x, y);
	std::cout << x << "," << y << std::endl;
	(b*a).inverse()(x, y, x, y);
	std::cout << x << "," << y << std::endl;

	CoordXform c = CoordXform::boxToBox(
		Bbox(1, -4, 3, 3.3),
		Bbox(3.2, -10, 2.7, 1.1),
		true
	);
	x=1.23; y=7;
	c.fwd(x, y, x, y);
	std::cout << x << "," << y << std::endl;
	c.inv(x, y, x, y);
	std::cout << x << "," << y << std::endl;
}
