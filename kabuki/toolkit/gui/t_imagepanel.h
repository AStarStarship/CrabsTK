/*
    /kabuki/toolkit/gui/imagepanel.h -- Image panel widget which shows a number of
    square-shaped icons

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
 * \class ImagePanel imagepanel.h /kabuki/toolkit/gui/imagepanel.h
 *
 * @brief Image panel widget which shows a number of square-shaped icons.
 */
class SDK ImagePanel : public Widget {
public:
  typedef std::vector<std::pair<int, std::string>> Images;
public:
  ImagePanel (Widget *parent)
    : Widget (parent), mThumbSize (64), mSpacing (10), mMargin (10),
    mMouseIndex (-1) {}

  void setImages (const Images &data) { mImages = data; }

  const Images& images () const { return mImages; }

  std::function<void (int)> callback () const { return mCallback; }

  void setCallback (const std::function<void (int)> &callback) { mCallback = callback; }

  virtual bool mouseMotionEvent (const Vector2i &p, const Vector2i &rel, int button, int modifiers) override {
    mMouseIndex = indexForPosition (p);
    return true;
  }

  virtual bool mouseButtonEvent (const Vector2i &p, int button, bool down, int modifiers) override {
    int index = indexForPosition (p);
    if (index >= 0 && mCallback && down)
      mCallback (index);
    return true;
  }

  virtual Vector2i preferredSize (NVGcontext *ctx) const override {
    Vector2i grid = gridSize ();
    return Vector2i (
      grid.x () * mThumbSize + (grid.x () - 1) * mSpacing + 2 * mMargin,
      grid.y () * mThumbSize + (grid.y () - 1) * mSpacing + 2 * mMargin
    );
  }

  virtual void draw (NVGcontext* ctx) override {
    Vector2i grid = gridSize ();

    for (size_t i = 0; i < mImages.size (); ++i) {
      Vector2i p = mPos + Vector2i::Constant (mMargin) +
        Vector2i ((int)i % grid.x (), (int)i / grid.x ()) * (mThumbSize + mSpacing);
      int imgw, imgh;

      nvgImageSize (ctx, mImages[i].first, &imgw, &imgh);
      float iw, ih, ix, iy;
      if (imgw < imgh) {
        iw = mThumbSize;
        ih = iw * (float)imgh / (float)imgw;
        ix = 0;
        iy = -(ih - mThumbSize) * 0.5f;
      } else {
        ih = mThumbSize;
        iw = ih * (float)imgw / (float)imgh;
        ix = -(iw - mThumbSize) * 0.5f;
        iy = 0;
      }

      NVGpaint imgPaint = nvgImagePattern (
        ctx, p.x () + ix, p.y () + iy, iw, ih, 0, mImages[i].first,
        mMouseIndex == (int)i ? 1.0 : 0.7);

      nvgBeginPath (ctx);
      nvgRoundedRect (ctx, p.x (), p.y (), mThumbSize, mThumbSize, 5);
      nvgFillPaint (ctx, imgPaint);
      nvgFill (ctx);

      NVGpaint shadowPaint =
        nvgBoxGradient (ctx, p.x () - 1, p.y (), mThumbSize + 2, mThumbSize + 2, 5, 3,
          nvgRGBA (0, 0, 0, 128), nvgRGBA (0, 0, 0, 0));
      nvgBeginPath (ctx);
      nvgRect (ctx, p.x () - 5, p.y () - 5, mThumbSize + 10, mThumbSize + 10);
      nvgRoundedRect (ctx, p.x (), p.y (), mThumbSize, mThumbSize, 6);
      nvgPathWinding (ctx, NVG_HOLE);
      nvgFillPaint (ctx, shadowPaint);
      nvgFill (ctx);

      nvgBeginPath (ctx);
      nvgRoundedRect (ctx, p.x () + 0.5f, p.y () + 0.5f, mThumbSize - 1, mThumbSize - 1, 4 - 0.5f);
      nvgStrokeWidth (ctx, 1.0f);
      nvgStrokeColor (ctx, nvgRGBA (255, 255, 255, 80));
      nvgStroke (ctx);
    }
  }
protected:
  Vector2i gridSize () const {
    int nCols = 1 + std::max (0,
      (int)((mSize.x () - 2 * mMargin - mThumbSize) /
      (float)(mThumbSize + mSpacing)));
    int nRows = ((int)mImages.size () + nCols - 1) / nCols;
    return Vector2i (nCols, nRows);
  }
  int indexForPosition (const Vector2i &p) const {
    Vector2f pp = ((p - mPos).cast<float> () - Vector2f::Constant (mMargin)) /
      (float)(mThumbSize + mSpacing);
    float iconRegion = mThumbSize / (float)(mThumbSize + mSpacing);
    bool overImage = pp.x () - std::floor (pp.x ()) < iconRegion &&
      pp.y () - std::floor (pp.y ()) < iconRegion;
    Vector2i gridPos = pp.cast<int> (), grid = gridSize ();
    overImage &= ((gridPos.array () >= 0).all () &&
      (gridPos.array () < grid.array ()).all ());
    return overImage ? (gridPos.x () + gridPos.y () * grid.x ()) : -1;
  }
protected:
  Images mImages;
  std::function<void (int)> mCallback;
  int mThumbSize;
  int mSpacing;
  int mMargin;
  int mMouseIndex;
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} //< namespace _
