/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/
/*
 * @file /kabuki/toolkit/gui/glcanvas.h
 *
 * @brief Canvas widget for rendering OpenGL content.  This widget was
 *        contributed by Jan Winkler.
 */

#pragma once
#include <pch.h>


#include <iostream>

#include "t_widget.h"
#include "t_opengl.h"
#include "t_glutil.h"
#include "t_window.h"
#include "t_screen.h"
#include "t_theme.h"
#include "t_opengl.h"
#include "t_serializer.h"

namespace _ {

/*
 * \class GLCanvas glcanvas.h /kabuki/toolkit/gui/glcanvas.h
 *
 * @brief Canvas widget for rendering OpenGL content.  This widget was
 *        contributed by Jan Winkler.
 *
 * Canvas widget that can be used to display arbitrary OpenGL content. This is
 * useful to display and manipulate 3D objects as part of an interactive
 * application. The implementation uses scissoring to ensure that rendered
 * objects don't spill into neighboring widgets.
 *
 * \rst
 * **Usage**
 *     Override :func:`_::GLCanvas::drawGL` in subclasses to provide
 *     custom drawing code.  See :ref:`nanogui_example_4`.
 *
 * \endrst
 */
class SDK GLCanvas : public Widget {
public:
    /*
     * Creates a GLCanvas attached to the specified parent.
     *
     * @param parent
     *     The Widget to attach this GLCanvas to.
     */
    GLCanvas(Widget *parent)
      : Widget (parent), mBackgroundColor (Vector4i (128, 128, 128, 255)),
      mDrawBorder (true) {
      mSize = Vector2i (250, 250);
    }

    // Returns the background color.
    const Color &backgroundColor() const { return mBackgroundColor; }

    // Sets the background color.
    void setBackgroundColor(const Color &backgroundColor) { mBackgroundColor = backgroundColor; }

    // Set whether to draw the widget border or not.
    void setDrawBorder(const bool bDrawBorder) { mDrawBorder = bDrawBorder; }

    // Return whether the widget border gets drawn or not.
    const bool &drawBorder() const { return mDrawBorder; }

    // Draw the canvas.
    virtual void draw(NVGcontext *ctx) override {
      Widget::draw (ctx);
      nvgEndFrame (ctx);

      if (mDrawBorder)
        drawWidgetBorder (ctx);

      const Screen* screen = this->screen ();
      DASSERT (screen);

      float pixelRatio = screen->pixelRatio ();
      Vector2f screenSize = screen->size ().cast<float> ();
      Vector2i positionInScreen = absolutePosition ();

      Vector2i size = (mSize.cast<float> () * pixelRatio).cast<int> (),
        imagePosition = (Vector2f (positionInScreen[0],
          screenSize[1] - positionInScreen[1] -
          (float)mSize[1]) * pixelRatio).cast<int> ();

      GLint storedViewport[4];
      glGetIntegerv (GL_VIEWPORT, storedViewport);

      glViewport (imagePosition[0], imagePosition[1], size[0], size[1]);

      glEnable (GL_SCISSOR_TEST);
      glScissor (imagePosition[0], imagePosition[1], size[0], size[1]);
      glClearColor (mBackgroundColor[0], mBackgroundColor[1],
        mBackgroundColor[2], mBackgroundColor[3]);
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      this->drawGL ();

      glDisable (GL_SCISSOR_TEST);
      glViewport (storedViewport[0], storedViewport[1],
        storedViewport[2], storedViewport[3]);
    }

    // Draw the GL scene. Override this method to draw the actual GL content.
    virtual void drawGL() {}

    // Save the state of this GLCanvas to the specified Serializer.
    virtual void save(Serializer &s) const override {
      Widget::save (s);
      s.set ("backgroundColor", mBackgroundColor);
      s.set ("drawBorder", mDrawBorder);

    // Set the state of this GLCanvas from the specified Serializer.
    virtual bool load(Serializer &s) override {
      if (!Widget::load (s)) return false;
      if (!s.get ("backgroundColor", mBackgroundColor)) return false;
      if (!s.get ("drawBorder", mDrawBorder)) return false;
      return true;
    }

protected:
    // Internal helper function for drawing the widget border
    void drawWidgetBorder(NVGcontext* ctx) const {
      nvgBeginPath (ctx);
      nvgStrokeWidth (ctx, 1.0f);
      nvgRoundedRect (ctx, mPos.x () - 0.5f, mPos.y () - 0.5f,
        mSize.x () + 1, mSize.y () + 1, mTheme->mWindowCornerRadius);
      nvgStrokeColor (ctx, mTheme->mBorderLight);
      nvgRoundedRect (ctx, mPos.x () - 1.0f, mPos.y () - 1.0f,
        mSize.x () + 2, mSize.y () + 2, mTheme->mWindowCornerRadius);
      nvgStrokeColor (ctx, mTheme->mBorderDark);
      nvgStroke (ctx);
    }

protected:
    // The background color (what is used with `glClearColor`).
    Color mBackgroundColor;

    // Whether to draw the widget border or not.
    bool mDrawBorder;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
