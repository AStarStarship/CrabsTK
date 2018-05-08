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

#ifndef SCOPEMM_LINEPLOT_H
#define SCOPEMM_LINEPLOT_H

#include <utility>
#include <vector>

#include "scopemm/plotcanvas.h"

namespace scopemm {

class PlotTraceImpl;

class PlotTrace : public PlotLayerSub<PlotTraceImpl> {
public:
	PlotTrace();

	~PlotTrace() { }

	PlotTrace& setColor(double r, double g, double b);

	std::vector<double> &getXPts();
	std::vector<double> &getYPts();

	template <class IterY>
	PlotTrace& setYData(std::pair<IterY, IterY> yiter, bool steps=false);

	// this handles the blitz::Array<double, 1> type, as well as STL containers
	template <class ArrY>
	PlotTrace& setYData(ArrY ydata, bool steps=false);

	template <class ArrX, class ArrY>
	PlotTrace& setXYData(ArrX xarr, ArrY yarr);

	template <class IterX, class IterY>
	PlotTrace& setXYData(
		std::pair<IterX, IterX> xiter, 
		std::pair<IterY, IterY> yiter
	);

	template <class ArrXY>
	PlotTrace& setXYData(ArrXY xyarr);

	template <class IterXY>
	PlotTrace& setXYData(std::pair<IterXY, IterXY> xyiter);
};

template <class IterY>
PlotTrace& PlotTrace::setYData(std::pair<IterY, IterY> yiter, bool steps) {
	std::vector<double> &xpts = getXPts();
	std::vector<double> &ypts = getYPts();

	size_t npts;
	if(steps) {
		ypts.clear();
		for(IterY p=yiter.first; p!=yiter.second; ++p) {
			ypts.push_back(*p);
			ypts.push_back(*p);
		}
		npts = ypts.size()/2;
	} else {
		ypts.assign(yiter.first, yiter.second);
		npts = ypts.size();
	}

	xpts.clear();
	if(steps) {
		for(size_t i=0; i<npts; ++i) {
			xpts.push_back(double(i));
			xpts.push_back(double(i+1));
		}
	} else {
		for(size_t i=0; i<npts; ++i) xpts.push_back(double(i));
	}

	fireChangeEvent();

	return *this;
}

template <class ArrY>
PlotTrace& PlotTrace::setYData(ArrY ydata, bool steps) {
	return setYData(std::make_pair(ydata.begin(), ydata.end()), steps);
}

template <class IterX, class IterY>
PlotTrace& PlotTrace::setXYData(
	std::pair<IterX, IterX> xiter, 
	std::pair<IterY, IterY> yiter
) {
	getXPts().assign(xiter.first, xiter.second);
	getYPts().assign(yiter.first, yiter.second);

	assert(getXPts().size() == getYPts().size());

	fireChangeEvent();

	return *this;
}

template <class IterXY>
PlotTrace& PlotTrace::setXYData(std::pair<IterXY, IterXY> xyiter) {
	std::vector<double> &xpts = getXPts();
	std::vector<double> &ypts = getYPts();

	xpts.clear();
	ypts.clear();

	for(IterXY p=xyiter.first; p!=xyiter.second; ++p) {
		xpts.push_back(p->first);
		ypts.push_back(p->second);
	}

	fireChangeEvent();

	return *this;
}

template <class ArrX, class ArrY>
PlotTrace& PlotTrace::setXYData(ArrX xarr, ArrY yarr) {
	setXYData(
		std::make_pair(xarr.begin(), xarr.end()),
		std::make_pair(yarr.begin(), yarr.end())
	);
	return *this;
}

template <class ArrXY>
PlotTrace& PlotTrace::setXYData(ArrXY xyarr) {
	setXYData(std::make_pair(xyarr.begin(), xyarr.end()));
	return *this;
}

} // namespace scopemm

#endif // SCOPEMM_LINEPLOT_H
