/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_bezier2dquad.h
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
#ifndef KABUKI_FEATURES_MATH_BEZIER2DQUAD_T
#define KABUKI_FEATURES_MATH_BEZIER2DQUAD_T 1

#include "t_point2d.h"

namespace _ {

/* Class that represents a 4th order Bezier. */
template<typename T>
class Bezier2DQuad {
public:

  enum {
    point_count = 4     //< 4 points in a quad Bezier curve.
  };

  /* Constructor.
  @contract The length of XPoints and YPoints must equal NumPoints. */
  Bezier2DQuad (const T* x_points, const T* y_points) {
    x0 = x_points[0];
    x1 = x_points[1];
    x2 = x_points[2];
    x3 = x_points[3];
    y0 = x_points[0];
    y1 = x_points[1];
    y2 = x_points[2];
    y3 = x_points[3];
  }

  /* Constructs a bezier curve with the given points. */
  Bezier2DQuad (T x0, T y0, T x1, T y1, T x2, T y2,
    T x3, T y3) :
    x0 (x0),
    x1 (x1),
    x2 (x2),
    x3 (x3)
  {
  }

  /* Copy constructor initializes curve with the given points. */
  Bezier2DQuad (const Bezier2DQuad& o) :
    x0 (o.x0),
    y0 (o.y0),
    x1 (o.x1),
    y1 (o.y1),
    x2 (o.x2),
    y2 (o.y2),
    x3 (o.x3),
    y3 (o.y3)
  {
    /// Nothing to do here! :-)
  }

  /* Gets a pointer to the x points. */
  T* GetXPoints () { return &x0; }

  /* Gets a pointer to the y points. */
  T* GetYPoints () { return &y0; }

  /* Sets the x and y points to the given arrays. */
  void SetXPoints (const T* x_points) {
    x0 = x_points[0];
    x1 = x_points[1];
    x2 = x_points[2];
    x3 = x_points[3];
  }

  /* Sets the x and y points to the given arrays. */
  void SetYPoints (const T* y_points) {
    y0 = y_points[0];
    y1 = y_points[1];
    y2 = y_points[2];
    y3 = y_points[3];
  }

  /* Sets the x and y points to the given arrays. */
  void SetPoints (const T* x_points, const T* y_points) {
    x0 = x_points[0];
    x1 = x_points[1];
    x2 = x_points[2];
    x3 = x_points[3];

    y0 = y_points[0];
    y1 = y_points[1];
    y2 = y_points[2];
    y3 = y_points[3];
  }

  /* Sets the x and y values to the new ones. */
  void SetPoints (T new_x0, T new_y0, T new_x1, T new_y1, T new_x2, T new_y2,
                  T new_x3, T new_y3) {
    x0 = new_x0;
    y0 = new_y0;
    x1 = new_x1;
    y1 = new_y1;
    x2 = new_x2;
    y2 = new_y2;
    x3 = new_x3;
    y3 = new_y3;
  }

  /* Gets the number of points, which is always going to be 4. */
  SI4 Count () {
    return 4;
  }

  /* Gets a TPoint2D<T> along the path of the of curve at time t. */
  TPoint2D<T> Point (T t) {
    T x = (1 - t) * (1 - t) * x0 + 2 * (1 - t) * t * x1 + t * t * x2,
      y = (1 - t) * (1 - t) * y0 + 2 * (1 - t) * t * y1 + t * t * y2;

    return TPoint3D<T> (x, y);
  }

  /* Prints this object to the terminal. */

  Printer& Print (Printer& o) {
    o << "\nBezier2D:"
      << "\n0:{x:" << x0 << ",y:" << y0 << '}'
      << "\n1:{x:" << x1 << ",y:" << y1 << '}'
      << "\n2:{x:" << x2 << ",y:" << y2 << '}'
      << "\n3:{x:" << x3 << ",y:" << y3 << '}';
    return o;
  }

private:

  T x0, //< x0 coordinate.
    x1, //< x1 coordinate.
    x2, //< x2 coordinate.
    x3, //< x3 coordinate.
    y0, //< y0 coordinate.
    y1, //< y1 coordinate.
    y2, //< y2 coordinate.
    y3; //< y3 coordinate.
};

}       //< namespace _
