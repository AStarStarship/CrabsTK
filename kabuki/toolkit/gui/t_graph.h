/*
    /kabuki/toolkit/gui/graph.h -- Simple graph widget for showing a function plot

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
 * \class Graph graph.h /kabuki/toolkit/gui/graph.h
 *
 * @brief Simple graph widget for showing a function plot.
 */
class SDK Graph : public Widget {
public:
    Graph(Widget *parent, const std::string &caption = "Untitled")
      : Widget (parent), mCaption (caption) {
      mBackgroundColor = Color (20, 128);
      mForegroundColor = Color (255, 192, 0, 128);
      mTextColor = Color (240, 192);
    }

    const std::string &caption() const { return mCaption; }
    void setCaption(const std::string &caption) { mCaption = caption; }

    const std::string &header() const { return mHeader; }
    void setHeader(const std::string &header) { mHeader = header; }

    const std::string &footer() const { return mFooter; }
    void setFooter(const std::string &footer) { mFooter = footer; }

    const Color &backgroundColor() const { return mBackgroundColor; }
    void setBackgroundColor(const Color &backgroundColor) { mBackgroundColor = backgroundColor; }

    const Color &foregroundColor() const { return mForegroundColor; }
    void setForegroundColor(const Color &foregroundColor) { mForegroundColor = 
    const Color &textColor() const { return mTextColor; }
    void setTextColor(const Color &textColor) { mTextColor = textColor; }

    const VectorXf &values() const { return mValues; }
    VectorXf &values() { return mValues; }
    void setValues(const VectorXf &values) { mValues = values; }

    virtual Vector2i preferredSize(NVGcontext *ctx) const override {
      return Vector2i (180, 45);
    }

    virtual void draw(NVGcontext *ctx) override {
      Widget::draw (ctx);

      nvgBeginPath (ctx);
      nvgRect (ctx, mPos.x (), mPos.y (), mSize.x (), mSize.y ());
      nvgFillColor (ctx, mBackgroundColor);
      nvgFill (ctx);

      if (mValues.size () < 2)
        return;

      nvgBeginPath (ctx);
      nvgMoveTo (ctx, mPos.x (), mPos.y () + mSize.y ());
      for (size_t i = 0; i < (size_t)mValues.size (); i++) {
        float value = mValues[i];
        float vx = mPos.x () + i * mSize.x () / (float)(mValues.size () - 1);
        float vy = mPos.y () + (1 - value) * mSize.y ();
        nvgLineTo (ctx, vx, vy);
      }

      nvgLineTo (ctx, mPos.x () + mSize.x (), mPos.y () + mSize.y ());
      nvgStrokeColor (ctx, Color (100, 255));
      nvgStroke (ctx);
      nvgFillColor (ctx, mForegroundColor);
      nvgFill (ctx);

      nvgFontFace (ctx, "sans");

      if (!mCaption.empty ()) {
        nvgFontSize (ctx, 14.0f);
        nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgFillColor (ctx, mTextColor);
        nvgText (ctx, mPos.x () + 3, mPos.y () + 1, mCaption.c_str (), NULL);
      }

      if (!mHeader.empty ()) {
        nvgFontSize (ctx, 18.0f);
        nvgTextAlign (ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
        nvgFillColor (ctx, mTextColor);
        nvgText (ctx, mPos.x () + mSize.x () - 3, mPos.y () + 1, mHeader.c_str (), NULL);
      }

      if (!mFooter.empty ()) {
        nvgFontSize (ctx, 15.0f);
        nvgTextAlign (ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM);
        nvgFillColor (ctx, mTextColor);
        nvgText (ctx, mPos.x () + mSize.x () - 3, mPos.y () + mSize.y () - 1, mFooter.c_str (), NULL);
      }

      nvgBeginPath (ctx);
      nvgRect (ctx, mPos.x (), mPos.y (), mSize.x (), mSize.y ());
      nvgStrokeColor (ctx, Color (100, 255));
      nvgStroke (ctx);
    }

    virtual void save(Serializer &s) const override {
      Widget::save (s);
      s.set ("caption", mCaption);
      s.set ("header", mHeader);
      s.set ("footer", mFooter);
      s.set ("backgroundColor", mBackgroundColor);
      s.set ("foregroundColor", mForegroundColor);
      s.set ("textColor", mTextColor);
      s.set ("values", mValues);
    }

    virtual bool load(Serializer &s) override {
      if (!Widget::load (s)) return false;
      if (!s.get ("caption", mCaption)) return false;
      if (!s.get ("header", mHeader)) return false;
      if (!s.get ("footer", mFooter)) return false;
      if (!s.get ("backgroundColor", mBackgroundColor)) return false;
      if (!s.get ("foregroundColor", mForegroundColor)) return false;
      if (!s.get ("textColor", mTextColor)) return false;
      if (!s.get ("values", mValues)) return false;
      return true;
    }

protected:
    std::string mCaption, mHeader, mFooter;
    Color mBackgroundColor, mForegroundColor, mTextColor;
    VectorXf mValues;
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
