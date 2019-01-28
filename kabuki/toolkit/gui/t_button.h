/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/*
 * @file /kabuki/button.h
 *
 * @brief Defines the [Normal/Toggle/Radio/Popup] @ref _::Button widget.
 */

#pragma once
#include <pch.h>

#include "t_opengl.h"
#include "t_serializer.h"
#include "t_theme.h"
#include "t_widget.h"

namespace _ {
/* [Normal/Toggle/Radio/Popup] Button widget. */
class SDK Button : public Widget {
 public:
  // Flags to specify the button behavior (can be combined with binary OR)
  enum Flags {
    NormalButton = (1 << 0),  //< A normal Button.
    RadioButton = (1 << 1),   //< A radio Button.
    ToggleButton = (1 << 2),  //< A toggle Button.
    PopupButton = (1 << 3)    //< A popup Button.
  };

  // The available icon positions.
  enum class IconPosition {
    Left,           //< Button icon on the far left.
    LeftCentered,   //< Button icon on the left, centered (depends on caption
                    // text length).
    RightCentered,  //< Button icon on the right, centered (depends on caption
                    // text length).
    Right           //< Button icon on the far right.
  };

  /* Creates a button attached to the specified parent.
  @param parent The @ref _::Widget this Button will be attached to.
  @param caption The name of the button (default `"Untitled"`).
  @param icon The icon to display with this Button.
  @see _::Button::mIcon. */
  Button(Widget *parent, const std::string &caption = "Untitled", int icon = 0)
      : Widget(parent),
        mCaption(caption),
        mIcon(icon),
        mIconPosition(IconPosition::LeftCentered),
        mPushed(false),
        mFlags(NormalButton),
        mBackgroundColor(Color(0, 0)),
        mTextColor(Color(0, 0)) {}

  // Returns the caption of this Button.
  const std::string &caption() const { return mCaption; }

  // Sets the caption of this Button.
  void setCaption(const std::string &caption) { mCaption = caption; }

  // Returns the background color of this Button.
  const Color &backgroundColor() const { return mBackgroundColor; }

  // Sets the background color of this Button.
  void setBackgroundColor(const Color &backgroundColor) {
    mBackgroundColor = backgroundColor;
  }

  // Returns the text color of the caption of this Button.
  const Color &textColor() const { return mTextColor; }

  // Sets the text color of the caption of this Button.
  void setTextColor(const Color &textColor) { mTextColor = textColor; }

  // Returns the icon of this Button.  See @ref _::Button::mIcon.
  int icon() const { return mIcon; }

  // Sets the icon of this Button.  See @ref _::Button::mIcon.
  void setIcon(int icon) { mIcon = icon; }

  // The current flags of this Button (see @ref _::Button::Flags for options).
  int flags() const { return mFlags; }

  // Sets the flags of this Button (see @ref _::Button::Flags for options).
  void setFlags(int buttonFlags) { mFlags = buttonFlags; }

  // The position of the icon for this Button.
  IconPosition iconPosition() const { return mIconPosition; }

  // Sets the position of the icon for this Button.
  void setIconPosition(IconPosition iconPosition) {
    mIconPosition = iconPosition;
  }

  // Whether or not this Button is currently pushed.
  bool pushed() const { return mPushed; }

  // Sets whether or not this Button is currently pushed.
  void setPushed(bool pushed) { mPushed = pushed; }

  // The current callback to execute (for any type of button).
  std::function<void()> callback() const { return mCallback; }

  // Set the push callback (for any type of button).
  void setCallback(const std::function<void()> &callback) {
    mCallback = callback;
  }

  // The current callback to execute (for toggle buttons).
  std::function<void(bool)> changeCallback() const { return mChangeCallback; }

  // Set the change callback (for toggle buttons).
  void setChangeCallback(const std::function<void(bool)> &callback) {
    mChangeCallback = callback;
  }

  // Set the button group (for radio buttons).
  void setButtonGroup(const std::vector<Button *> &buttonGroup) {
    mButtonGroup = buttonGroup;
  }

  // The current button group (for radio buttons).
  const std::vector<Button *> &buttonGroup() const { return mButtonGroup; }

  // The preferred size of this Button.
  virtual Vector2i preferredSize(NVGcontext *ctx) const override {
    int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    float tw = nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);
    float iw = 0.0f, ih = fontSize;

