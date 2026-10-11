/*
    /kabuki/toolkit/gui/screen.h -- Top-level widget and interface between NanoGUI and GLFW

    A significant redesign of this code was contributed by Christian Schueller.

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
#include "t_window.h"
#include "t_popup.h"

#include <map>
#include <iostream>

namespace _ {

/* Represents a display surface (i.e. a full-screen or windowed GLFW window)
and forms the root element of a hierarchy of nanogui widgets. */
class SDK Screen : public Widget {
  friend class Widget;
  friend class Window;
public:
  /* Create a new Screen instance
  @param size Size in pixels at 96 dpi (on high-DPI screens, the actual resolution in terms of hardware pixels may be larger by an integer factor)
  @param caption Window title (in UTF-8 encoding)
  @param resizable If creating a window, should it be resizable?
  @param fullscreen Specifies whether to create a windowed or full-screen view
  @param colorBits Number of bits per pixel dedicated to the R/G/B color components
  @param alphaBits Number of bits per pixel dedicated to the alpha channel
  @param depthBits Number of bits per pixel dedicated to the Z-buffer
  @param stencilBits Number of bits per pixel dedicated to the stencil buffer
  (recommended to set this to 8. NanoVG can draw higher-quality strokes using
  a stencil buffer)
  @param nSamples Number of MSAA samples (set to 0 to disable)
  @param glMajor
  The requested OpenGL Major version number.  Default is 3, if changed the
  value must correspond to a forward compatible core profile (for portability
  reasons).  For example, set this to 4 and @ref glMinor to 1 for a forward
  compatible core OpenGL 4.1 profile.  Requesting an invalid profile will
  result in no context (and therefore no GUI being created.
  @param glMinor The requested OpenGL Minor version number.  Default is 3, if
  changed the value must correspond to a forward compatible core profile (for
  portability reasons).  For example, set this to 1 and @ref glMajor to 4 for
  a forward compatible core OpenGL 4.1 profile.  Requesting an invalid profile
  will result in no context (and therefore no GUI) being created. */
  Screen (const Vector2i &size, const std::string &caption,
    bool resizable = true, bool fullscreen = false, int colorBits = 8,
    int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
    int nSamples = 0,
    unsigned int glMajor = 3, unsigned int glMinor = 3)
    : Widget (nullptr), mGLFWWindow (nullptr), mNVGContext (nullptr),
     mBackground (0.3f, 0.3f, 0.32f, 1.f), mCaption (caption),
     mFullscreen (fullscreen) {
    memset (mCursors, 0, sizeof (GLFWcursor *) * (int)Cursor::CursorCount);

    /* Request a forward compatible OpenGL glMajor.glMinor core profile context.
       Default value is an OpenGL 3.3 core profile context. */
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, glMajor);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, glMinor);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint (GLFW_SAMPLES, nSamples);
    glfwWindowHint (GLFW_RED_BITS, colorBits);
    glfwWindowHint (GLFW_GREEN_BITS, colorBits);
    glfwWindowHint (GLFW_BLUE_BITS, colorBits);
    glfwWindowHint (GLFW_ALPHA_BITS, alphaBits);
    glfwWindowHint (GLFW_STENCIL_BITS, stencilBits);
    glfwWindowHint (GLFW_DEPTH_BITS, depthBits);
    glfwWindowHint (GLFW_VISIBLE, GL_FALSE);
    glfwWindowHint (GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

    if (fullscreen) {
      GLFWmonitor *monitor = glfwGetPrimaryMonitor ();
      const GLFWvidmode *mode = glfwGetVideoMode (monitor);
      mGLFWWindow = glfwCreateWindow (mode->width, mode->height,
        caption.c_str (), monitor, nullptr);
    } else {
      mGLFWWindow = glfwCreateWindow (size.x (), size.y (),
        caption.c_str (), nullptr, nullptr);
    }

    if (!mGLFWWindow)
      throw std::runtime_error ("Could not create an OpenGL " +
        std::to_string (glMajor) + "." +
        std::to_string (glMinor) + " context!");

    glfwMakeContextCurrent (mGLFWWindow);

#if defined(NANOGUI_GLAD)
    if (!gladInitialized) {
      gladInitialized = true;
      if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error ("Could not initialize GLAD!");
      glGetError (); // pull and ignore unhandled errors like GL_INVALID_ENUM
    }
#endif

    glfwGetFramebufferSize (mGLFWWindow, &mFBSize[0], &mFBSize[1]);
    glViewport (0, 0, mFBSize[0], mFBSize[1]);
    glClearColor (mBackground[0], mBackground[1], mBackground[2], mBackground[3]);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glfwSwapInterval (0);
    glfwSwapBuffers (mGLFWWindow);

#if defined(__APPLE__)
    /* Poll for events once before starting a potentially
       lengthy loading process. This is needed to be
       classified as "interactive" by other software such
       as iTerm2 */

    glfwPollEvents ();
#endif

    /* Propagate GLFW events to the appropriate Screen instance */
    glfwSetCursorPosCallback (mGLFWWindow,
      [](GLFWwindow *w, double x, double y) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;
      if (!s->mProcessEvents)
        return;
      s->cursorPosCallbackEvent (x, y);
    }
    );

    glfwSetMouseButtonCallback (mGLFWWindow,
      [](GLFWwindow *w, int button, int action, int modifiers) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;
      if (!s->mProcessEvents)
        return;
      s->mouseButtonCallbackEvent (button, action, modifiers);
    }
    );

    glfwSetKeyCallback (mGLFWWindow,
      [](GLFWwindow *w, int key, int scancode, int action, int mods) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;
      if (!s->mProcessEvents)
        return;
      s->keyCallbackEvent (key, scancode, action, mods);
    }
    );

    glfwSetCharCallback (mGLFWWindow,
      [](GLFWwindow *w, unsigned int codepoint) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;
      if (!s->mProcessEvents)
        return;
      s->charCallbackEvent (codepoint);
    }
    );

    glfwSetDropCallback (mGLFWWindow,
      [](GLFWwindow *w, int count, const char **filenames) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;
      if (!s->mProcessEvents)
        return;
      s->dropCallbackEvent (count, filenames);
    }
    );

    glfwSetScrollCallback (mGLFWWindow,
      [](GLFWwindow *w, double x, double y) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;
      if (!s->mProcessEvents)
        return;
      s->scrollCallbackEvent (x, y);
    }
    );

    /* React to framebuffer size events -- includes window
       size events and also catches things like dragging
       a window from a Retina-capable screen to a normal
       screen on Mac OS X */
    glfwSetFramebufferSizeCallback (mGLFWWindow,
      [](GLFWwindow *w, int width, int height) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;
      Screen *s = it->second;

      if (!s->mProcessEvents)
        return;

      s->resizeCallbackEvent (width, height);
    }
    );

    // notify when the screen has lost focus (e.g. application switch)
    glfwSetWindowFocusCallback (mGLFWWindow,
      [](GLFWwindow *w, int focused) {
      auto it = __nanogui_screens.find (w);
      if (it == __nanogui_screens.end ())
        return;

      Screen *s = it->second;
      // focused: 0 when false, 1 when true
      s->focusEvent (focused != 0);
    }
    );

    initialize (mGLFWWindow, true);
  }

  // Release all resources
  ~Screen () override {
    __nanogui_screens.erase (mGLFWWindow);
    for (int i = 0; i < static_cast<int>(Cursor::CursorCount); ++i) {
      if (mCursors[i])
        glfwDestroyCursor (mCursors[i]);
    }
    if (mNVGContext)
      nvgDeleteGL3 (mNVGContext);
    if (mGLFWWindow && mShutdownGLFWOnDestruct)
      glfwDestroyWindow (mGLFWWindow);
  }

  // Get the window title bar caption
  const std::string &caption () const { return mCaption; }

  // Set the window title bar caption
  void setCaption (const std::string &caption) {
    if (caption != mCaption) {
      glfwSetWindowTitle (mGLFWWindow, caption.c_str ());
      mCaption = caption;
    }
  }

  // Return the screen's background color
  const Color &background () const { return mBackground; }

  // Set the screen's background color
  void setBackground (const Color &background) { mBackground = background; }

  // Set the top-level window visibility (no effect on full-screen windows)
  void setVisible (bool visible) {
    if (mVisible != visible) {
      mVisible = visible;

      if (visible)
        glfwShowWindow (mGLFWWindow);
      else
        glfwHideWindow (mGLFWWindow);
    }
  }

  // Set window size
  void setSize (const Vector2i& size) {
    Widget::setSize (size);

#if defined(_WIN32) || defined(__linux__)
    glfwSetWindowSize (mGLFWWindow, size.x () * mPixelRatio, size.y () * mPixelRatio);
#else
    glfwSetWindowSize (mGLFWWindow, size.x (), size.y ());
#endif
  }

  // Draw the Screen contents
  virtual void drawAll () {
    glClearColor (mBackground[0], mBackground[1], mBackground[2], mBackground[3]);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    drawContents ();
    drawWidgets ();

    glfwSwapBuffers (mGLFWWindow);
  }

  // Draw the window contents --- put your OpenGL draw calls here
  virtual void drawContents () { /* To be overridden */ }

  // Return the ratio between pixel and device coordinates (e.g. >= 2 on Mac Retina displays)
  float pixelRatio () const { return mPixelRatio; }

  // Handle a file drop event
  virtual bool dropEvent (const std::vector<std::string> & /* filenames */) { return false; /* To be overridden */ }

  // Default keyboard event handler
  bool keyboardEvent (int key, int scancode, int action, int modifiers) override {
    if (mFocusPath.size () > 0) {
      for (auto it = mFocusPath.rbegin () + 1; it != mFocusPath.rend (); ++it)
        if ((*it)->focused () && (*it)->keyboardEvent (key, scancode, action, modifiers))
          return true;
    }

    return false;
  }

  // Text input event handler: codepoint is native endian UTF-32 format
  bool keyboardCharacterEvent (unsigned int codepoint) override {
    if (mFocusPath.size () > 0) {
      for (auto it = mFocusPath.rbegin () + 1; it != mFocusPath.rend (); ++it)
        if ((*it)->focused () && (*it)->keyboardCharacterEvent (codepoint))
          return true;
    }
    return false;
  }

  // Window resize event handler
  virtual bool resizeEvent (const Vector2i& size) {
    return ;
  }

  // Set the resize callback
  std::function<void (Vector2i)> resizeCallback () const { return mResizeCallback; }
  void setResizeCallback (const std::function<void (Vector2i)> &callback) { mResizeCallback = callback; }

  // Return the last observed mouse position value
  Vector2i mousePos () const { return mMousePos; }

  // Return a pointer to the underlying GLFW window data structure
  GLFWwindow *glfwWindow () { return mGLFWWindow; }

  // Return a pointer to the underlying nanoVG draw context
  NVGcontext *nvgContext () { return mNVGContext; }

  void setShutdownGLFWOnDestruct (bool v) { mShutdownGLFWOnDestruct = v; }
  bool shutdownGLFWOnDestruct () { return mShutdownGLFWOnDestruct; }

  using Widget::performLayout;

  // Compute the layout of all widgets
  void performLayout () {
    Widget::performLayout (mNVGContext);
  }

