/*
    /kabuki/toolkit/gui/widget.h -- Base class of all widgets

    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


#pragma once
#include <pch.h>


#include "t_object.h"
#include "t_theme.h"
#include "t_layout.h"
#include "t_theme.h"
#include "t_window.h"
#include "t_opengl.h"
#include "t_screen.h"
#include "t_serializer.h"

#include <vector>

namespace _ {

enum class Cursor;// do not put a docstring, this is already documented

/*
 * \class Widget widget.h /kabuki/toolkit/gui/widget.h
 *
 * @brief Base class of all widgets.
 *
 * @ref Widget is the base class of all widgets in \c nanogui. It can
 * also be used as an panel to arrange an arbitrary number of child
 * widgets using a layout generator (see @ref Layout).
 */
class SDK Widget : public Object {
public:
  // Construct a new widget with the given parent widget
  Widget (Widget *parent)
    : mParent (nullptr), mTheme (nullptr), mLayout (nullptr),
    mPos (Vector2i::Zero ()), mSize (Vector2i::Zero ()),
    mFixedSize (Vector2i::Zero ()), mVisible (true), mEnabled (true),
    mFocused (false), mMouseFocus (false), mTooltip (""), mFontSize (-1.0f),
    mIconExtraScale (1.0f), mCursor (Cursor::Arrow) {
    if (parent != nullptr) {
      parent->addChild (this);
}
  }

  // Return the parent widget
  Widget *parent () { return mParent; }
  // Return the parent widget
  const Widget *parent () const { return mParent; }
  // Set the parent widget
  void setParent (Widget *parent) { mParent = parent; }

  // Return the used @ref Layout generator
  Layout *layout () { return mLayout; }
  // Return the used @ref Layout generator
  const Layout *layout () const { return mLayout.get (); }
  // Set the used @ref Layout generator
  void setLayout (Layout *layout) { mLayout = layout; }

  // Return the @ref Theme used to draw this widget
  Theme *theme () { return mTheme; }
  // Return the @ref Theme used to draw this widget
  const Theme *theme () const { return mTheme.get (); }

  // Set the @ref Theme used to draw this widget
  virtual void setTheme (Theme *theme) {
    if (mTheme.get () == theme)
      return;
    mTheme = theme;
    for (auto child : mChildren)
      child->setTheme (theme);
  }

  // Return the position relative to the parent widget
  const Vector2i &position () const { return mPos; }
  // Set the position relative to the parent widget
  void setPosition (const Vector2i &pos) { mPos = pos; }

  // Return the absolute position on screen
  Vector2i absolutePosition () const {
    return mParent ?
      (parent ()->absolutePosition () + mPos) : mPos;
  }

  // Return the size of the widget
  const Vector2i &size () const { return mSize; }
  // set the size of the widget
  void setSize (const Vector2i &size) { mSize = size; }

  // Return the width of the widget
  int width () const { return mSize.x (); }
  // Set the width of the widget
  void setWidth (int width) { mSize.x () = width; }

  // Return the height of the widget
  int height () const { return mSize.y (); }
  // Set the height of the widget
  void setHeight (int height) { mSize.y () = height; }

  /*
   * @brief Set the fixed size of this widget
   *
   * If nonzero, components of the fixed size attribute override any values
   * computed by a layout generator associated with this widget. Note that
   * just setting the fixed size alone is not enough to actually change its
   * size; this is done with a call to @ref setSize or a call to @ref performLayout()
   * in the parent widget.
   */
  void setFixedSize (const Vector2i &fixedSize) { mFixedSize = fixedSize; }

  // Return the fixed size (see @ref setFixedSize())
  const Vector2i &fixedSize () const { return mFixedSize; }

  // Return the fixed width (see @ref setFixedSize())
  int fixedWidth () const { return mFixedSize.x (); }
  // Return the fixed height (see @ref setFixedSize())
  int fixedHeight () const { return mFixedSize.y (); }
  // Set the fixed width (see @ref setFixedSize())
  void setFixedWidth (int width) { mFixedSize.x () = width; }
  // Set the fixed height (see @ref setFixedSize())
  void setFixedHeight (int height) { mFixedSize.y () = height; }

  // Return whether or not the widget is currently visible (assuming all parents are visible)
  bool visible () const { return mVisible; }
  // Set whether or not the widget is currently visible (assuming all parents are visible)
  void setVisible (bool visible) { mVisible = visible; }

