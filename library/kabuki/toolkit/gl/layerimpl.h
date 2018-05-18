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

#ifndef SCOPEMM_LAYERIMPL_H
#define SCOPEMM_LAYERIMPL_H

// NOTE: don't include this in the public interface: if this is only referenced
// by the scopemm-*.cc files then the library is protected against ABI
// incompatibilities.

#include <set>

#include <gtkmm/main.h>
#include <gtkmm/drawingarea.h>

namespace scopemm {

class PlotLayerImplBase : private boost::noncopyable {
public:
	PlotLayerImplBase() : zorder(0) { }

	virtual ~PlotLayerImplBase() { }
	virtual void draw(PlotCanvas *parent, Cairo::RefPtr<Cairo::Context>) = 0;
	virtual bool hasMinMax() const { return false; }
	virtual Bbox getBbox() const { assert(0); }

	std::set<PlotCanvas *> change_listeners;
	double zorder;
};

} // namespace scopemm

#endif // SCOPEMM_LAYERIMPL_H