public:
  /******** API for applications which manage GLFW themselves *********/

  /*
   * @brief Default constructor
   *
   * Performs no initialization at all. Use this if the application is
   * responsible for setting up GLFW, OpenGL, etc.
   *
   * In this case, override @ref Screen and call @ref initalize() with a
   * pointer to an existing \c GLFWwindow instance
   *
   * You will also be responsible in this case to deliver GLFW callbacks
   * to the appropriate callback event handlers below
   */
  Screen ()
    : Widget (nullptr),  mBackground (0.3f, 0.3f, 0.32f, 1.f) {
    memset (mCursors, 0, sizeof (GLFWcursor *) * (int)Cursor::CursorCount);
  }

  // Initialize the @ref Screen
  void initialize (GLFWwindow *window, bool shutdownGLFWOnDestruct) {
    mGLFWWindow = window;
    mShutdownGLFWOnDestruct = shutdownGLFWOnDestruct;
    glfwGetWindowSize (mGLFWWindow, &mSize[0], &mSize[1]);
    glfwGetFramebufferSize (mGLFWWindow, &mFBSize[0], &mFBSize[1]);

    mPixelRatio = get_pixel_ratio (window);

#if defined(_WIN32) || defined(__linux__)
    if (mPixelRatio != 1 && !mFullscreen)
      glfwSetWindowSize (window, mSize.x () * mPixelRatio, mSize.y () * mPixelRatio);
#endif

#if defined(NANOGUI_GLAD)
    if (!gladInitialized) {
      gladInitialized = true;
      if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error ("Could not initialize GLAD!");
      glGetError (); // pull and ignore unhandled errors like GL_INVALID_ENUM
    }
#endif

    /* Detect framebuffer properties and set up compatible NanoVG context */
    GLint nStencilBits = 0, nSamples = 0;
    glGetFramebufferAttachmentParameteriv (GL_DRAW_FRAMEBUFFER,
      GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &nStencilBits);
    glGetIntegerv (GL_SAMPLES, &nSamples);

    int flags = 0;
    if (nStencilBits >= 8)
      flags |= NVG_STENCIL_STROKES;
    if (nSamples <= 1)
      flags |= NVG_ANTIALIAS;
#if !defined(NDEBUG)
    flags |= NVG_DEBUG;
#endif

    mNVGContext = nvgCreateGL3 (flags);
    if (mNVGContext == nullptr)
      throw std::runtime_error ("Could not initialize NanoVG!");

    mVisible = glfwGetWindowAttrib (window, GLFW_VISIBLE) != 0;
    setTheme (new Theme (mNVGContext));
    mMousePos = Vector2i::Zero ();
    mMouseState = mModifiers = 0;
    mDragActive = false;
    mLastInteraction = glfwGetTime ();
    mProcessEvents = true;
    __nanogui_screens[mGLFWWindow] = this;

    for (int i = 0; i < (int)Cursor::CursorCount; ++i)
      mCursors[i] = glfwCreateStandardCursor (GLFW_ARROW_CURSOR + i);

    // Fixes retina display-related font rendering issue (#185)
    nvgBeginFrame (mNVGContext, mSize[0], mSize[1], mPixelRatio);
    nvgEndFrame (mNVGContext);
  }

  /* Event handlers */
  bool cursorPosCallbackEvent (double x, double y) {
    Vector2i p ((int)x, (int)y);

#if defined(_WIN32) || defined(__linux__)
    p = (p.cast<float> () / mPixelRatio).cast<int> ();
#endif

    bool ret = false;
    mLastInteraction = glfwGetTime ();
    try {
      p -= Vector2i (1, 2);

      if (!mDragActive) {
        Widget *widget = findWidget (p);
        if (widget != nullptr && widget->cursor () != mCursor) {
          mCursor = widget->cursor ();
          glfwSetCursor (mGLFWWindow, mCursors[(int)mCursor]);
        }
      } else {
        ret = mDragWidget->mouseDragEvent (
          p - mDragWidget->parent ()->absolutePosition (), p - mMousePos,
          mMouseState, mModifiers);
      }

      if (!ret)
        ret = mouseMotionEvent (p, p - mMousePos, mMouseState, mModifiers);

      mMousePos = p;

      return ret;
    } catch (const std::exception &e) {
      std::cerr << "Caught exception in event handler: " << e.what () << std::endl;
      return false;
    }
  }

  bool mouseButtonCallbackEvent (int button, int action, int modifiers) {
    mModifiers = modifiers;
    mLastInteraction = glfwGetTime ();
    try {
      if (mFocusPath.size () > 1) {
        const Window *window =
          dynamic_cast<Window *>(mFocusPath[mFocusPath.size () - 2]);
        if (window && window->modal ()) {
          if (!window->contains (mMousePos))
            return false;
        }
      }

      if (action == GLFW_PRESS)
        mMouseState |= 1 << button;
      else
        mMouseState &= ~(1 << button);

      auto dropWidget = findWidget (mMousePos);
      if (mDragActive && action == GLFW_RELEASE &&
        dropWidget != mDragWidget)
        mDragWidget->mouseButtonEvent (
          mMousePos - mDragWidget->parent ()->absolutePosition (), button,
          false, mModifiers);

      if (dropWidget != nullptr && dropWidget->cursor () != mCursor) {
        mCursor = dropWidget->cursor ();
        glfwSetCursor (mGLFWWindow, mCursors[(int)mCursor]);
      }

      if (action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2)) {
        mDragWidget = findWidget (mMousePos);
        if (mDragWidget == this)
          mDragWidget = nullptr;
        mDragActive = mDragWidget != nullptr;
        if (!mDragActive) {
          updateFocus (nullptr);
}
      } else {
        mDragActive = false;
        mDragWidget = nullptr;
      }

      return mouseButtonEvent (mMousePos, button, action == GLFW_PRESS,
        mModifiers);
    } catch (const std::exception &e) {
      std::cerr << "Caught exception in event handler: " << e.what () << std::endl;
      return false;
    }
  }

  bool keyCallbackEvent (int key, int scancode, int action, int mods) {
    mLastInteraction = glfwGetTime ();
    try {
      return keyboardEvent (key, scancode, action, mods);
    } catch (const std::exception &e) {
      std::cerr << "Caught exception in event handler: " << e.what () << std::endl;
      return false;
    }
  }

  bool charCallbackEvent (unsigned int codepoint) {
    mLastInteraction = glfwGetTime ();
    try {
      return keyboardCharacterEvent (codepoint);
    } catch (const std::exception &e) {
      std::cerr << "Caught exception in event handler: " << e.what ()
        << std::endl;
      return false;
    }
  }

  bool dropCallbackEvent (int count, const char **filenames) {
    std::vector<std::string> arg (count);
    for (int i = 0; i < count; ++i)
      arg[i] = filenames[i];
    return dropEvent (arg);
  }

  bool scrollCallbackEvent (double x, double y) {
    mLastInteraction = glfwGetTime ();
    try {
      if (mFocusPath.size () > 1) {
        const Window *window =
          dynamic_cast<Window *>(mFocusPath[mFocusPath.size () - 2]);
        if (window && window->modal ()) {
          if (!window->contains (mMousePos))
            return false;
        }
      }
      return scrollEvent (mMousePos, Vector2f (x, y));
    } catch (const std::exception &e) {
      std::cerr << "Caught exception in event handler: " << e.what ()
        << std::endl;
      return false;
    }
  }

  bool resizeCallbackEvent (int width, int height) {
    Vector2i fbSize, size;
    glfwGetFramebufferSize (mGLFWWindow, &fbSize[0], &fbSize[1]);
    glfwGetWindowSize (mGLFWWindow, &size[0], &size[1]);

#if defined(_WIN32) || defined(__linux__)
    size = (size.cast<float> () / mPixelRatio).cast<int> ();
#endif

    if (mFBSize == Vector2i (0, 0) || size == Vector2i (0, 0))
      return false;

    mFBSize = fbSize; mSize = size;
    mLastInteraction = glfwGetTime ();

    try {
      return resizeEvent (mSize);
    } catch (const std::exception &e) {
      std::cerr << "Caught exception in event handler: " << e.what ()
        << std::endl;
      return false;
    }
  }

  /* Internal helper functions */
  void updateFocus (Widget *widget) {
    for (auto w : mFocusPath) {
      if (!w->focused ())
        continue;
      w->focusEvent (false);
    }
    mFocusPath.clear ();
    Widget *window = nullptr;
    while (widget != nullptr) {
      mFocusPath.push_back (widget);
      if (dynamic_cast<Window *>(widget))
        window = widget;
      widget = widget->parent ();
    }
    for (auto it = mFocusPath.rbegin (); it != mFocusPath.rend (); ++it)
      (*it)->focusEvent (true);

    if (window != nullptr) {
      moveWindowToFront (reinterpret_cast<Window *>(window));
}
  }

  void disposeWindow (Window *window) {
    if (std::find (mFocusPath.begin (), mFocusPath.end (), window) != mFocusPath.end ())
      mFocusPath.clear ();
    if (mDragWidget == window)
      mDragWidget = nullptr;
    removeChild (window);
  }

  void centerWindow (Window *window) {
    if (window->size () == Vector2i::Zero ()) {
      window->setSize (window->preferredSize (mNVGContext));
      window->performLayout (mNVGContext);
    }
    window->setPosition ((mSize - window->size ()) / 2);
  }

  void moveWindowToFront (Window *window) {
    mChildren.erase (std::remove (mChildren.begin (), mChildren.end (), window), mChildren.end ());
    mChildren.push_back (window);
    /* Brute force topological sort (no problem for a few windows..) */
    bool changed = false;
    do {
      size_t baseIndex = 0;
      for (size_t index = 0; index < mChildren.size (); ++index)
        if (mChildren[index] == window)
          baseIndex = index;
      changed = false;
      for (size_t index = 0; index < mChildren.size (); ++index) {
        Popup *pw = dynamic_cast<Popup *>(mChildren[index]);
        if (pw && pw->parentWindow () == window && index < baseIndex) {
          moveWindowToFront (pw);
          changed = true;
          break;
        }
      }
    } while (changed);
  }

  void drawWidgets () {
    if (!mVisible) {
      return;
}

    glfwMakeContextCurrent (mGLFWWindow);

    glfwGetFramebufferSize (mGLFWWindow, &mFBSize[0], &mFBSize[1]);
    glfwGetWindowSize (mGLFWWindow, &mSize[0], &mSize[1]);

#if defined(_WIN32) || defined(__linux__)
    mSize = (mSize.cast<float> () / mPixelRatio).cast<int> ();
    mFBSize = (mSize.cast<float> () * mPixelRatio).cast<int> ();
#else
    /* Recompute pixel ratio on OSX */
    if (mSize[0])
      mPixelRatio = (float)mFBSize[0] / (float)mSize[0];
#endif

    glViewport (0, 0, mFBSize[0], mFBSize[1]);
    glBindSampler (0, 0);
    nvgBeginFrame (mNVGContext, mSize[0], mSize[1], mPixelRatio);

    draw (mNVGContext);

    double elapsed = glfwGetTime () - mLastInteraction;

    if (elapsed > 0.5f) {
      /* Draw tooltips */
      const Widget *widget = findWidget (mMousePos);
      if (widget && !widget->tooltip ().empty ()) {
        int tooltipWidth = 150;

        float bounds[4];
        nvgFontFace (mNVGContext, "sans");
        nvgFontSize (mNVGContext, 15.0f);
        nvgTextAlign (mNVGContext, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgTextLineHeight (mNVGContext, 1.1f);
        Vector2i pos = widget->absolutePosition () +
          Vector2i (widget->width () / 2, widget->height () + 10);

        nvgTextBounds (mNVGContext, pos.x (), pos.y (),
          widget->tooltip ().c_str (), nullptr, bounds);
        int h = (bounds[2] - bounds[0]) / 2;
        if (h > tooltipWidth / 2) {
          nvgTextAlign (mNVGContext, NVG_ALIGN_CENTER | NVG_ALIGN_TOP);
          nvgTextBoxBounds (mNVGContext, pos.x (), pos.y (), tooltipWidth,
            widget->tooltip ().c_str (), nullptr, bounds);

          h = (bounds[2] - bounds[0]) / 2;
        }
        nvgGlobalAlpha (mNVGContext,
          std::min (1.0, 2 * (elapsed - 0.5f)) * 0.8);

        nvgBeginPath (mNVGContext);
        nvgFillColor (mNVGContext, Color (0, 255));
        nvgRoundedRect (mNVGContext, bounds[0] - 4 - h, bounds[1] - 4,
          (int)(bounds[2] - bounds[0]) + 8,
          (int)(bounds[3] - bounds[1]) + 8, 3);

        int px = static_cast<int>((bounds[2] + bounds[0]) / 2) - h;
        nvgMoveTo (mNVGContext, px, bounds[1] - 10);
        nvgLineTo (mNVGContext, px + 7, bounds[1] + 1);
        nvgLineTo (mNVGContext, px - 7, bounds[1] + 1);
        nvgFill (mNVGContext);

        nvgFillColor (mNVGContext, Color (255, 255));
        nvgFontBlur (mNVGContext, 0.0f);
        nvgTextBox (mNVGContext, pos.x () - h, pos.y (), tooltipWidth,
          widget->tooltip ().c_str (), nullptr);
      }
    }

    nvgEndFrame (mNVGContext);
  }

protected:
  GLFWwindow *mGLFWWindow{nullptr};
  NVGcontext *mNVGContext{nullptr};
  GLFWcursor *mCursors[static_cast<int>(Cursor::CursorCount)];
  Cursor mCursor{Cursor::Arrow};
  std::vector<Widget *> mFocusPath;
  Vector2i mFBSize;
  float mPixelRatio;
  int mMouseState, mModifiers;
  Vector2i mMousePos;
  bool mDragActive;
  Widget *mDragWidget = nullptr;
  double mLastInteraction;
  bool mProcessEvents;
  Color mBackground;
  std::string mCaption;
  bool mShutdownGLFWOnDestruct{false};
  bool mFullscreen{false};
  std::function<void (Vector2i)> mResizeCallback;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

}  // namespace _
