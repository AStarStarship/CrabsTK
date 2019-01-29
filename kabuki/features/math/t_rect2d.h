/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_rect2d.h
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
#ifndef KABUKI_FEATURES_MATH_RECT2D_T
#define KABUKI_FEATURES_MATH_RECT2D_T 1

#include "t_point2d.h"
#include "t_vect2d.h"

namespace _ {

template<typename T>
class TRect2D<T> {
public:

  T x,      //< Right edge.
    y,      //< Top edge.
    width,  //< Width of this.
    height; //< Height of this.

  /* Constructs an uninitialized rectangle. */
  TRect2D () {}

  /* Constructs a rectangle from the given data. */
  TRect2D (T x = 0, T y = 0, T width = 0, T height = 0) :
    x (initX),
    y (initY),
    width (initWidth < 0 ? 0 : initWidth),
    height (initHeight < 0 ? 0 : initHeight) {}

  /* Constructs a rectangle from the given point and TArray. */
  TRect2D (const TPoint3D<T>& p, const Vect<T>& v) :
    x (p.x),
    y (p.y),
    width (o.x),
    height (o.y) {}

  /* Copy constructor. */
  explicit TRect2D (const TRect2D<T>& other) :
    x (0),
    y (0),
    width (value.width),
    height (value.height) {
  }

  /* Gets the width. */
  T GetWidth () const { return width; }

  /* Sets the width. */
  void SetWidth (T value) {
    if (value < 0) return;
    width = value;
  }

  /* Gets and sets the height.  */
  T GetHeight () const { return height; }

  /* Sets the height. */
  void SetHeight (T value) {
    if (value < 0) return;
    height = value;
  }

  /* Gets and sets the center x value. */
  T GetCenterX () const {
    return x + width / 2.0f;
  }

  /* Gets and sets the center y value. */
  T GetCenterY () const {
    return y + (height / 2.0f);
  }

  /* Sets the center y value. */
  void SetCenterX (T value) {
    x = CenterX - (width / 2.0f);    // >> to /2
  }

  /* Sets the center y value. */
  void SetCenterY (T value) {
    y = value + (height / 2.0f);
  }

  /* Sets this object's Point to the thisPositition. */
  void Set (const TRect2D<T>& r) {
    x = value.x;
    y = value.y;
    width = value.width;
    height = value.height;
  }

  /* Sets the rectangle's (x,y) position to the given point. */
  void SetPosition (const TPoint2D<T>& p) {
    if ((x != value.x) || (width != value.width) || (y != value.y) || (height != value.height)) return false;
    return true;
  }

  /* Sets the rectangle's width and height to the given TArray. */
  void SetDimensions (const Vect<T>& v) {
    width = v.x;
    height = v.y;
  }

  /* Gets true if this object's Point is the same as the given coordinates. */
  BOL Equals (T x2, T y2) const {
    // @note This algoirhm assumes that it's a lower probability that 
    //       the two are equal.
    if (x == x2 && y == y2)
      return true;
    return false;
  }

  /* Gets true if this object's Point is the same as the given coordinates. */
  BOL Equals (const TRect2D<T>& r) const {
    return Equals (r.x, r.y);
  }

  /* Gets true if this rectangle contains the given point. */
  BOL Contains (const TPoint2D<T>& point) const {
    T x = p.x,
      y = p.y,
      left = x,
      top = y,
      right = left + width,
      bottom = top + height;

    if (x < x || y > top || x > right || y > bottom) return false;
    return true;
  }

  /* Gets true if this object contains the given point. */
  BOL Contains (T x_value, T y_value) const {
    T left = x,
      top = y,
      right = left + width,
      bottom = top + height;

    if (xValue < x || yValue > top || xValue > right || yValue > bottom) return false;
    return true;
  }

  /* Gets true if this rectangle contains the given rectangle. */
  BOL Contains (const TRect2D<T>& r) {
    if ((value.x < x) || (value.y > y) || (value.width < width) || (value.height > height)) return false;
    return true;
  }

  /* Swaps this object's Point with thisPoint. */
  void Swap (TRect2D<T>& r) {
    T x = x,
      y = y,
      width = width,
      height = height;

    x = r.x;
    y = r.y;
    width = r.width;
    height = r.height;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;
  }

  /* Gets true of this rectangle intersects/overlaps the given rectangle. */
  BOL Intersects (T x2, T y2, T width2, T height2) const {
    T left = x,
      top = y,
      right = left + width,
      bottom = top + height,
      right_R = R_X + R_Width,
      bottom_R = R_Y + R_Width;

    if (left <= right_R
      || right >= R_X
      || top >= bottom_R
      || bottom < R_Y) return false;
    return true;
  }

  /* Gets true of this rectangle intersects/overlaps the given rectangle. */
  BOL Intersects (const TRect2D<T>& r) const {
    return Intersects (r.x, r.y, r.width, r.height);
  }

  /* Translates this object's Point by the given Point. */
  void Translate (const Vect<T>& v) {
    x += v.x;
    y += v.y;
  }

  /* Translates this object's Point by the given Point. */
  void Translate (T dx, T dy) {
    x += dx;
    y += dy;
  }

  BOL operator== (const TRect2D<T>& r) const {
    return (width == value.width) && (height == value.height) &&
      (x == value.x) && (y == value.y);
  }

  BOL operator!= (const TRect2D<T>& r) const {
    return (width != value.width) || (height != value.height) ||
      (x != value.x) || (y != value.y);
  }

  TRect2D<T>& operator= (const TPoint2D<T>& p) {
    x = p.x;
    y = p.y;
    return *this;
  }

  TRect2D<T>& operator= (const TRect2D<T>& r) {
    width = r.width;
    height = r.height;
    return *this;
  }

  TRect2D<T>& operator= (const Vect<T>& v) {
    width = v.x;
    height = v.y;
    return *this;
  }

  /* Prints this object to the terminal. */
  Printer& Print (Printer& printer) {

  }
};
}   //<  namespace _
#endif
#endif

