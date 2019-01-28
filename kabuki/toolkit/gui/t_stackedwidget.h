/*
    /kabuki/toolkit/gui/stackedwidget.h -- Widget used to stack widgets on top
    of each other. Only the active widget is visible.

    The stacked widget was contributed by Stefan Ivanov.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


#pragma once
#include <pch.h>


#include "t_widget.h"

namespace _ {

/*
 * \class StackedWidget stackedwidget.h /kabuki/toolkit/gui/stackedwidget.h
 *
 * @brief A stack widget.
 */
class SDK StackedWidget : public Widget {
public:
  StackedWidget (Widget* parent)
    : Widget (parent) { }

  void setSelectedIndex (int index) {
    DASSERT (index < childCount ());
    if (mSelectedIndex >= 0)
      mChildren[mSelectedIndex]->setVisible (false);
    mSelectedIndex = index;
    mChildren[mSelectedIndex]->setVisible (true);
  }

  int selectedIndex () const {
    return mSelectedIndex;
  }

  virtual void performLayout (NVGcontext* ctx) override {
    for (auto child : mChildren) {
      child->setPosition (Vector2i::Zero ());
      child->setSize (mSize);
      child->performLayout (ctx);
    }
  }

  virtual Vector2i preferredSize (NVGcontext* ctx) const override {
    Vector2i size = Vector2i::Zero ();
    for (auto child : mChildren)
      size = size.cwiseMax (child->preferredSize (ctx));
    return size;
  }

  virtual void addChild (int index, Widget* widget) override {
    if (mSelectedIndex >= 0)
      mChildren[mSelectedIndex]->setVisible (false);
    Widget::addChild (index, widget);
    widget->setVisible (true);
    setSelectedIndex (index);
  }

private:
  int mSelectedIndex = -1;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
