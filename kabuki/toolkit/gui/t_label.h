/*
    /kabuki/toolkit/gui/label.h -- Text label with an arbitrary font, color, and size

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
 * \class Label label.h /kabuki/toolkit/gui/label.h
 *
 * @brief Text label widget.
 *
 * The font and color can be customized. When @ref Widget::setFixedWidth()
 * is used, the text is wrapped when it surpasses the specified width.
 */
class SDK Label : public Widget {
public:
  Label (Widget *parent, const std::string &caption,
    const std::string &font = "sans", int fontSize = -1)
    : Widget (parent), mCaption (caption), mFont (font) {
    if (mTheme) {
      mFontSize = mTheme->mStandardFontSize;
      mColor = mTheme->mTextColor;
    }
    if (fontSize >= 0) mFontSize = fontSize;
  }

  // Get the label's text caption
  const std::string &caption () const { return mCaption; }

  // Set the label's text caption
  void setCaption (const std::string &caption) { mCaption = caption; }

  // Set the currently active font (2 are available by default: 'sans' and 'sans-bold')
  void setFont (const std::string &font) { mFont = font; }
  // Get the currently active font
  const std::string &font () const { return mFont; }

  // Get the label color
  Color color () const { return mColor; }
  // Set the label color
  void setColor (const Color& color) { mColor = color; }

  // Set the @ref Theme used to draw this widget
  virtual void setTheme (Theme *theme) override {
    Widget::setTheme (theme);
    if (mTheme) {
      mFontSize = mTheme->mStandardFontSize;
      mColor = mTheme->mTextColor;
    }
  }

  // Compute the size needed to fully display the label
  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    if (mCaption == "")
      return Vector2i::Zero ();
    nvgFontFace (ctx, mFont.c_str ());
    nvgFontSize (ctx, fontSize ());
    if (mFixedSize.x () > 0) {
      float bounds[4];
      nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
      nvgTextBoxBounds (ctx, mPos.x (), mPos.y (), mFixedSize.x (), mCaption.c_str (), nullptr, bounds);
      return Vector2i (mFixedSize.x (), bounds[3] - bounds[1]);
    } else {
      nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
      return Vector2i (
        nvgTextBounds (ctx, 0, 0, mCaption.c_str (), nullptr, nullptr) + 2,
        fontSize ()
      );
    }
  }

  // Draw the label
  virtual void draw (NVGcontext *ctx) override {
    Widget::draw (ctx);
    nvgFontFace (ctx, mFont.c_str ());
    nvgFontSize (ctx, fontSize ());
    nvgFillColor (ctx, mColor);
    if (mFixedSize.x () > 0) {
      nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
      nvgTextBox (ctx, mPos.x (), mPos.y (), mFixedSize.x (), mCaption.c_str (), nullptr);
    } else {
      nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
      nvgText (ctx, mPos.x (), mPos.y () + mSize.y () * 0.5f, mCaption.c_str (), nullptr);
    }
  }

  virtual void save (Serializer &s) const override {
    Widget::save (s);
    s.set ("caption", mCaption);
    s.set ("font", mFont);
    s.set ("color", mColor);
  }

  virtual bool load (Serializer &s) override {
    if (!Widget::load (s)) return false;
    if (!s.get ("caption", mCaption)) return false;
    if (!s.get ("font", mFont)) return false;
    if (!s.get ("color", mColor)) return false;
    return true;
  }

protected:
  std::string mCaption;
  std::string mFont;
  Color mColor;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