  // Check if this widget is currently visible, taking parent widgets into account
  bool visibleRecursive () const {
    bool visible = true;
    const Widget *widget = this;
    while (widget != nullptr) {
      visible &= widget->visible ();
      widget = widget->parent ();
    }
    return visible;
  }

  // Return the number of child widgets
  int childCount () const { return (int)mChildren.size (); }

  // Return the list of child widgets of the current widget
  const std::vector<Widget *> &children () const { return mChildren; }

  /*
   * @brief Add a child widget to the current widget at
   * the specified index.
   *
   * This function almost never needs to be called by hand,
   * since the constructor of @ref Widget automatically
   * adds the current widget to its parent
   */
  virtual void addChild (int index, Widget *widget) {
    DASSERT (index <= childCount ());
    mChildren.insert (mChildren.begin () + index, widget);
    widget->incRef ();
    widget->setParent (this);
    widget->setTheme (mTheme);
  }

  // Convenience function which appends a widget at the end
  void addChild (Widget *widget) {
    addChild (childCount (), widget);
  }

  // Remove a child widget by index
  void removeChild (int index) {
    mChildren.erase (std::remove (mChildren.begin (), mChildren.end (), widget), mChildren.end ());
    widget->decRef ();
  }

  // Remove a child widget by value
  void removeChild (const Widget *widget) {
    Widget *widget = mChildren[index];
    mChildren.erase (mChildren.begin () + index);
    widget->decRef ();
  }

  // Retrieves the child at the specific position
  const Widget* childAt (int index) const { return mChildren[index]; }

  // Retrieves the child at the specific position
  Widget* childAt (int index) { return mChildren[index]; }

  // Returns the index of a specific child or -1 if not found
  int childIndex (Widget* widget) const {
    auto it = std::find (mChildren.begin (), mChildren.end (), widget);
    if (it == mChildren.end ())
      return -1;
    return (int)(it - mChildren.begin ());
  }

  // Variadic shorthand notation to construct and add a child widget
  template<typename WidgetClass, typename... Args>
  WidgetClass* add (const Args&... args) {
    return new WidgetClass (this, args...);
  }

  // Walk up the hierarchy and return the parent window
  Window *window () {
    Widget *widget = this;
    while (true) {
      if (!widget)
        throw std::runtime_error (
          "Widget:internal error (could not find parent window)");
      Window *window = dynamic_cast<Window *>(widget);
      if (window != nullptr) {
        return window;
}
      widget = widget->parent ();
    }
  }

  // Walk up the hierarchy and return the parent screen
  Screen *screen () {
    Widget *widget = this;
    while (true) {
      if (!widget)
        throw std::runtime_error (
          "Widget:internal error (could not find parent screen)");
      Screen *screen = dynamic_cast<Screen *>(widget);
      if (screen != nullptr) {
        return screen;
}
      widget = widget->parent ();
    }
  }

  // Associate this widget with an ID value (optional)
  void setId (const std::string &id) { mId = id; }
  // Return the ID value associated with this widget, if any
  const std::string &id () const { return mId; }

  // Return whether or not this widget is currently enabled
  bool enabled () const { return mEnabled; }
  // Set whether or not this widget is currently enabled
  void setEnabled (bool enabled) { mEnabled = enabled; }

  // Return whether or not this widget is currently focused
  bool focused () const { return mFocused; }
  // Set whether or not this widget is currently focused
  void setFocused (bool focused) { mFocused = focused; }
  // Request the focus to be moved to this widget
  void requestFocus () {
    Widget *widget = this;
    while (widget->parent () != nullptr) {
      widget = widget->parent ();
}
    ((Screen *)widget)->updateFocus (this);
  }

  const std::string &tooltip () const { return mTooltip; }
  void setTooltip (const std::string &tooltip) { mTooltip = tooltip; }

  // Return current font size. If not set the default of the current theme will be returned
  int fontSize () const {
    return (mFontSize < 0 && mTheme) ? mTheme->mStandardFontSize : mFontSize;
  }

  // Set the font size of this widget
  void setFontSize (int fontSize) { mFontSize = fontSize; }
  // Return whether the font size is explicitly specified for this widget
  bool hasFontSize () const { return mFontSize > 0; }

  /*
   * The amount of extra scaling applied to *icon* fonts.
   * See @ref _::Widget::mIconExtraScale.
   */
  float iconExtraScale () const { return mIconExtraScale; }

  /*
   * Sets the amount of extra scaling applied to *icon* fonts.
   * See @ref _::Widget::mIconExtraScale.
   */
  void setIconExtraScale (float scale) { mIconExtraScale = scale; }

