/*
    /kabuki/toolkit/gui/progressbar.h -- Standard widget for visualizing progress

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


#pragma once
#include <pch.h>


#include "t_widget.h"
#include "t_opengl.h"
#include "t_serializer.h"

namespace _ {

/* Standard widget for visualizing progress. */
class SDK ProgressBar : public Widget {
public:
  ProgressBar (Widget *parent)
    : Widget (parent), mValue (0.0f) {}

  float value () { return mValue; }
  void setValue (float value) { mValue = value; }

  virtual Vector2i preferredSize (NVGcontext *ctx) const override const {
    return Vector2i (70, 12);
  }

  virtual void draw (NVGcontext* ctx) override {
    Widget::draw (ctx);

    NVGpaint paint = nvgBoxGradient (
      ctx, mPos.x () + 1, mPos.y () + 1,
      mSize.x () - 2, mSize.y (), 3, 4, Color (0, 32), Color (0, 92));
    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x (), mPos.y (), mSize.x (), mSize.y (), 3);
    nvgFillPaint (ctx, paint);
    nvgFill (ctx);

    float value = std::min (std::max (0.0f, mValue), 1.0f);
    int barPos = (int)std::round ((mSize.x () - 2) * value);

    paint = nvgBoxGradient (
      ctx, mPos.x (), mPos.y (),
      barPos + 1.5f, mSize.y () - 1, 3, 4,
      Color (220, 100), Color (128, 100));

    nvgBeginPath (ctx);
    nvgRoundedRect (
      ctx, mPos.x () + 1, mPos.y () + 1,
      barPos, mSize.y () - 2, 3);
    nvgFillPaint (ctx, paint);
    nvgFill (ctx);
  }

  virtual void save (Serializer &s) const override {
    Widget::save (s);
    s.set ("value", mValue);
  }

  virtual bool load (Serializer &s) override {
    if (!Widget::load (s))
      return false;
    if (!s.get ("value", mValue))
      return false;
    return true;
  }

protected:
  float mValue;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
