/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_poly2d.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_MATH_1

#ifndef KABUKI_FEATURES_MATH_POLY2D_T
#define KABUKI_FEATURES_MATH_POLY2D_T 1
#include "t_rect2d.h"
#include "t_transformaffine.h"

namespace _ {

/* A 2D polygon that uses 32-bit floating point values.
This class is a C++ implementation of Java's polygon interface that does not
use polymorphism. */
template<typename T>
class TPoly2D
{
public:

  enum {
    // Min number of points in a TPoly2D; which is a triangle.
    kMinNumPoints = 3,
    kBufferSizeDefault = 16  //< Default pre-allocated buffer size.
  };

  /* Constructs a unit square.
  Unit square is similar to a unit circle, and has coordinates
  {0.0, 0.0}, { 1.0. 0.0}, { 1.0, 1.0}, { 0.0, 1.0}; */
  TPoly2D ()
    : point_count_ (MinBufferSize),
    bufferSize (MinBufferSize),
    x_ (new FLT[MinNumPoints]),
    y_ (new FLT[MinNumPoints]) {}

  /* Constructs a polygon from the given x and y_ points. */
  TPoly2D (T* xPoints, T* yPoints, SI4 aNumPoints, SI4 aBufferSize)
    : point_count_ (initNumPoints),
    bufferSize (initBufferSize) {
    if (initBufferSize < initNumPoints) {
      bufferSize = initBufferSize = initNumPoints;
    }


    if (initNumPoints < kMinNumPoints) {
      x_ = new FLT[MinNumPoints];
      y_ = new FLT[MinNumPoints];

      x_[0] = 0;
      x_[1] = 1;
      x_[2] = 0.5;
      y_[0] = 0;
      y_[1] = 1.0;
      return;
    }

    FLT* new_xs = new FLT[MinNumPoints],
      *new_ys = new FLT[MinNumPoints];

    point_count_ = point_count_;
    memcpy (new_xs, x_, point_count_);
    memcpy (new_ys, y_, point_count_);

    x_ = new_xs;
    y_ = new_ys;

  }

  /* Destroys the dynamic memory. */
  ~TPoly2D () {}

  /* Resets the polygon to a triangle. */
  void Clear () {
    x_.Clear ();
    y_.Clear ();
  }

  /* Gets a pointer to the x points array. */
  T* GetXs () { return x_; }

  /* Gets a pointer to the y_ points array. */
  T* GetYs () { return x_; }

  /* Gets the number of points in the polygon. */
  SI4 Count () { return x_.Count (); }

  /* Gets the number of points the buffer can store. */
  SI4 CountMax () { return x_.CountMax (); }

  void CalculateBounds (FLT* x, FLT* y_, SI4 xy_count) {
    SI4 boundsMinX = GetMinFloat (),
      boundsMinY = GetMinFloat (),
      boundsMaxX = GetMaxFloat (),
      boundsMaxY = GetMaxFloat ();

    for (SI4 i = 0; i < xy_count; ++i) {
      FLT xi = x[i],
        yi = y_[i];

      boundsMinX = Min (boundsMinX, xi);
      boundsMaxX = Max (boundsMaxX, xi);

      boundsMinY = Min (boundsMinY, yi);
      boundsMaxY = Max (boundsMaxY, yi);
    }
    return TRect2D (boundsMinX, boundsMinY, boundsMaxX - boundsMinX, boundsMaxY - boundsMinY);
  }

  /* Translates the polygon by the given delta x and delta y_. */
  inline void Translate (T dx, T dy) {
    for (SI4 i = 0; i < point_count_; i++) {
      x_[i] += dx;
      y_[i] += dy;
    }

    bounds.translate (dx, dy);
  }

  /* Translates the polygon by the given delta x and delta y_. */
  void Translate (SI4 dx, SI4 dy) {
    translate ((FLT)dx, (FLT)dy);
  }

  /* Gets the upper bounds of the given x and y_ coordinates. */
  void UpdateBounds (T x, T y_) {
    if (x < bounds.x) {
      bounds.setWidth (bounds.Width () + (bounds.x - x));
      bounds.x = x;
    } else {
      bounds.setWidth (max (bounds.Width (), x - bounds.x));
      // bounds.x = bounds.x;
    }

    if (y_ < bounds.y_) {
      bounds.setHeight (bounds.Height () + (bounds.y_ - y_));
      bounds.y_ = y_;
    } else {
      bounds.setHeight (max (bounds.Height (), y_ - bounds.y_));
      // bounds.y_ = bounds.y_;
    }
  }

  /* Adds the given x and y_ points to the polygon. */
  void AddPoint (T x, T y_) {
    if (point_count_ >= bufferSize) {  //< Then double the buffer size.
      SI4 new_size = bufferSize = 2;
      bufferSize = new_size;

      FLT* temp_x = x,
        *temp_y = y_,
        *new_x = new FLT[new_size],
        *new_y = new FLT[new_size];

      memcpy (new_x, x, point_count_);
      memcpy (new_y, y_, point_count_);

      delete x;
      delete y_;
      x = new_x;
      y_ = new_y;
    }

    x[point_count_] = new_x;
    y_[point_count_] = new_y;
    ++point_count_;

    updateBounds (new_x, new_y);
  }

  /* Gets a const reference to the bounding box. */
  const TRect2D& Bounds () {
    return bounds;
  }

