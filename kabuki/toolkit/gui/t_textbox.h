/*
    /kabuki/toolkit/gui/textbox.h -- Fancy text box with builtin regular
    expression-based validation

    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once
#include <pch.h>

#include "c_compat.h"
#include "t_widget.h"
#include "t_window.h"
#include "t_screen.h"
#include "t_opengl.h"
#include "t_theme.h"
#include "t_serializer.h"

#include <regex>
#include <iostream>
#include <sstream>

namespace _ {

/* Fancy text box with builtin regular expression-based validation.
This class overrides @ref _::Widget::mIconExtraScale to be `0.8f`, which
affects all subclasses of this Widget.  Subclasses must explicitly set a
different value if needed (e.g., in their constructor). */
class SDK TextBox : public Widget {
public:
  // How to align the text in the text box.
  enum class Alignment {
    Left,
    Center,
    Right
  };

  TextBox (Widget *parent, const std::string &value = "Untitled")
    : Widget (parent),
    mEditable (false),
    mSpinnable (false),
    mCommitted (true),
    mValue (value),
    mDefaultValue (""),
    mAlignment (Alignment::Center),
    mUnits (""),
    mFormat (""),
    mUnitsImage (-1),
    mValidFormat (true),
    mValueTemp (value),
    mCursorPos (-1),
    mSelectionPos (-1),
    mMousePos (Vector2i (-1, -1)),
    mMouseDownPos (Vector2i (-1, -1)),
    mMouseDragPos (Vector2i (-1, -1)),
    mMouseDownModifier (0),
    mTextOffset (0),
    mLastClick (0) {
    if (mTheme) mFontSize = mTheme->mTextBoxFontSize;
    mIconExtraScale = 0.8f;// widget override
  }

  bool editable () const { return mEditable; }

  void setEditable (bool editable) {
    mEditable = editable;
    setCursor (editable ? Cursor::IBeam : Cursor::Arrow);
  }

  bool spinnable () const { return mSpinnable; }
  void setSpinnable (bool spinnable) { mSpinnable = spinnable; }

  const std::string &value () const { return mValue; }
  void setValue (const std::string &value) { mValue = value; }

  const std::string &defaultValue () const { return mDefaultValue; }
  void setDefaultValue (const std::string &defaultValue) { mDefaultValue = defaultValue; }

  Alignment alignment () const { return mAlignment; }
  void setAlignment (Alignment align) { mAlignment = align; }

  const std::string &units () const { return mUnits; }
  void setUnits (const std::string &units) { mUnits = units; }

  int unitsImage () const { return mUnitsImage; }
  void setUnitsImage (int image) { mUnitsImage = image; }

  // Return the underlying regular expression specifying valid formats
  const std::string &format () const { return mFormat; }
  // Specify a regular expression specifying valid formats
  void setFormat (const std::string &format) { mFormat = format; }

  // Return the placeholder text to be displayed while the text box is empty.
  const std::string &placeholder () const { return mPlaceholder; }
  // Specify a placeholder text to be displayed while the text box is empty.
  void setPlaceholder (const std::string &placeholder) { mPlaceholder = placeholder; }

  // Set the @ref Theme used to draw this widget
  virtual void setTheme (Theme *theme) override {
    Widget::setTheme (theme);
    if (mTheme)
      mFontSize = mTheme->mTextBoxFontSize;
  }

  // The callback to execute when the value of this TextBox has changed.
  std::function<bool (const std::string& str)> callback () const { return mCallback; }

