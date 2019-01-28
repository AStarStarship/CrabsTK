/*
    /kabuki/toolkit/gui/popupbutton.h -- Button which launches a popup widget

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


#pragma once
#include <pch.h>


#include "t_button.h"
#include "t_popup.h"
#include "c_entypo.h"
#include "t_theme.h"
#include "t_opengl.h"
#include "t_serializer.h"

namespace _ {

/* Button which launches a popup widget.
This class overrides @ref _::Widget::mIconExtraScale to be `0.8f`,
which affects all subclasses of this Widget.  Subclasses must explicitly
set a different value if needed (e.g., in their constructor). */
class SDK PopupButton : public Button {
public:
  PopupButton (Widget *parent, const std::string &caption = "Untitled",
    int buttonIcon = 0)
    : Button (parent, caption, buttonIcon) {

    mChevronIcon = mTheme->mPopupChevronRightIcon;

    setFlags (Flags::ToggleButton | Flags::PopupButton);

    Window *parentWindow = window ();
    mPopup = new Popup (parentWindow->parent (), window ());
    mPopup->setSize (Vector2i (320, 250));
    mPopup->setVisible (false);

    mIconExtraScale = 0.8f;// widget override
  }

  virtual ~PopupButton () {
    mPopup->setVisible (false);
  }

  void setChevronIcon (int icon) { mChevronIcon = icon; }
  int chevronIcon () const { return mChevronIcon; }

  void setSide (Popup::Side popupSide) {
    if (mPopup->side () == Popup::Right &&
      mChevronIcon == mTheme->mPopupChevronRightIcon)
      setChevronIcon (mTheme->mPopupChevronLeftIcon);
    else if (mPopup->side () == Popup::Left &&
      mChevronIcon == mTheme->mPopupChevronLeftIcon)
      setChevronIcon (mTheme->mPopupChevronRightIcon);
    mPopup->setSide (side);
  }

  Popup::Side side () const { return mPopup->side (); }

  Popup *popup () { return mPopup; }
  const Popup *popup () const { return mPopup; }

  virtual void draw (NVGcontext* ctx) override {
    if (!mEnabled && mPushed)
      mPushed = false;

    mPopup->setVisible (mPushed);
    Button::draw (ctx);

    if (mChevronIcon) {
      auto icon = utf8 (mChevronIcon);
      NVGcolor textColor =
        mTextColor.w () == 0 ? mTheme->mTextColor : mTextColor;

      nvgFontSize (ctx, (mFontSize < 0 ? mTheme->mButtonFontSize : mFontSize) * icon_scale ());
      nvgFontFace (ctx, "icons");
      nvgFillColor (ctx, mEnabled ? textColor : mTheme->mDisabledTextColor);
      nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

      float iw = nvgTextBounds (ctx, 0, 0, icon.data (), nullptr, nullptr);
      Vector2f iconPos (0, mPos.y () + mSize.y () * 0.5f - 1);

      if (mPopup->side () == Popup::Right)
        iconPos[0] = mPos.x () + mSize.x () - iw - 8;
      else
        iconPos[0] = mPos.x () + 8;

      nvgText (ctx, iconPos.x (), iconPos.y (), icon.data (), nullptr);
    }
  }

  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    return Button::preferredSize (ctx) + Vector2i (15, 0);
  }

  virtual void performLayout (NVGcontext *ctx) override {
    Widget::performLayout (ctx);

    const Window *parentWindow = window ();

    int posY = absolutePosition ().y () - parentWindow->position ().y () + mSize.y () / 2;
    if (mPopup->side () == Popup::Right)
      mPopup->setAnchorPos (Vector2i (parentWindow->width () + 15, posY));
    else
      mPopup->setAnchorPos (Vector2i (0 - 15, posY));
  }

  virtual void save (Serializer &s) const override {
    Button::save (s);
    s.set ("chevronIcon", mChevronIcon);
  }

  virtual bool load (Serializer &s) override {
    if (!Button::load (s))
      return false;
    if (!s.get ("chevronIcon", mChevronIcon))
      return false;
    return true;
  }

protected:
  Popup *mPopup;
  int mChevronIcon;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
