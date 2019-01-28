/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/*
 * @file /kabuki/toolkit/gui/checkbox.h
 *
 * @brief Two-state check box Widget.
 */

#pragma once
#include <pch.h>


#include "t_widget.h"

namespace _ {

/* Two-state check box widget.
This class overrides @ref _::Widget::mIconExtraScale to be `1.2f`,
which affects all subclasses of this Widget.  Subclasses must explicitly
set a different value if needed (e.g., in their constructor). */
class SDK CheckBox : public Widget {
public:
  /* Adds a CheckBox to the specified `parent`.
  @param parent The Widget to add this CheckBox to.
  @param caption The caption text of the CheckBox (default `"Untitled"`).
  @param callback If provided, the callback to execute when the CheckBox
  is checked or unchecked.  Default parameter function does nothing.
  See @ref _::CheckBox::mPushed for the difference between "pushed" and
  "checked". */
  CheckBox (Widget *parent, const std::string &caption = "Untitled",
    const std::function<void (bool)> &callback =
    std::function<void (bool)> ())
    : Widget (parent), mCaption (caption), mPushed (false), mChecked (false),
    mCallback (callback) {
    mIconExtraScale = 1.2f;// widget override
  }

  // The caption of this CheckBox.
  const std::string &caption () const { return mCaption; }

  // Sets the caption of this CheckBox.
  void setCaption (const std::string &caption) { mCaption = caption; }

  // Whether or not this CheckBox is currently checked.
  const bool &checked () const { return mChecked; }

  // Sets whether or not this CheckBox is currently checked.
  void setChecked (const bool &checked) { mChecked = checked; }

  // Whether or not this CheckBox is currently pushed.  See @ref _::CheckBox::mPushed.
  const bool &pushed () const { return mPushed; }

  // Sets whether or not this CheckBox is currently pushed.  See @ref _::CheckBox::mPushed.
  void setPushed (const bool &pushed) { mPushed = pushed; }

  // Returns the current callback of this CheckBox.
  std::function<void (bool)> callback () const { return mCallback; }

  // Sets the callback to be executed when this CheckBox is checked / unchecked.
  void setCallback (const std::function<void (bool)> &callback) { mCallback = callback; }

  /* The mouse button callback will return `true` when all three conditions
  described are met.
  1. This CheckBox is "enabled" (see @ref _::Widget::mEnabled).
  2. `p` is inside this CheckBox.
  3. `button` is `GLFW_MOUSE_BUTTON_1` (left mouse click).

  Since a mouse button event is issued for both when the mouse is pressed, as
  well as released, this function sets @ref _::CheckBox::mPushed to `true` when parameter `down == true`.  When the second event (`down == false`) is fired, @ref _::CheckBox::mChecked is inverted and
  @ref _::CheckBox::mCallback is called.

  That is, the callback provided is only called when the mouse button is
  released, **and** the click location remains within the CheckBox boundaries.
  If the user clicks on the CheckBox and releases away from the bounds of the
  CheckBox,
  * @ref _::CheckBox::mPushed is simply set back to `false`. */
  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down,
    int modifiers) override {
    Widget::mouseButtonEvent (p, button, down, modifiers);
    if (!mEnabled)
      return false;

    if (button == GLFW_MOUSE_BUTTON_1) {
      if (down) {
        mPushed = true;
      } else if (mPushed) {
        if (contains (p)) {
          mChecked = !mChecked;
          if (mCallback)
            mCallback (mChecked);
        }
        mPushed = false;
      }
      return true;
    }
    return false;
  }

  // The preferred size of this CheckBox.
  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    if (mFixedSize != Vector2i::Zero ())
      return mFixedSize;
    nvgFontSize (ctx, fontSize ());
    nvgFontFace (ctx, "sans");
    return Vector2i (
      nvgTextBounds (ctx, 0, 0, mCaption.c_str (), nullptr, nullptr) +
      1.8f * fontSize (),
      fontSize () * 1.3f);
  }

  // Draws this CheckBox.
  virtual void draw (NVGcontext *ctx) override {
    Widget::draw (ctx);

    nvgFontSize (ctx, fontSize ());
    nvgFontFace (ctx, "sans");
    nvgFillColor (ctx,
      mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor);
    nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgText (ctx, mPos.x () + 1.6f * fontSize (), mPos.y () + mSize.y () * 0.5f,
      mCaption.c_str (), nullptr);

    NVGpaint bg = nvgBoxGradient (ctx, mPos.x () + 1.5f, mPos.y () + 1.5f,
      mSize.y () - 2.0f, mSize.y () - 2.0f, 3, 3,
      mPushed ? Color (0, 100) : Color (0, 32),
      Color (0, 0, 0, 180));

    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x () + 1.0f, mPos.y () + 1.0f, mSize.y () - 2.0f,
      mSize.y () - 2.0f, 3);
    nvgFillPaint (ctx, bg);
    nvgFill (ctx);

    if (mChecked) {
      nvgFontSize (ctx, mSize.y () * icon_scale ());
      nvgFontFace (ctx, "icons");
      nvgFillColor (ctx, mEnabled ? mTheme->mIconColor
        : mTheme->mDisabledTextColor);
      nvgTextAlign (ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
      nvgText (ctx, mPos.x () + mSize.y () * 0.5f + 1,
        mPos.y () + mSize.y () * 0.5f, utf8 (mTheme->mCheckBoxIcon).data (),
        nullptr);
    }
  }

  // Saves this CheckBox to the specified Serializer.
  virtual void save (Serializer &s) const override {
    Widget::save (s);
    s.set ("caption", mCaption);
    s.set ("pushed", mPushed);
    s.set ("checked", mChecked);
  }

  // Loads the state of the specified Serializer to this CheckBox.
  virtual bool load (Serializer &s) override {
    if (!Widget::load (s)) return false;
    if (!s.get ("caption", mCaption)) return false;
    if (!s.get ("pushed", mPushed)) return false;
    if (!s.get ("checked", mChecked)) return false;
    return true;
  }

protected:
  // The caption text of this CheckBox.
  std::string mCaption;

  /*
   * Internal tracking variable to distinguish between mouse click and release.
   * @ref _::CheckBox::mCallback is only called upon release.  See
   * @ref _::CheckBox::mouseButtonEvent for specific conditions.
   */
  bool mPushed;

  // Whether or not this CheckBox is currently checked or unchecked.
  bool mChecked;

  // The function to execute when @ref _::CheckBox::mChecked is changed.
  std::function<void (bool)> mCallback;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