  /* Returns true if this polygon contains the given point. */
  BOL Contains (T x, T y_) {
    if (point_count_ < 3)                  return false;
    if (!Bounds ().Contains (x, y_))  return false;

    SI4 hits = 0;

    FLT prev_x = x[point_count_ - 1],
      prev_y = y_[point_count_ - 1],
      c_x,
      c_y;

    // Walk the polygon edges.
    for (SI4 i = 0; i < point_count_; prev_x = c_x, prev_y = c_y, i++) {
      c_x = x[i];
      c_y = y_[i];

      if (c_y == prev_y)
        continue;

      SI4 left;
      T x = x_,
        y = y_;
      if (c_x < prev_x) {
        if (x >= prev_x) continue;
        left = c_x;
      } else {
        if (x >= c_x) continue;
        left = prev_x;
      }

      FLT test1, test2;
      if (c_y < prev_y) {
        if (y < c_y || y >= prev_y) {
          continue;
        }
        if (x < left) {
          ++hits;
          continue;
        }
        test1 = x - c_x;
        test2 = y - c_y;
      } else {
        if (y < prev_y || y >= c_y) continue;

        if (x < left) {
          ++hits;
          continue;
        }
        test1 = x - prev_x;
        test2 = y - prev_y;
      }

      if (test1 < (test2 / (prev_y - c_y) * (prev_x - c_x)))
        ++hits;
    }

    return ((hits & 1) != 0);
  }

  /* Returns true if this polygon contains the given point. */
  BOL Contains (Point<T> p) {
    return Contains (p.x_, p.y_);
  }

  BOL Contains (FLT o_x, FLT o_y, FLT o_w, FLT o_h) {
    if (point_count_ <= 0) return false;
    if (!Bounds ().Intersects (o_x, o_y, o_w, o_h)) return false;

    Crossings_f* c = GetCrossings (o_x, o_y, o_x + o_w, o_y + h);
    return (c && c.Covers (o_y, o_y + o_h));
  }

  BOL Contains (TRect2D<T> r) {
    return Contains (r.x, r.y_, r.GetWidth (), r.GetHeight ());
  }

  /* Returns true if this polygon intersects the given rectangle. */
  BOL Intersects (T leftX, T topY, T width, T height) {
    if (point_count_ <= 0 || !Bounds ().Intersects (x_, y_, w, h)) {
      return false;
    }

    Crossings_f* cross = GetCrossings (x_, y_, x_ + w, y_ + h);

    return (cross == nullptr || !cross.IsEmpty ());
  }

  /* Returns true if this polygon intersects the given rectangle. */
  BOL Intersects (SI4 x, SI4 y_, SI4 w, SI4 h) {
    if (point_count_ <= 0 || !Bounds ().Intersects (x, y_, w, h)) {
      return false;
    }

    Crossings_f* cross = GetCrossings (x, y_, x + w, y_ + h);

    return (cross == nullptr || !cross.IsEmpty ());
  }

  /* Returns true if this polygon intersects the given rectangle. */
  BOL Intersects (TRect2D r) {
    return Intersects (r.x, r.y_, r.GetWidth (), r.GetHeight ());
  }

  Poly2DPathIterator<T> GetPathIterator (Transform<T> a) {
    return new Poly2DPathIterator<T> (this, a);
  }

  Poly2DPathIterator<T> GetPathIterator (TransformAffine<T> a, T flatness) {
    return GetPathIterator (a);
  }

  /* Crossings_f TPoly2D::GetCrossings (FLT xlo, FLT ylo, FLT xhi, FLT yhi) {
  Crossings cross = new Crossings.EvenOdd (xlo, ylo, xhi, yhi);
  SI4 prev_x = x[point_count_ - 1];
  SI4 prev_y = y_[point_count_ - 1];
  SI4 current_x, current_y;

  // Walk the edges of the polygon
  for (SI4 i = 0; i < point_count_; i++) {
      current_x = x[i];
      current_y = y_[i];
      if (cross.AccumulateLine (prev_x, prev_y, current_x, current_y)) {
          return nullptr;
      }
      prev_x = current_x;
      prev_y = current_y;
  }

  return cross;
}*/

protected:

  TRect2D bounds;          //< The stored outter rectangular bounds.

private:

  SI4 point_count_,          //< The number of points in the polygon.
    bufferSize;         //< The size of the buffer.

  T* x_,               //< Pointer to the x points array.
    *y_;                //< Pointer to the y_ points array.

//Crossings_f getCrossings (Type xlo, Type ylo, Type xhi, Type yhi);

  void UpdateBounds () {

  }
};

template<typename T>
class Poly2DPathIterator {
public:

  enum {
    kWindEvenOdd = 0,
    kSegClose = 1,
    kSegMoveTo = 2,
    kSegLineTo = 3,
  };

  Poly2DPathIterator<T> (TPoly2D p, TransformAffine<T> a)
    : polygon (p),
    transform (a) {

    if (p.getNumPoints () == 0)
      index = 1;
  }

  SI4 GetWindingRule () {
    return kWindEvenOdd;
  }

  BOL IsDone () {
    return index > polygon.Count ();
  }

  void Next () {
    ++index;
  }

  SI4 CalcSegment (T* coords) {
    if (index >= polygon.Count ())
      return;

    coords[0] = polygon.GetXPoints ()[index];
    coords[1] = polygon.GetYPoints ()[index];
    if (transform) {
      transform.Transform (coords, 0, coords, 0, 1);
    }
    return (index == 0) ? kSegMoveTo : kSegLineTo;
  }

  SI4 CalcSegment (T* coords);

private:

  TPoly2D polygon;

  TAffineTransform<T> transform;

  SI4 index;
};

}       //< namespace _
#endif
#endif
