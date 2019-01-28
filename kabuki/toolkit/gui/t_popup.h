/*
    /kabuki/toolkit/gui/popup.h -- Simple popup widget which is attached to another given
    window (can be nested)

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once
#include <pch.h>


#include "t_window.h"
#include "t_theme.h"
#include "t_opengl.h"
#include "t_serializer.h"

namespace _ {

/*
 * \class Popup popup.h /kabuki/toolkit/gui/popup.h
 *
 * @brief Popup window for combo boxes, popup buttons, nested dialogs etc.
 *
 * Usually the Popup instance is constructed by another widget (e.g. @ref PopupButton)
 * and does not need to be created by hand.
 */
class SDK Popup : public Window {
public:
  enum Side { Left = 0, Right };

  // Create a new popup parented to a screen (first argument) and a parent window
  Popup (Widget *parent, Window *parentWindow)
    : Window (parent, ""), mParentWindow (parentWindow),
    mAnchorPos (Vector2i::Zero ()), mAnchorHeight (30), mSide (Side::Right) {
  }

  // Return the anchor position in the parent window; the placement of the popup is relative to it
  void setAnchorPos (const Vector2i &anchorPos) { mAnchorPos = anchorPos; }
  // Set the anchor position in the parent window; the placement of the popup is relative to it
  const Vector2i &anchorPos () const { return mAnchorPos; }

  // Set the anchor height; this determines the vertical shift relative to the anchor position
  void setAnchorHeight (int anchorHeight) { mAnchorHeight = anchorHeight; }
  // Return the anchor height; this determines the vertical shift relative to the anchor position
  int anchorHeight () const { return mAnchorHeight; }

  // Set the side of the parent window at which popup will appear
  void setSide (Side popupSide) { mSide = popupSide; }
  // Return the side of the parent window at which popup will appear
  Side side () const { return mSide; }

  // Return the parent window of the popup
  Window *parentWindow () { return mParentWindow; }
  // Return the parent window of the popup
  const Window *parentWindow () const { return mParentWindow; }

  // Invoke the associated layout generator to properly place child widgets, if any
  virtual void performLayout (NVGcontext *ctx) override {
    if (mLayout || mChildren.size () != 1) {
      Widget::performLayout (ctx);
    } else {
      mChildren[0]->setPosition (Vector2i::Zero ());
      mChildren[0]->setSize (mSize);
      mChildren[0]->performLayout (ctx);
    }
    if (mSide == Side::Left)
      mAnchorPos[0] -= size ()[0];
  }

  // Draw the popup window
  virtual void draw (NVGcontext* ctx) override {
    refreshRelativePlacement ();

    if (!mVisible)
      return;

    int ds = mTheme->mWindowDropShadowSize, cr = mTheme->mWindowCornerRadius;

    nvgSave (ctx);
    nvgResetScissor (ctx);

    /* Draw a drop shadow */
    NVGpaint shadowPaint = nvgBoxGradient (
      ctx, mPos.x (), mPos.y (), mSize.x (), mSize.y (), cr * 2, ds * 2,
      mTheme->mDropShadow, mTheme->mTransparent);

    nvgBeginPath (ctx);
    nvgRect (ctx, mPos.x () - ds, mPos.y () - ds, mSize.x () + 2 * ds, mSize.y () + 2 * ds);
    nvgRoundedRect (ctx, mPos.x (), mPos.y (), mSize.x (), mSize.y (), cr);
    nvgPathWinding (ctx, NVG_HOLE);
    nvgFillPaint (ctx, shadowPaint);
    nvgFill (ctx);

    /* Draw window */
    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x (), mPos.y (), mSize.x (), mSize.y (), cr);

    Vector2i base = mPos + Vector2i (0, mAnchorHeight);
    int sign = -1;
    if (mSide == Side::Left) {
      base.x () += mSize.x ();
      sign = 1;
    }

    nvgMoveTo (ctx, base.x () + 15 * sign, base.y ());
    nvgLineTo (ctx, base.x () - 1 * sign, base.y () - 15);
    nvgLineTo (ctx, base.x () - 1 * sign, base.y () + 15);

    nvgFillColor (ctx, mTheme->mWindowPopup);
    nvgFill (ctx);
    nvgRestore (ctx);

    Widget::draw (ctx);
  }

  virtual void save (Serializer &s) const override {
    Window::save (s);
    s.set ("anchorPos", mAnchorPos);
    s.set ("anchorHeight", mAnchorHeight);
    s.set ("side", mSide);
  }

  virtual bool load (Serializer &s) override {
    if (!Window::load (s)) return false;
    if (!s.get ("anchorPos", mAnchorPos)) return false;
    if (!s.get ("anchorHeight", mAnchorHeight)) return false;
    if (!s.get ("side", mSide)) return false;
    return true;
  }

protected:
  // Internal helper function to maintain nested window position values
  virtual void refreshRelativePlacement () override {
    mParentWindow->refreshRelativePlacement ();
    mVisible &= mParentWindow->visibleRecursive ();
    mPos = mParentWindow->position () + mAnchorPos - Vector2i (0, mAnchorHeight);
  }

protected:
  Window *mParentWindow;
  Vector2i mAnchorPos;
  int mAnchorHeight;
  Side mSide;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