    if (mIcon) {
      if (nvgIsFontIcon(mIcon)) {
        ih *= icon_scale();
        nvgFontFace(ctx, "icons");
        nvgFontSize(ctx, ih);
        iw = nvgTextBounds(ctx, 0, 0, utf8(mIcon).data(), nullptr, nullptr) +
             mSize.y() * 0.15f;
      } else {
        int w, h;
        ih *= 0.9f;
        nvgImageSize(ctx, mIcon, &w, &h);
        iw = w * ih / h;
      }
    }
    return Vector2i((int)(tw + iw) + 20, fontSize + 10);
  }

  // The callback that is called when any type of mouse button event is issued
  // to this Button.
  virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down,
                                int modifiers) override {
    Widget::mouseButtonEvent(p, button, down, modifiers);
    /* Temporarily increase the reference count of the button in case the
       button causes the parent window to be destructed */
    ref<Button> self = this;

    if (button == GLFW_MOUSE_BUTTON_1 && mEnabled) {
      bool pushedBackup = mPushed;
      if (down) {
        if (mFlags & RadioButton) {
          if (mButtonGroup.empty()) {
            for (auto widget : parent()->children()) {
              Button *b = dynamic_cast<Button *>(widget);
              if (b != this && b && (b->flags() & RadioButton) && b->mPushed) {
                b->mPushed = false;
                if (b->mChangeCallback) b->mChangeCallback(false);
              }
            }
          } else {
            for (auto b : mButtonGroup) {
              if (b != this && (b->flags() & RadioButton) && b->mPushed) {
                b->mPushed = false;
                if (b->mChangeCallback) b->mChangeCallback(false);
              }
            }
          }
        }
        if (mFlags & PopupButton) {
          for (auto widget : parent()->children()) {
            Button *b = dynamic_cast<Button *>(widget);
            if (b != this && b && (b->flags() & PopupButton) && b->mPushed) {
              b->mPushed = false;
              if (b->mChangeCallback) b->mChangeCallback(false);
            }
          }
        }
        if (mFlags & ToggleButton)
          mPushed = !mPushed;
        else
          mPushed = true;
      } else if (mPushed) {
        if (contains(p) && mCallback) mCallback();
        if (mFlags & NormalButton) mPushed = false;
      }
      if (pushedBackup != mPushed && mChangeCallback) mChangeCallback(mPushed);

      return true;
    }
    return false;
  }

  // Responsible for drawing the Button.
  virtual void draw(NVGcontext *ctx) override {
    Widget::draw(ctx);

    NVGcolor gradTop = mTheme->mButtonGradientTopUnfocused;
    NVGcolor gradBot = mTheme->mButtonGradientBotUnfocused;

    if (mPushed) {
      gradTop = mTheme->mButtonGradientTopPushed;
      gradBot = mTheme->mButtonGradientBotPushed;
    } else if (mMouseFocus && mEnabled) {
      gradTop = mTheme->mButtonGradientTopFocused;
      gradBot = mTheme->mButtonGradientBotFocused;
    }

    nvgBeginPath(ctx);

    nvgRoundedRect(ctx, mPos.x() + 1, mPos.y() + 1.0f, mSize.x() - 2,
                   mSize.y() - 2, mTheme->mButtonCornerRadius - 1);

    if (mBackgroundColor.w() != 0) {
      nvgFillColor(ctx, Color(mBackgroundColor.head<3>(), 1.f));
      nvgFill(ctx);
      if (mPushed) {
        gradTop.a = gradBot.a = 0.8f;
      } else {
        double v = 1 - mBackgroundColor.w();
        gradTop.a = gradBot.a = mEnabled ? v : v * .5f + .5f;
      }
    }

    NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(),
                                    mPos.y() + mSize.y(), gradTop, gradBot);

    nvgFillPaint(ctx, bg);
    nvgFill(ctx);

    nvgBeginPath(ctx);
    nvgStrokeWidth(ctx, 1.0f);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + (mPushed ? 0.5f : 1.5f),
                   mSize.x() - 1, mSize.y() - 1 - (mPushed ? 0.0f : 1.0f),
                   mTheme->mButtonCornerRadius);
    nvgStrokeColor(ctx, mTheme->mBorderLight);
    nvgStroke(ctx);

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
                   mSize.y() - 2, mTheme->mButtonCornerRadius);
    nvgStrokeColor(ctx, mTheme->mBorderDark);
    nvgStroke(ctx);

    int fontSize = mFontSize == -1 ? mTheme->mButtonFontSize : mFontSize;
    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    float tw = nvgTextBounds(ctx, 0, 0, mCaption.c_str(), nullptr, nullptr);

    Vector2f center = mPos.cast<float>() + mSize.cast<float>() * 0.5f;
    Vector2f textPos(center.x() - tw * 0.5f, center.y() - 1);
    NVGcolor textColor = mTextColor.w() == 0 ? mTheme->mTextColor : mTextColor;
    if (!mEnabled) textColor = mTheme->mDisabledTextColor;

    if (mIcon) {
      auto icon = utf8(mIcon);

      float iw, ih = fontSize;
      if (nvgIsFontIcon(mIcon)) {
        ih *= icon_scale();
        nvgFontSize(ctx, ih);
        nvgFontFace(ctx, "icons");
        iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
      } else {
        int w, h;
        ih *= 0.9f;
        nvgImageSize(ctx, mIcon, &w, &h);
        iw = w * ih / h;
      }
      if (mCaption != "") iw += mSize.y() * 0.15f;
      nvgFillColor(ctx, textColor);
      nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
      Vector2f iconPos = center;
      iconPos.y() -= 1;

      if (mIconPosition == IconPosition::LeftCentered) {
        iconPos.x() -= (tw + iw) * 0.5f;
        textPos.x() += iw * 0.5f;
      } else if (mIconPosition == IconPosition::RightCentered) {
        textPos.x() -= iw * 0.5f;
        iconPos.x() += tw * 0.5f;
      } else if (mIconPosition == IconPosition::Left) {
        iconPos.x() = mPos.x() + 8;
      } else if (mIconPosition == IconPosition::Right) {
        iconPos.x() = mPos.x() + mSize.x() - iw - 8;
      }

      if (nvgIsFontIcon(mIcon)) {
        nvgText(ctx, iconPos.x(), iconPos.y() + 1, icon.data(), nullptr);
      } else {
        NVGpaint imgPaint =
            nvgImagePattern(ctx, iconPos.x(), iconPos.y() - ih / 2, iw, ih, 0,
                            mIcon, mEnabled ? 0.5f : 0.25f);

        nvgFillPaint(ctx, imgPaint);
        nvgFill(ctx);
      }
    }

    nvgFontSize(ctx, fontSize);
    nvgFontFace(ctx, "sans-bold");
    nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(ctx, mTheme->mTextColorShadow);
    nvgText(ctx, textPos.x(), textPos.y(), mCaption.c_str(), nullptr);
    nvgFillColor(ctx, textColor);
    nvgText(ctx, textPos.x(), textPos.y() + 1, mCaption.c_str(), nullptr);
  }

  // Saves the state of this Button provided the given Serializer.
  virtual void save(Serializer &s) const override {
    Widget::save(s);
    s.set("caption", mCaption);
    s.set("icon", mIcon);
    s.set("iconPosition", (int)mIconPosition);
    s.set("pushed", mPushed);
    s.set("flags", mFlags);
    s.set("backgroundColor", mBackgroundColor);
    s.set("textColor", mTextColor);
  }

  // Sets the state of this Button provided the given Serializer.
  virtual bool load(Serializer &s) override {
    if (!Widget::load(s)) return false;
    if (!s.get("caption", mCaption)) return false;
    if (!s.get("icon", mIcon)) return false;
    if (!s.get("iconPosition", mIconPosition)) return false;
    if (!s.get("pushed", mPushed)) return false;
    if (!s.get("flags", mFlags)) return false;
    if (!s.get("backgroundColor", mBackgroundColor)) return false;
    if (!s.get("textColor", mTextColor)) return false;
    return true;
  }

 protected:
  // The caption of this Button.
  std::string mCaption;

  /*
   * @brief The icon of this Button (`0` means no icon).
   *
   * \rst
   * The icon to display with this Button.  If not `0`, may either be a
   * picture icon, or one of the icons enumerated in
   * :ref:`file_nanogui_entypo.h`.  The kind of icon (image or Entypo)
   * is determined by the functions :func:`_::nvgIsImageIcon` and its
   * reciprocal counterpart :func:`_::nvgIsFontIcon`.
   * \endrst
   */
  int mIcon;

  // The position to draw the icon at.
  IconPosition mIconPosition;

  // Whether or not this Button is currently pushed.
  bool mPushed;

  // The current flags of this button (see @ref _::Button::Flags for options).
  int mFlags;

  // The background color of this Button.
  Color mBackgroundColor;

  // The color of the caption text of this Button.
  Color mTextColor;

  // The callback issued for all types of buttons.
  std::function<void()> mCallback;

  // The callback issued for toggle buttons.
  std::function<void(bool)> mChangeCallback;

  // The button group for radio buttons.
  std::vector<Button *> mButtonGroup;

 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}  // namespace _