  // Return a pointer to the cursor of the widget
  Cursor cursor () const { return mCursor; }
  // Set the cursor of the widget
  void setCursor (Cursor cursor) { mCursor = cursor; }

  // Check if the widget contains a certain position
  bool contains (const Vector2i &p) const {
    auto d = (p - mPos).array ();
    return (d >= 0).all () && (d < mSize.array ()).all ();
  }

  // Determine the widget located at the given position value (recursive)
  Widget *findWidget (const Vector2i &p) {
    for (auto it = mChildren.rbegin (); it != mChildren.rend (); ++it) {
      Widget *child = *it;
      if (child->visible () && child->contains (p - mPos))
        return child->findWidget (p - mPos);
    }
    return contains (p) ? this : nullptr;
  }

  // Handle a mouse button event (default implementation: propagate to children)
  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down,
    int modifiers) {
    for (auto it = mChildren.rbegin (); it != mChildren.rend (); ++it) {
      Widget *child = *it;
      if (child->visible () && child->contains (p - mPos) &&
        child->mouseButtonEvent (p - mPos, button, down, modifiers))
        return true;
    }
    if (button == GLFW_MOUSE_BUTTON_1 && down && !mFocused)
      requestFocus ();
    return false;
  }

  // Handle a mouse motion event (default implementation: propagate to children)
  virtual bool mouseMotionEvent (const Vector2i &p, const Vector2i &rel,
    int button, int modifiers) {
    for (auto it = mChildren.rbegin (); it != mChildren.rend (); ++it) {
      Widget *child = *it;
      if (!child->visible ())
        continue;
      bool contained = child->contains (p - mPos), prevContained = child->contains (p - mPos - rel);
      if (contained != prevContained)
        child->mouseEnterEvent (p, contained);
      if ((contained || prevContained) &&
        child->mouseMotionEvent (p - mPos, rel, button, modifiers))
        return true;
    }
    return false;
  }

  // Handle a mouse drag event (default implementation: do nothing)
  virtual bool mouseDragEvent (const Vector2i &p, const Vector2i &rel,
    int button, int modifiers) {
    return false;
  }

  // Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
  virtual bool mouseEnterEvent (const Vector2i &p, bool enter) {
    mMouseFocus = enter;
    return false;
  }

  // Handle a mouse scroll event (default implementation: propagate to children)
  virtual bool scrollEvent (const Vector2i &p, const Vector2f &rel) {
    for (auto it = mChildren.rbegin (); it != mChildren.rend (); ++it) {
      Widget *child = *it;
      if (!child->visible ())
        continue;
      if (child->contains (p - mPos) && child->scrollEvent (p - mPos, rel))
        return true;
    }
    return false;
  }

  // Handle a focus change event (default implementation: record the focus status, but do nothing)
  virtual bool focusEvent (bool focused) {
    mFocused = focused;
    return false;
  }

  // Handle a keyboard event (default implementation: do nothing)
  virtual bool keyboardEvent (int key, int scancode, int action, int modifiers)
  {
    return false;
  }

  // Handle text input (UTF-32 format) (default implementation: do nothing)
  virtual bool keyboardCharacterEvent (unsigned int codepoint) {
    return false;
  }

  // Compute the preferred size of the widget
  virtual Vector2i preferredSize (NVGcontext *ctx) const {
    if (mLayout)
      return mLayout->preferredSize (ctx, this);
    else
      return mSize;
  }

  // Invoke the associated layout generator to properly place child widgets, if any
  virtual void performLayout (NVGcontext *ctx) {
    if (mLayout) {
      mLayout->performLayout (ctx, this);
    } else {
      for (auto c : mChildren) {
        Vector2i pref = c->preferredSize (ctx), fix = c->fixedSize ();
        c->setSize (Vector2i (
          fix[0] ? fix[0] : pref[0],
          fix[1] ? fix[1] : pref[1]
        ));
        c->performLayout (ctx);
      }
    }
  }

  // Draw the widget (and all child widgets)
  virtual void draw (NVGcontext *ctx) {
#if NANOGUI_SHOW_WIDGET_BOUNDS
    nvgStrokeWidth (ctx, 1.0f);
    nvgBeginPath (ctx);
    nvgRect (ctx, mPos.x () - 0.5f, mPos.y () - 0.5f, mSize.x () + 1, mSize.y () + 1);
    nvgStrokeColor (ctx, nvgRGBA (255, 0, 0, 255));
    nvgStroke (ctx);
#endif

    if (mChildren.empty ())
      return;

    nvgSave (ctx);
    nvgTranslate (ctx, mPos.x (), mPos.y ());
    for (auto child : mChildren) {
      if (child->visible ()) {
        nvgSave (ctx);
        nvgIntersectScissor (ctx, child->mPos.x (), child->mPos.y (), child->mSize.x (), child->mSize.y ());
        child->draw (ctx);
        nvgRestore (ctx);
      }
    }
    nvgRestore (ctx);
  }

  // Save the state of the widget into the given @ref Serializer instance
  virtual void save (Serializer &s) const {
    s.set ("position", mPos);
    s.set ("size", mSize);
    s.set ("fixedSize", mFixedSize);
    s.set ("visible", mVisible);
    s.set ("enabled", mEnabled);
    s.set ("focused", mFocused);
    s.set ("tooltip", mTooltip);
    s.set ("fontSize", mFontSize);
    s.set ("cursor", (int)mCursor);
  }

  // Restore the state of the widget from the given @ref Serializer instance
  virtual bool load (Serializer &s) {
    if (!s.get ("position", mPos)) return false;
    if (!s.get ("size", mSize)) return false;
    if (!s.get ("fixedSize", mFixedSize)) return false;
    if (!s.get ("visible", mVisible)) return false;
    if (!s.get ("enabled", mEnabled)) return false;
    if (!s.get ("focused", mFocused)) return false;
    if (!s.get ("tooltip", mTooltip)) return false;
    if (!s.get ("fontSize", mFontSize)) return false;
    if (!s.get ("cursor", mCursor)) return false;
    return true;
  }

