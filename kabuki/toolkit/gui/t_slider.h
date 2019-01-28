/*
    /kabuki/toolkit/gui/slider.h -- Fractional slider widget with mouse control

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


#pragma once
#include <pch.h>


#include "t_widget.h"
#include "t_theme.h"
#include "t_opengl.h"
#include "t_serializer.h"

namespace _ {

/*
 * \class Slider slider.h /kabuki/toolkit/gui/slider.h
 *
 * @brief Fractional slider widget with mouse control.
 */
class SDK Slider : public Widget {
public:
  Slider (Widget *parent)
    : Widget (parent), mValue (0.0f), mRange (0.f, 1.f),
    mHighlightedRange (0.f, 0.f) {
    mHighlightColor = Color (255, 80, 80, 70);
  }

  float value () const { return mValue; }
  void setValue (float value) { mValue = value; }

  const Color &highlightColor () const { return mHighlightColor; }
  void setHighlightColor (const Color &highlightColor) { mHighlightColor = highlightColor; }

  std::pair<float, float> range () const { return mRange; }
  void setRange (std::pair<float, float> range) { mRange = range; }

  std::pair<float, float> highlightedRange () const { return mHighlightedRange; }
  void setHighlightedRange (std::pair<float, float> highlightedRange) { mHighlightedRange = highlightedRange; }

  std::function<void (float)> callback () const { return mCallback; }
  void setCallback (const std::function<void (float)> &callback) { mCallback = callback; }

  std::function<void (float)> finalCallback () const { return mFinalCallback; }
  void setFinalCallback (const std::function<void (float)> &callback) { mFinalCallback = callback; }

  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    return Vector2i (70, 16);
  }

  virtual bool mouseDragEvent (const Vector2i &p, const Vector2i &rel,
    int button, int modifiers) override {
    if (!mEnabled)
      return false;

    const float kr = (int)(mSize.y () * 0.4f), kshadow = 3;
    const float startX = kr + kshadow + mPos.x () - 1;
    const float widthX = mSize.x () - 2 * (kr + kshadow);

    float value = (p.x () - startX) / widthX;
    value = value * (mRange.second - mRange.first) + mRange.first;
    mValue = std::min (std::max (value, mRange.first), mRange.second);
    if (mCallback)
      mCallback (mValue);
    return true;
  }

  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down, int modifiers) override {
    if (!mEnabled)
      return false;

    const float kr = (int)(mSize.y () * 0.4f), kshadow = 3;
    const float startX = kr + kshadow + mPos.x () - 1;
    const float widthX = mSize.x () - 2 * (kr + kshadow);

    float value = (p.x () - startX) / widthX;
    value = value * (mRange.second - mRange.first) + mRange.first;
    mValue = std::min (std::max (value, mRange.first), mRange.second);
    if (mCallback)
      mCallback (mValue);
    if (mFinalCallback && !down)
      mFinalCallback (mValue);
    return true;
  }

  virtual void draw (NVGcontext* ctx) override {
    Vector2f center = mPos.cast<float> () + mSize.cast<float> () * 0.5f;
    float kr = (int)(mSize.y () * 0.4f), kshadow = 3;

    float startX = kr + kshadow + mPos.x ();
    float widthX = mSize.x () - 2 * (kr + kshadow);

    Vector2f knobPos (startX + (mValue - mRange.first) /
      (mRange.second - mRange.first) * widthX,
      center.y () + 0.5f);

    NVGpaint bg = nvgBoxGradient (
      ctx, startX, center.y () - 3 + 1, widthX, 6, 3, 3,
      Color (0, mEnabled ? 32 : 10), Color (0, mEnabled ? 128 : 210));

    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, startX, center.y () - 3 + 1, widthX, 6, 2);
    nvgFillPaint (ctx, bg);
    nvgFill (ctx);

    if (mHighlightedRange.second != mHighlightedRange.first) {
      nvgBeginPath (ctx);
      nvgRoundedRect (ctx, startX + mHighlightedRange.first * mSize.x (),
        center.y () - kshadow + 1,
        widthX *
        (mHighlightedRange.second - mHighlightedRange.first),
        kshadow * 2, 2);
      nvgFillColor (ctx, mHighlightColor);
      nvgFill (ctx);
    }

    NVGpaint knobShadow =
      nvgRadialGradient (ctx, knobPos.x (), knobPos.y (), kr - kshadow,
        kr + kshadow, Color (0, 64), mTheme->mTransparent);

    nvgBeginPath (ctx);
    nvgRect (ctx, knobPos.x () - kr - 5, knobPos.y () - kr - 5, kr * 2 + 10,
      kr * 2 + 10 + kshadow);
    nvgCircle (ctx, knobPos.x (), knobPos.y (), kr);
    nvgPathWinding (ctx, NVG_HOLE);
    nvgFillPaint (ctx, knobShadow);
    nvgFill (ctx);

    NVGpaint knob = nvgLinearGradient (ctx,
      mPos.x (), center.y () - kr, mPos.x (), center.y () + kr,
      mTheme->mBorderLight, mTheme->mBorderMedium);
    NVGpaint knobReverse = nvgLinearGradient (ctx,
      mPos.x (), center.y () - kr, mPos.x (), center.y () + kr,
      mTheme->mBorderMedium,
      mTheme->mBorderLight);

    nvgBeginPath (ctx);
    nvgCircle (ctx, knobPos.x (), knobPos.y (), kr);
    nvgStrokeColor (ctx, mTheme->mBorderDark);
    nvgFillPaint (ctx, knob);
    nvgStroke (ctx);
    nvgFill (ctx);
    nvgBeginPath (ctx);
    nvgCircle (ctx, knobPos.x (), knobPos.y (), kr / 2);
    nvgFillColor (ctx, Color (150, mEnabled ? 255 : 100));
    nvgStrokePaint (ctx, knobReverse);
    nvgStroke (ctx);
    nvgFill (ctx);
  }

  virtual void save (Serializer &s) const override {
    Widget::save (s);
    s.set ("value", mValue);
    s.set ("range", mRange);
    s.set ("highlightedRange", mHighlightedRange);
    s.set ("highlightColor", mHighlightColor);
  }

  virtual bool load (Serializer &s) override {
    if (!Widget::load (s)) return false;
    if (!s.get ("value", mValue)) return false;
    if (!s.get ("range", mRange)) return false;
    if (!s.get ("highlightedRange", mHighlightedRange)) return false;
    if (!s.get ("highlightColor", mHighlightColor)) return false;
    return true;
  }

protected:
  float mValue;
  std::function<void (float)> mCallback;
  std::function<void (float)> mFinalCallback;
  std::pair<float, float> mRange;
  std::pair<float, float> mHighlightedRange;
  Color mHighlightColor;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
