/*
    /kabuki/toolkit/gui/vscrollpanel.h -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

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
 * \class VScrollPanel vscrollpanel.h /kabuki/toolkit/gui/vscrollpanel.h
 *
 * @brief Adds a vertical scrollbar around a widget that is too big to fit into
 *        a certain area.
 */
class SDK VScrollPanel : public Widget {
public:
  VScrollPanel (Widget *parent)
    : Widget (parent),
    mChildPreferredHeight (0),
    mScroll (0.0f),
    mUpdateLayout (false) {}

  // Return the current scroll amount as a value between 0 and 1. 0 means scrolled to the top and 1 to the bottom.
  float scroll () const { return mScroll; }
  // Set the scroll amount to a value between 0 and 1. 0 means scrolled to the top and 1 to the bottom.
  void setScroll (float scroll) { mScroll = scroll; }

  virtual void performLayout (NVGcontext *ctx) override {
    Widget::performLayout (ctx);

    if (mChildren.empty ())
      return;
    if (mChildren.size () > 1)
      throw std::runtime_error ("VScrollPanel should have one child.");

    Widget *child = mChildren[0];
    mChildPreferredHeight = child->preferredSize (ctx).y ();

    if (mChildPreferredHeight > mSize.y ()) {
      child->setPosition (Vector2i (0, -mScroll * (mChildPreferredHeight - mSize.y ())));
      child->setSize (Vector2i (mSize.x () - 12, mChildPreferredHeight));
    } else {
      child->setPosition (Vector2i::Zero ());
      child->setSize (mSize);
      mScroll = 0;
    }
    child->performLayout (ctx);
  }

  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    if (mChildren.empty ())
      return Vector2i::Zero ();
    return mChildren[0]->preferredSize (ctx) + Vector2i (12, 0);
  }

  virtual bool mouseDragEvent (const Vector2i &p, const Vector2i &rel,
    int button, int modifiers) override {
    if (!mChildren.empty () && mChildPreferredHeight > mSize.y ()) {
      float scrollh = height () *
        std::min (1.0f, height () / (float)mChildPreferredHeight);

      mScroll = std::max ((float) 0.0f, std::min ((float) 1.0f,
        mScroll + rel.y () / (float)(mSize.y () - 8 - scrollh)));
      mUpdateLayout = true;
      return true;
    } else {
      return Widget::mouseDragEvent (p, rel, button, modifiers);
    }
  }

  virtual bool scrollEvent (const Vector2i &p, const Vector2f &rel) override {
    if (!mChildren.empty () && mChildPreferredHeight > mSize.y ()) {
      float scrollAmount = rel.y () * (mSize.y () / 20.0f);
      float scrollh = height () *
        std::min (1.0f, height () / (float)mChildPreferredHeight);

      mScroll = std::max ((float) 0.0f, std::min ((float) 1.0f,
        mScroll - scrollAmount / (float)(mSize.y () - 8 - scrollh)));
      mUpdateLayout = true;
      return true;
    } else {
      return Widget::scrollEvent (p, rel);
    }
  }

  virtual void draw (NVGcontext *ctx) override {
    if (mChildren.empty ())
      return;
    Widget *child = mChildren[0];
    child->setPosition (Vector2i (0, -mScroll * (mChildPreferredHeight - mSize.y ())));
    mChildPreferredHeight = child->preferredSize (ctx).y ();
    float scrollh = height () *
      std::min (1.0f, height () / (float)mChildPreferredHeight);

    if (mUpdateLayout)
      child->performLayout (ctx);

    nvgSave (ctx);
    nvgTranslate (ctx, mPos.x (), mPos.y ());
    nvgIntersectScissor (ctx, 0, 0, mSize.x (), mSize.y ());
    if (child->visible ())
      child->draw (ctx);
    nvgRestore (ctx);

    if (mChildPreferredHeight <= mSize.y ())
      return;

    NVGpaint paint = nvgBoxGradient (
      ctx, mPos.x () + mSize.x () - 12 + 1, mPos.y () + 4 + 1, 8,
      mSize.y () - 8, 3, 4, Color (0, 32), Color (0, 92));
    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x () + mSize.x () - 12, mPos.y () + 4, 8,
      mSize.y () - 8, 3);
    nvgFillPaint (ctx, paint);
    nvgFill (ctx);

    paint = nvgBoxGradient (
      ctx, mPos.x () + mSize.x () - 12 - 1,
      mPos.y () + 4 + (mSize.y () - 8 - scrollh) * mScroll - 1, 8, scrollh,
      3, 4, Color (220, 100), Color (128, 100));

    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x () + mSize.x () - 12 + 1,
      mPos.y () + 4 + 1 + (mSize.y () - 8 - scrollh) * mScroll, 8 - 2,
      scrollh - 2, 2);
    nvgFillPaint (ctx, paint);
    nvgFill (ctx);
  }

  virtual void save (Serializer &s) const override {
    Widget::save (s);
    s.set ("childPreferredHeight", mChildPreferredHeight);
    s.set ("scroll", mScroll);
  }

  virtual bool load (Serializer &s) override {
    if (!Widget::load (s)) return false;
    if (!s.get ("childPreferredHeight", mChildPreferredHeight)) return false;
    if (!s.get ("scroll", mScroll)) return false;
    return true;
  }

protected:
  int mChildPreferredHeight;
  float mScroll;
  bool mUpdateLayout;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
