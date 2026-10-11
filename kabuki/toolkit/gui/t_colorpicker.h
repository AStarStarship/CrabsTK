/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/*
 * @file /kabuki/toolkit/gui/colorpicker.h
 *
 * @brief Push button with a popup to tweak a color value.  This widget was
 *        contributed by Christian Schueller.
 */

#pragma once
#include <pch.h>


#include "t_popupbutton.h"

namespace _ {

/* Push button with a popup to tweak a color value.  This widget was contributed
by Christian Schueller. */
class SDK ColorPicker : public PopupButton {
public:
  /* Attaches a ColorPicker to the specified parent.
  @param parent The Widget to add this ColorPicker to.
  @param color The color initially selected by this ColorPicker (default: Red).
  */
  ColorPicker (Widget *parent, const Color& color = Color (1.0f, 0.0f, 0.0f, 1.0f))
    : PopupButton (parent, "") {
    setBackgroundColor (color);
    Popup *popup = this->popup ();
    popup->setLayout (new GroupLayout ());

    // initialize callback to do nothing; this is for users to hook into
    // receiving a new color value
    mCallback = [](const Color &) {};
    mFinalCallback = [](const Color &) {};

    // set the color wheel to the specified color
    mColorWheel = new ColorWheel (popup, color);

    // set the pick button to the specified color
    mPickButton = new Button (popup, "Pick");
    mPickButton->setBackgroundColor (color);
    mPickButton->setTextColor (color.contrastingColor ());
    mPickButton->setFixedSize (Vector2i (100, 20));

    // set the reset button to the specified color
    mResetButton = new Button (popup, "Reset");
    mResetButton->setBackgroundColor (color);
    mResetButton->setTextColor (color.contrastingColor ());
    mResetButton->setFixedSize (Vector2i (100, 20));

    PopupButton::setChangeCallback ([&](bool) {
      if (this->mPickButton->pushed ()) {
        setColor (backgroundColor ());
        mFinalCallback (backgroundColor ());
      }
    });

    mColorWheel->setCallback ([&](const Color &value) {
      mPickButton->setBackgroundColor (value);
      mPickButton->setTextColor (value.contrastingColor ());
      mCallback (value);
    });

    mPickButton->setCallback ([this]() {
      if (mPushed) {
        Color value = mColorWheel->color ();
        setPushed (false);
        setColor (value);
        mFinalCallback (value);
      }
    });

    mResetButton->setCallback ([this]() {
      Color bg = this->mResetButton->backgroundColor ();
      Color fg = this->mResetButton->textColor ();

      mColorWheel->setColor (bg);
      mPickButton->setBackgroundColor (bg);
      mPickButton->setTextColor (fg);

      mCallback (bg);
      mFinalCallback (bg);
    });
  }

  // The callback executed when the ColorWheel changes.
  std::function<void (const Color &)> callback () const { return mCallback; }

  /*
   * Sets the callback is executed as the ColorWheel itself is changed.  Set
   * this callback if you need to receive updates for the ColorWheel changing
   * before the user clicks @ref _::ColorPicker::mPickButton or
   * @ref _::ColorPicker::mPickButton.
   */
  void setCallback (const std::function<void (const Color &)> &callback) {
    mCallback = callback;
    mCallback (backgroundColor ());
  }

  /*
   * The callback to execute when a new Color is selected on the ColorWheel
   * **and** the user clicks the @ref _::ColorPicker::mPickButton or
   * @ref _::ColorPicker::mResetButton.
   */
  std::function<void (const Color &)> finalCallback () const { return mFinalCallback; }

  /*
   * The callback to execute when a new Color is selected on the ColorWheel
   * **and** the user clicks the @ref _::ColorPicker::mPickButton or
   * @ref _::ColorPicker::mResetButton.
   */
  void setFinalCallback (const std::function<void (const Color &)> &callback) { mFinalCallback = callback; }

  // Get the current Color selected for this ColorPicker.
  Color color () const {
    return backgroundColor ();
  }

  // Set the current Color selected for this ColorPicker.
  void setColor (const Color& color) {
    /* Ignore setColor() calls when the user is currently editing */
    if (!mPushed) {
      Color fg = color.contrastingColor ();
      setBackgroundColor (color);
      setTextColor (fg);
      mColorWheel->setColor (color);

      mPickButton->setBackgroundColor (color);
      mPickButton->setTextColor (fg);

      mResetButton->setBackgroundColor (color);
      mResetButton->setTextColor (fg);
    }
  }

  // The current caption of the @ref _::ColorPicker::mPickButton.
  const std::string &pickButtonCaption () { return mPickButton->caption (); }

  // Sets the current caption of the @ref _::ColorPicker::mPickButton.
  void setPickButtonCaption (const std::string &caption) { mPickButton->setCaption (caption); }

  // The current caption of the @ref _::ColorPicker::mResetButton.
  const std::string &resetButtonCaption () { return mResetButton->caption (); }

  // Sets the current caption of the @ref _::ColorPicker::mResetButton.
  void setResetButtonCaption (const std::string &caption) { mResetButton->setCaption (caption); }

protected:
  // The "fast" callback executed when the ColorWheel has changed.
  std::function<void (const Color &)> mCallback;

  /*
   * The callback to execute when a new Color is selected on the ColorWheel
   * **and** the user clicks the @ref _::ColorPicker::mPickButton or
   * @ref _::ColorPicker::mResetButton.
   */
  std::function<void (const Color &)> mFinalCallback;

  // The ColorWheel for this ColorPicker (the actual widget allowing selection).
  ColorWheel *mColorWheel;

  /*
   * The Button used to signal that the current value on the ColorWheel is the
   * desired color to be chosen.  The default value for the caption of this
   * Button is `"Pick"`.  You can change it using
   * @ref _::ColorPicker::setPickButtonCaption if you need.
   *
   * The color of this Button will not affect @ref _::ColorPicker::color
   * until the user has actively selected by clicking this pick button.
   * Similarly, the @ref _::ColorPicker::mCallback function is only
   * called when a user selects a new Color using by clicking this Button.
   */
  Button *mPickButton;

  /*
   * Remains the Color of the active color selection, until the user picks a
   * new Color on the ColorWheel **and** selects the
   * @ref _::ColorPicker::mPickButton.  The default value for the
   * caption of this Button is `"Reset"`.  You can change it using
   * @ref _::ColorPicker::setResetButtonCaption if you need.
   */
  Button *mResetButton;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