protected:
  // Free all resources used by the widget and any children
  ~Widget () override {
    for (auto child : mChildren) {
      if (child)
        child->decRef ();
    }
  }

  /*
   * Convenience definition for subclasses to get the full icon scale for this
   * class of Widget.  It simple returns the value
   * `mTheme->mIconScale * this->mIconExtraScale`.
   *
   * \remark
   *     See also: @ref _::Theme::mIconScale and
   *     @ref _::Widget::mIconExtraScale.  This tiered scaling
   *     strategy may not be appropriate with fonts other than `entypo.ttf`.
   */
  inline float icon_scale () const { return mTheme->mIconScale * mIconExtraScale; }

protected:
  Widget *mParent;
  ref<Theme> mTheme;
  ref<Layout> mLayout;
  std::string mId;
  Vector2i mPos, mSize, mFixedSize;
  std::vector<Widget *> mChildren;

  /*
   * Whether or not this Widget is currently visible.  When a Widget is not
   * currently visible, no time is wasted executing its drawing method.
   */
  bool mVisible;

  /*
   * Whether or not this Widget is currently enabled.  Various different kinds
   * of derived types use this to determine whether or not user input will be
   * accepted.  For example, when `mEnabled == false`, the state of a
   * CheckBox cannot be changed, or a TextBox will not allow new input.
   */
  bool mEnabled;
  bool mFocused, mMouseFocus;
  std::string mTooltip;
  int mFontSize;

  /* The amount of extra icon scaling used in addition the the theme's
  default icon font scale.  Default value is `1.0`, which implies that
  @ref _::Widget::icon_scale simply returns the value of
  @ref _::Theme::mIconScale.
  Most widgets do not need extra scaling, but some (e.g., CheckBox, TextBox)
   * need to adjust the Theme's default icon scaling
   * (@ref _::Theme::mIconScale) to properly display icons within their
   * bounds (upscale, or downscale).
   *
   * \rst
   * .. note::
   *
   *    When using `nvgFontSize` for icons in subclasses, make sure to call
   *    the :func:`_::Widget::icon_scale` function.  Expected usage when
   *    *drawing* icon fonts is something like:
   *
   *    @code
   *
   *       virtual void draw(NVGcontext *ctx) {
   *           // fontSize depends on the kind of Widget.  Search for `FontSize`
   *           // in the Theme class (e.g., standard vs button)
   *           float ih = fontSize;
   *           // assuming your Widget has a declared `mIcon`
   *           if (nvgIsFontIcon(mIcon)) {
   *               ih *= icon_scale();
   *               nvgFontFace(ctx, "icons");
   *               nvgFontSize(ctx, ih);
   *               // remaining drawing code (see button.cpp for more)
   *           }
   *       }
   * \endrst
   */
  float mIconExtraScale;
  Cursor mCursor;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}  // namespace _