  // Sets the callback to execute when the value of this TextBox has changed.
  void setCallback (const std::function<bool (const std::string& str)> &callback) { mCallback = callback; }

  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down, int modifiers) override {

    if (button == GLFW_MOUSE_BUTTON_1 && down && !mFocused) {
      if (!mSpinnable || spinArea (p) == SpinArea::None) /* not on scrolling arrows */
        requestFocus ();
    }

    if (mEditable && focused ()) {
      if (down) {
        mMouseDownPos = p;
        mMouseDownModifier = modifiers;

        double time = glfwGetTime ();
        if (time - mLastClick < 0.25) {
          /* Double-click: select all text */
          mSelectionPos = 0;
          mCursorPos = (int)mValueTemp.size ();
          mMouseDownPos = Vector2i (-1, -1);
        }
        mLastClick = time;
      } else {
        mMouseDownPos = Vector2i (-1, -1);
        mMouseDragPos = Vector2i (-1, -1);
      }
      return true;
    } else if (mSpinnable && !focused ()) {
      if (down) {
        if (spinArea (p) == SpinArea::None) {
          mMouseDownPos = p;
          mMouseDownModifier = modifiers;

          double time = glfwGetTime ();
          if (time - mLastClick < 0.25) {
            /* Double-click: reset to default value */
            mValue = mDefaultValue;
            if (mCallback)
              mCallback (mValue);

            mMouseDownPos = Vector2i (-1, -1);
          }
          mLastClick = time;
        } else {
          mMouseDownPos = Vector2i (-1, -1);
          mMouseDragPos = Vector2i (-1, -1);
        }
      } else {
        mMouseDownPos = Vector2i (-1, -1);
        mMouseDragPos = Vector2i (-1, -1);
      }
      return true;
    }

    return false;
  }

  virtual bool mouseMotionEvent (const Vector2i &p, const Vector2i &rel,
    int button, int modifiers) override {
    mMousePos = p;

    if (!mEditable)
      setCursor (Cursor::Arrow);
    else if (mSpinnable && !focused () && spinArea (mMousePos) != SpinArea::None) /* scrolling arrows */
      setCursor (Cursor::Hand);
    else
      setCursor (Cursor::IBeam);

    if (mEditable && focused ()) {
      return true;
    }
    return false;
  }

  virtual bool mouseDragEvent (const Vector2i &p, const Vector2i &rel,
    int button, int modifiers) override {
    mMousePos = p;
    mMouseDragPos = p;

    if (mEditable && focused ()) {
      return true;
    }
    return false;
  }

  virtual bool focusEvent (bool focused) override {
    Widget::focusEvent (focused);

    std::string backup = mValue;

    if (mEditable) {
      if (focused) {
        mValueTemp = mValue;
        mCommitted = false;
        mCursorPos = 0;
      } else {
        if (mValidFormat) {
          if (mValueTemp == "")
            mValue = mDefaultValue;
          else
            mValue = mValueTemp;
        }

        if (mCallback && !mCallback (mValue))
          mValue = backup;

        mValidFormat = true;
        mCommitted = true;
        mCursorPos = -1;
        mSelectionPos = -1;
        mTextOffset = 0;
      }

      mValidFormat = (mValueTemp == "") || checkFormat (mValueTemp, mFormat);
    }

    return true;
  }

  virtual bool keyboardEvent (int key, int scancode, int action, int modifiers)
    override {
    if (mEditable && focused ()) {
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) {
          if (modifiers == GLFW_MOD_SHIFT) {
            if (mSelectionPos == -1)
              mSelectionPos = mCursorPos;
          } else {
            mSelectionPos = -1;
          }

          if (mCursorPos > 0)
            mCursorPos--;
        } else if (key == GLFW_KEY_RIGHT) {
          if (modifiers == GLFW_MOD_SHIFT) {
            if (mSelectionPos == -1)
              mSelectionPos = mCursorPos;
          } else {
            mSelectionPos = -1;
          }

          if (mCursorPos < (int)mValueTemp.length ())
            mCursorPos++;
        } else if (key == GLFW_KEY_HOME) {
          if (modifiers == GLFW_MOD_SHIFT) {
            if (mSelectionPos == -1)
              mSelectionPos = mCursorPos;
          } else {
            mSelectionPos = -1;
          }

          mCursorPos = 0;
        } else if (key == GLFW_KEY_END) {
          if (modifiers == GLFW_MOD_SHIFT) {
            if (mSelectionPos == -1)
              mSelectionPos = mCursorPos;
          } else {
            mSelectionPos = -1;
          }

          mCursorPos = (int)mValueTemp.size ();
        } else if (key == GLFW_KEY_BACKSPACE) {
          if (!deleteSelection ()) {
            if (mCursorPos > 0) {
              mValueTemp.erase (mValueTemp.begin () + mCursorPos - 1);
              mCursorPos--;
            }
          }
        } else if (key == GLFW_KEY_DELETE) {
          if (!deleteSelection ()) {
            if (mCursorPos < (int)mValueTemp.length ())
              mValueTemp.erase (mValueTemp.begin () + mCursorPos);
          }
        } else if (key == GLFW_KEY_ENTER) {
          if (!mCommitted)
            focusEvent (false);
        } else if (key == GLFW_KEY_A && modifiers == SYSTEM_COMMAND_MOD) {
          mCursorPos = (int)mValueTemp.length ();
          mSelectionPos = 0;
        } else if (key == GLFW_KEY_X && modifiers == SYSTEM_COMMAND_MOD) {
          copySelection ();
          deleteSelection ();
        } else if (key == GLFW_KEY_C && modifiers == SYSTEM_COMMAND_MOD) {
          copySelection ();
        } else if (key == GLFW_KEY_V && modifiers == SYSTEM_COMMAND_MOD) {
          deleteSelection ();
          pasteFromClipboard ();
        }

        mValidFormat =
          (mValueTemp == "") || checkFormat (mValueTemp, mFormat);
      }

      return true;
    }

    return false;
  }

  virtual bool keyboardCharacterEvent (unsigned int codepoint) override {
    if (mEditable && focused ()) {
      std::ostringstream convert;
      convert << (char)codepoint;

      deleteSelection ();
      mValueTemp.insert (mCursorPos, convert.str ());
      mCursorPos++;

      mValidFormat = (mValueTemp == "") || checkFormat (mValueTemp, mFormat);

      return true;
    }

    return false;
  }

  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    Vector2i size (0, fontSize () * 1.4f);

    float uw = 0;
    if (mUnitsImage > 0) {
      int w, h;
      nvgImageSize (ctx, mUnitsImage, &w, &h);
      float uh = size (1) * 0.4f;
      uw = w * uh / h;
    } else if (!mUnits.empty ()) {
      uw = nvgTextBounds (ctx, 0, 0, mUnits.c_str (), nullptr, nullptr);
    }
    float sw = 0;
    if (mSpinnable) {
      sw = 14.f;
    }

    float ts = nvgTextBounds (ctx, 0, 0, mValue.c_str (), nullptr, nullptr);
    size (0) = size (1) + ts + uw + sw;
    return size;
  }

  virtual void draw (NVGcontext* ctx) override {
    Widget::draw (ctx);

    NVGpaint bg = nvgBoxGradient (ctx,
      mPos.x () + 1, mPos.y () + 1 + 1.0f, mSize.x () - 2, mSize.y () - 2,
      3, 4, Color (255, 32), Color (32, 32));
    NVGpaint fg1 = nvgBoxGradient (ctx,
      mPos.x () + 1, mPos.y () + 1 + 1.0f, mSize.x () - 2, mSize.y () - 2,
      3, 4, Color (150, 32), Color (32, 32));
    NVGpaint fg2 = nvgBoxGradient (ctx,
      mPos.x () + 1, mPos.y () + 1 + 1.0f, mSize.x () - 2, mSize.y () - 2,
      3, 4, nvgRGBA (255, 0, 0, 100), nvgRGBA (255, 0, 0, 50));

    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x () + 1, mPos.y () + 1 + 1.0f, mSize.x () - 2,
      mSize.y () - 2, 3);

    if (mEditable && focused ())
      mValidFormat ? nvgFillPaint (ctx, fg1) : nvgFillPaint (ctx, fg2);
    else if (mSpinnable && mMouseDownPos.x () != -1)
      nvgFillPaint (ctx, fg1);
    else
      nvgFillPaint (ctx, bg);

    nvgFill (ctx);

    nvgBeginPath (ctx);
    nvgRoundedRect (ctx, mPos.x () + 0.5f, mPos.y () + 0.5f, mSize.x () - 1,
      mSize.y () - 1, 2.5f);
    nvgStrokeColor (ctx, Color (0, 48));
    nvgStroke (ctx);

    nvgFontSize (ctx, fontSize ());
    nvgFontFace (ctx, "sans");
    Vector2i drawPos (mPos.x (), mPos.y () + mSize.y () * 0.5f + 1);

    float xSpacing = mSize.y () * 0.3f;

    float unitWidth = 0;

    if (mUnitsImage > 0) {
      int w, h;
      nvgImageSize (ctx, mUnitsImage, &w, &h);
      float unitHeight = mSize.y () * 0.4f;
      unitWidth = w * unitHeight / h;
      NVGpaint imgPaint = nvgImagePattern (
        ctx, mPos.x () + mSize.x () - xSpacing - unitWidth,
        drawPos.y () - unitHeight * 0.5f, unitWidth, unitHeight, 0,
        mUnitsImage, mEnabled ? 0.7f : 0.35f);
      nvgBeginPath (ctx);
      nvgRect (ctx, mPos.x () + mSize.x () - xSpacing - unitWidth,
        drawPos.y () - unitHeight * 0.5f, unitWidth, unitHeight);
      nvgFillPaint (ctx, imgPaint);
      nvgFill (ctx);
      unitWidth += 2;
    } else if (!mUnits.empty ()) {
      unitWidth = nvgTextBounds (ctx, 0, 0, mUnits.c_str (), nullptr, nullptr);
      nvgFillColor (ctx, Color (255, mEnabled ? 64 : 32));
      nvgTextAlign (ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
      nvgText (ctx, mPos.x () + mSize.x () - xSpacing, drawPos.y (),
        mUnits.c_str (), nullptr);
      unitWidth += 2;
    }

    float spinArrowsWidth = 0.f;

    if (mSpinnable && !focused ()) {
      spinArrowsWidth = 14.f;

      nvgFontFace (ctx, "icons");
      nvgFontSize (ctx, ((mFontSize < 0) ? mTheme->mButtonFontSize : mFontSize) * icon_scale ());

      bool spinning = mMouseDownPos.x () != -1;

      /* up button */
      {
        bool hover = mMouseFocus && spinArea (mMousePos) == SpinArea::Top;
        nvgFillColor (ctx, (mEnabled && (hover || spinning)) ? mTheme->mTextColor : mTheme->mDisabledTextColor);
        auto icon = utf8 (mTheme->mTextBoxUpIcon);
        nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        Vector2f iconPos (mPos.x () + 4.f,
          mPos.y () + mSize.y () / 2.f - xSpacing / 2.f);
        nvgText (ctx, iconPos.x (), iconPos.y (), icon.data (), nullptr);
      }

      /* down button */
      {
        bool hover = mMouseFocus && spinArea (mMousePos) == SpinArea::Bottom;
        nvgFillColor (ctx, (mEnabled && (hover || spinning)) ? mTheme->mTextColor : mTheme->mDisabledTextColor);
        auto icon = utf8 (mTheme->mTextBoxDownIcon);
        nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        Vector2f iconPos (mPos.x () + 4.f,
          mPos.y () + mSize.y () / 2.f + xSpacing / 2.f + 1.5f);
        nvgText (ctx, iconPos.x (), iconPos.y (), icon.data (), nullptr);
      }

      nvgFontSize (ctx, fontSize ());
      nvgFontFace (ctx, "sans");
    }

    switch (mAlignment) {
    case Alignment::Left:
      nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
      drawPos.x () += xSpacing + spinArrowsWidth;
      break;
    case Alignment::Right:
      nvgTextAlign (ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
      drawPos.x () += mSize.x () - unitWidth - xSpacing;
      break;
    case Alignment::Center:
      nvgTextAlign (ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
      drawPos.x () += mSize.x () * 0.5f;
      break;
    }

    nvgFontSize (ctx, fontSize ());
    nvgFillColor (ctx, mEnabled && (!mCommitted || !mValue.empty ()) ?
      mTheme->mTextColor :
      mTheme->mDisabledTextColor);

    // clip visible text area
    float clipX = mPos.x () + xSpacing + spinArrowsWidth - 1.0f;
    float clipY = mPos.y () + 1.0f;
    float clipWidth = mSize.x () - unitWidth - spinArrowsWidth - 2 * xSpacing + 2.0f;
    float clipHeight = mSize.y () - 3.0f;

    nvgSave (ctx);
    nvgIntersectScissor (ctx, clipX, clipY, clipWidth, clipHeight);

    Vector2i oldDrawPos (drawPos);
    drawPos.x () += mTextOffset;

    if (mCommitted) {
      nvgText (ctx, drawPos.x (), drawPos.y (),
        mValue.empty () ? mPlaceholder.c_str () : mValue.c_str (), nullptr);
    } else {
      const int maxGlyphs = 1024;
      NVGglyphPosition glyphs[maxGlyphs];
      float textBound[4];
      nvgTextBounds (ctx, drawPos.x (), drawPos.y (), mValueTemp.c_str (),
        nullptr, textBound);
      float lineh = textBound[3] - textBound[1];

      // find cursor positions
      int nglyphs =
        nvgTextGlyphPositions (ctx, drawPos.x (), drawPos.y (),
          mValueTemp.c_str (), nullptr, glyphs, maxGlyphs);
      updateCursor (ctx, textBound[2], glyphs, nglyphs);

      // compute text offset
      int prevCPos = mCursorPos > 0 ? mCursorPos - 1 : 0;
      int nextCPos = mCursorPos < nglyphs ? mCursorPos + 1 : nglyphs;
      float prevCX = cursorIndex2Position (prevCPos, textBound[2], glyphs, nglyphs);
      float nextCX = cursorIndex2Position (nextCPos, textBound[2], glyphs, nglyphs);

      if (nextCX > clipX + clipWidth)
        mTextOffset -= nextCX - (clipX + clipWidth) + 1;
      if (prevCX < clipX)
        mTextOffset += clipX - prevCX + 1;

      drawPos.x () = oldDrawPos.x () + mTextOffset;

      // draw text with offset
      nvgText (ctx, drawPos.x (), drawPos.y (), mValueTemp.c_str (), nullptr);
      nvgTextBounds (ctx, drawPos.x (), drawPos.y (), mValueTemp.c_str (),
        nullptr, textBound);

      // recompute cursor positions
      nglyphs = nvgTextGlyphPositions (ctx, drawPos.x (), drawPos.y (),
        mValueTemp.c_str (), nullptr, glyphs, maxGlyphs);

      if (mCursorPos > -1) {
        if (mSelectionPos > -1) {
          float caretx = cursorIndex2Position (mCursorPos, textBound[2],
            glyphs, nglyphs);
          float selx = cursorIndex2Position (mSelectionPos, textBound[2],
            glyphs, nglyphs);

          if (caretx > selx)
            std::swap (caretx, selx);

          // draw selection
          nvgBeginPath (ctx);
          nvgFillColor (ctx, nvgRGBA (255, 255, 255, 80));
          nvgRect (ctx, caretx, drawPos.y () - lineh * 0.5f, selx - caretx,
            lineh);
          nvgFill (ctx);
        }

        float caretx = cursorIndex2Position (mCursorPos, textBound[2], glyphs, nglyphs);

        // draw cursor
        nvgBeginPath (ctx);
        nvgMoveTo (ctx, caretx, drawPos.y () - lineh * 0.5f);
        nvgLineTo (ctx, caretx, drawPos.y () + lineh * 0.5f);
        nvgStrokeColor (ctx, nvgRGBA (255, 192, 0, 255));
        nvgStrokeWidth (ctx, 1.0f);
        nvgStroke (ctx);
      }
    }
    nvgRestore (ctx);
  }

  virtual void save (Serializer &s) const override {
    Widget::save (s);
    s.set ("editable", mEditable);
    s.set ("spinnable", mSpinnable);
    s.set ("committed", mCommitted);
    s.set ("value", mValue);
    s.set ("defaultValue", mDefaultValue);
    s.set ("alignment", (int)mAlignment);
    s.set ("units", mUnits);
    s.set ("format", mFormat);
    s.set ("unitsImage", mUnitsImage);
    s.set ("validFormat", mValidFormat);
    s.set ("valueTemp", mValueTemp);
    s.set ("cursorPos", mCursorPos);
    s.set ("selectionPos", mSelectionPos);
  }

  virtual bool load (Serializer &s) override {
    if (!Widget::load (s)) return false;
    if (!s.get ("editable", mEditable)) return false;
    if (!s.get ("spinnable", mSpinnable)) return false;
    if (!s.get ("committed", mCommitted)) return false;
    if (!s.get ("value", mValue)) return false;
    if (!s.get ("defaultValue", mDefaultValue)) return false;
    if (!s.get ("alignment", mAlignment)) return false;
    if (!s.get ("units", mUnits)) return false;
    if (!s.get ("format", mFormat)) return false;
    if (!s.get ("unitsImage", mUnitsImage)) return false;
    if (!s.get ("validFormat", mValidFormat)) return false;
    if (!s.get ("valueTemp", mValueTemp)) return false;
    if (!s.get ("cursorPos", mCursorPos)) return false;
    if (!s.get ("selectionPos", mSelectionPos)) return false;
    mMousePos = mMouseDownPos = mMouseDragPos = Vector2i::Constant (-1);
    mMouseDownModifier = mTextOffset = 0;
    return true;
  }

protected:
  bool checkFormat (const std::string& input, const std::string& format) {
    if (format.empty ())
      return true;
    try {
      std::regex regex (format);
      return regex_match (input, regex);
    } catch (const std::regex_error &) {
#if __GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
      std::cerr << "Warning: cannot validate text field due to lacking regular expression support. please compile with GCC >= 4.9" << std::endl;
      return true;
#else
      throw;
#endif
    }
  }

  bool copySelection () {
    if (mSelectionPos > -1) {
      Screen *sc = dynamic_cast<Screen *>(this->window ()->parent ());
      if (!sc)
        return false;

      int begin = mCursorPos;
      int end = mSelectionPos;

      if (begin > end)
        std::swap (begin, end);

      glfwSetClipboardString (sc->glfwWindow (),
        mValueTemp.substr (begin, end).c_str ());
      return true;
    }

    return false;
  }

  void pasteFromClipboard () {
    Screen *sc = dynamic_cast<Screen *>(this->window ()->parent ());
    if (!sc)
      return;
    const char* cbstr = glfwGetClipboardString (sc->glfwWindow ());
    if (cbstr)
      mValueTemp.insert (mCursorPos, std::string (cbstr));
  }

  bool deleteSelection () {
    if (mSelectionPos > -1) {
      int begin = mCursorPos;
      int end = mSelectionPos;

      if (begin > end)
        std::swap (begin, end);

      if (begin == end - 1)
        mValueTemp.erase (mValueTemp.begin () + begin);
      else
        mValueTemp.erase (mValueTemp.begin () + begin,
          mValueTemp.begin () + end);

      mCursorPos = begin;
      mSelectionPos = -1;
      return true;
    }

    return false;
  }

  void updateCursor (NVGcontext *ctx, float lastx,
    const NVGglyphPosition *glyphs, int size) {
    // handle mouse cursor events
    if (mMouseDownPos.x () != -1) {
      if (mMouseDownModifier == GLFW_MOD_SHIFT) {
        if (mSelectionPos == -1)
          mSelectionPos = mCursorPos;
      } else
        mSelectionPos = -1;

      mCursorPos =
        position2CursorIndex (mMouseDownPos.x (), lastx, glyphs, size);

      mMouseDownPos = Vector2i (-1, -1);
    } else if (mMouseDragPos.x () != -1) {
      if (mSelectionPos == -1)
        mSelectionPos = mCursorPos;

      mCursorPos =
        position2CursorIndex (mMouseDragPos.x (), lastx, glyphs, size);
    } else {
      // set cursor to last character
      if (mCursorPos == -2)
        mCursorPos = size;
    }

    if (mCursorPos == mSelectionPos)
      mSelectionPos = -1;
  }

  float cursorIndex2Position (int index, float lastx,
    const NVGglyphPosition *glyphs, int size) {
    float pos = 0;
    if (index == size)
      pos = lastx; // last character
    else
      pos = glyphs[index].x;

    return pos;
  }

  int position2CursorIndex (float posx, float lastx,
    const NVGglyphPosition *glyphs, int size) {
    int mCursorId = 0;
    float caretx = glyphs[mCursorId].x;
    for (int j = 1; j < size; j++) {
      if (std::abs (caretx - posx) > std::abs (glyphs[j].x - posx)) {
        mCursorId = j;
        caretx = glyphs[mCursorId].x;
      }
    }
    if (std::abs (caretx - posx) > std::abs (lastx - posx))
      mCursorId = size;

    return mCursorId;
  }

  // The location (if any) for the spin area.
  enum class SpinArea { None, Top, Bottom };

  SpinArea spinArea (const Vector2i & pos) {
    if (0 <= pos.x () - mPos.x () && pos.x () - mPos.x () < 14.f) { /* on scrolling arrows */
      if (mSize.y () >= pos.y () - mPos.y () && pos.y () - mPos.y () <= mSize.y () / 2.f) { /* top part */
        return SpinArea::Top;
      } else if (0.f <= pos.y () - mPos.y () && pos.y () - mPos.y () > mSize.y () / 2.f) { /* bottom part */
        return SpinArea::Bottom;
      }
    }
    return SpinArea::None;
  }

protected:
  bool mEditable;
  bool mSpinnable;
  bool mCommitted;
  std::string mValue;
  std::string mDefaultValue;
  Alignment mAlignment;
  std::string mUnits;
  std::string mFormat;
  int mUnitsImage;
  std::function<bool (const std::string& str)> mCallback;
  bool mValidFormat;
  std::string mValueTemp;
  std::string mPlaceholder;
  int mCursorPos;
  int mSelectionPos;
  Vector2i mMousePos;
  Vector2i mMouseDownPos;
  Vector2i mMouseDragPos;
  int mMouseDownModifier;
  float mTextOffset;
  double mLastClick;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

/*
 * \class IntBox textbox.h /kabuki/toolkit/gui/textbox.h
 *
 * @brief A specialization of TextBox for representing integral values.
 *
 * Template parameters should be integral types, e.g. `int`, `long`,
 * `uint32_t`, etc.
 */
template <typename Scalar>
class IntBox : public TextBox {
public:
  IntBox (Widget *parent, Scalar value = (Scalar)0) : TextBox (parent) {
    setDefaultValue ("0");
    setFormat (std::is_signed<Scalar>::value ? "[-]?[0-9]*" : "[0-9]*");
    setValueIncrement (1);
    setMinMaxValues (std::numeric_limits<Scalar>::lowest (), std::numeric_limits<Scalar>::max ());
    setValue (value);
    setSpinnable (false);
  }

  Scalar value () const {
    std::istringstream iss (TextBox::value ());
    Scalar value = 0;
    iss >> value;
    return value;
  }

  void setValue (Scalar value) {
    Scalar clampedValue = std::min (std::max (value, mMinValue), mMaxValue);
    TextBox::setValue (std::to_string (clampedValue));
  }

  void setCallback (const std::function<void (Scalar)> &cb) {
    TextBox::setCallback (
      [cb, this](const std::string &str) {
      std::istringstream iss (str);
      Scalar value = 0;
      iss >> value;
      setValue (value);
      cb (value);
      return true;
    }
    );
  }

  void setValueIncrement (Scalar incr) {
    mValueIncrement = incr;
  }
  void setMinValue (Scalar minValue) {
    mMinValue = minValue;
  }
  void setMaxValue (Scalar maxValue) {
    mMaxValue = maxValue;
  }
  void setMinMaxValues (Scalar minValue, Scalar maxValue) {
    setMinValue (minValue);
    setMaxValue (maxValue);
  }

  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down, int modifiers) override {
    if ((mEditable || mSpinnable) && down)
      mMouseDownValue = value ();

    SpinArea area = spinArea (p);
    if (mSpinnable && area != SpinArea::None && down && !focused ()) {
      if (area == SpinArea::Top) {
        setValue (value () + mValueIncrement);
        if (mCallback)
          mCallback (mValue);
      } else if (area == SpinArea::Bottom) {
        setValue (value () - mValueIncrement);
        if (mCallback)
          mCallback (mValue);
      }
      return true;
    }

    return TextBox::mouseButtonEvent (p, button, down, modifiers);
  }
  virtual bool mouseDragEvent (const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    if (TextBox::mouseDragEvent (p, rel, button, modifiers)) {
      return true;
    }
    if (mSpinnable && !focused () && button == 2 /* 1 << GLFW_MOUSE_BUTTON_2 */ && mMouseDownPos.x () != -1) {
      int valueDelta = static_cast<int>((p.x () - mMouseDownPos.x ()) / float (10));
      setValue (mMouseDownValue + valueDelta * mValueIncrement);
      if (mCallback)
        mCallback (mValue);
      return true;
    }
    return false;
  }
  virtual bool scrollEvent (const Vector2i &p, const Vector2f &rel) override {
    if (Widget::scrollEvent (p, rel)) {
      return true;
    }
    if (mSpinnable && !focused ()) {
      int valueDelta = (rel.y () > 0) ? 1 : -1;
      setValue (value () + valueDelta * mValueIncrement);
      if (mCallback)
        mCallback (mValue);
      return true;
    }
    return false;
  }
private:
  Scalar mMouseDownValue;
  Scalar mValueIncrement;
  Scalar mMinValue, mMaxValue;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

/*
 * \class FloatBox textbox.h /kabuki/toolkit/gui/textbox.h
 *
 * @brief A specialization of TextBox representing floating point values.

 * Template parameters should be float types, e.g. `float`, `double`,
 * `float64_t`, etc.
 */
template <typename Scalar>
class FloatBox : public TextBox {
public:
  FloatBox (Widget *parent, Scalar value = (Scalar) 0.f) : TextBox (parent) {
    mNumberFormat = sizeof (Scalar) == sizeof (float) ? "%.4g" : "%.7g";
    setDefaultValue ("0");
    setFormat ("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
    setValueIncrement ((Scalar) 0.1);
    setMinMaxValues (std::numeric_limits<Scalar>::lowest (), std::numeric_limits<Scalar>::max ());
    setValue (value);
    setSpinnable (false);
  }

  std::string numberFormat () const { return mNumberFormat; }
  void numberFormat (const std::string &format) { mNumberFormat = format; }

  Scalar value () const {
    return (Scalar)std::stod (TextBox::value ());
  }

  void setValue (Scalar value) {
    Scalar clampedValue = std::min (std::max (value, mMinValue), mMaxValue);
    char buffer[50];
    NANOGUI_SNPRINTF (buffer, 50, mNumberFormat.c_str (), clampedValue);
    TextBox::setValue (buffer);
  }

  void setCallback (const std::function<void (Scalar)> &cb) {
    TextBox::setCallback ([cb, this](const std::string &str) {
      Scalar scalar = (Scalar)std::stod (str);
      setValue (scalar);
      cb (scalar);
      return true;
    });
  }

  void setValueIncrement (Scalar incr) {
    mValueIncrement = incr;
  }
  void setMinValue (Scalar minValue) {
    mMinValue = minValue;
  }
  void setMaxValue (Scalar maxValue) {
    mMaxValue = maxValue;
  }
  void setMinMaxValues (Scalar minValue, Scalar maxValue) {
    setMinValue (minValue);
    setMaxValue (maxValue);
  }

  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down, int modifiers) override {
    if ((mEditable || mSpinnable) && down)
      mMouseDownValue = value ();

    SpinArea area = spinArea (p);
    if (mSpinnable && area != SpinArea::None && down && !focused ()) {
      if (area == SpinArea::Top) {
        setValue (value () + mValueIncrement);
        if (mCallback)
          mCallback (mValue);
      } else if (area == SpinArea::Bottom) {
        setValue (value () - mValueIncrement);
        if (mCallback)
          mCallback (mValue);
      }
      return true;
    }

    return TextBox::mouseButtonEvent (p, button, down, modifiers);
  }
  virtual bool mouseDragEvent (const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    if (TextBox::mouseDragEvent (p, rel, button, modifiers)) {
      return true;
    }
    if (mSpinnable && !focused () && button == 2 /* 1 << GLFW_MOUSE_BUTTON_2 */ && mMouseDownPos.x () != -1) {
      int valueDelta = static_cast<int>((p.x () - mMouseDownPos.x ()) / float (10));
      setValue (mMouseDownValue + valueDelta * mValueIncrement);
      if (mCallback)
        mCallback (mValue);
      return true;
    }
    return false;
  }
  virtual bool scrollEvent (const Vector2i &p, const Vector2f &rel) override {
    if (Widget::scrollEvent (p, rel)) {
      return true;
    }
    if (mSpinnable && !focused ()) {
      int valueDelta = (rel.y () > 0) ? 1 : -1;
      setValue (value () + valueDelta * mValueIncrement);
      if (mCallback)
        mCallback (mValue);
      return true;
    }
    return false;
  }

private:
  std::string mNumberFormat;
  Scalar mMouseDownValue;
  Scalar mValueIncrement;
  Scalar mMinValue, mMaxValue;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
