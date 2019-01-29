/* Kabuki Toolkit @version 0.x_
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/t_poly3d.h
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
#ifndef KABUKI_FEATURES_MATH_POLY3D_T
#define KABUKI_FEATURES_MATH_POLY3D_T 1

#include "t_point2d.h"

namespace _ {

/* A 3D polygon. */
template<typename T>
class TPoly3D {
  /*  */
  TPoly3D (SI4* vertex_index, TPoint3D<T>* points, SI4 point_count)
    :
    shading_type_ (kFlatShading),
    is_two_sided_ (false),
    is_clipped_ (false),
    is_active_ (false),
    is_visible_ (true),
    vertex_index_ (vertex_index) {
    if (point_count < 4) {
      // Create unit box with coordinates all 0.0 and 1.0.
    }
    center_ = points;
    vertex_count_ = vertex_index_.Count ();
  }

  /* Returns the point at the given vertex. */
  TPoint3D<FLT> Vertex (SI4 vertex_index) {
    return center_[vertex_index];
  }

  SI4 VertexIndex (SI4 forThisPoint) {
    return vertex_index_[forThisPoint];
  }

  /* Gets the normal */
  TPoint3D<FLT> Normal () {
    return normal_;
  }

  /* Reclaculates the normal vector. */
  void UpdateNormal () {
    TPoint3D u (center_[0], center_[1]),
             v (center_[0], center_[2]);
    normal_ = v.CrossProduct (u);
    normal_mag_ = normal_.Magnitude ();
  }

  /*  */
  T NormalMagnitude () {
    return normal_mag_;
  }

  /*  */
  void ScaleNormalMagnitude (T scaleFactor) {
    normal_mag_ *= scaleFactor;
  }

  /* Gets the Z-Average of this polygon. */
  T ZAverage () {
    return z_avg_;
  }

  /* Computes the Z-Average of this polygon. */
  void ZAvergeCompute () {
    if (vertex_index_.Length == 3)
      z_avg_ = (T)0.33333 * (center_[0].z + center_[2].z + 
                center_[2].z);
    else // polygon is a quad
      z_avg_ = 0.25 * (center_[0].z + center_[2].z + 
                center_[2].z + center_[3].z);
  }

  /*< Prints this object to a Printer. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nPoly3D:";
    for (SI4 i = 0; i < vertex_count_; i++) {
      o << center_[i].Print (print)
        << "\n localVertex[" << vertex_index_[i] << "] ";
    }
    return o;
  }

private:

  enum {
    kFlatShading = 0,        //< Just draws plain triangles.
    kTextured
  };

  BOL is_two_sided_,       //< Flag for if this is two-sided.
    is_clipped_,           //< Flag for if this is clipped.
    is_active_,            //< Flag for if this is active.
    is_visible_;           //< Flag for if this is visible.
  SI4 shading_type_,       //< Type of shadding: kFlat, kTextured.
    vertex_count_;         //< Number of vertexes in this.
  Color color_,            //< Polygon color.
    shade_;                //< Polygon shade.
  T  normal_mag_,          //< Magnitude of the normal vector.
    z_avg_;                //< Z-plane average.
  TVect3D<FLT> normal_;    //< Normal vector.
  TPoint3D<FLT>[] center_; //< Center of this.
  SI4* vertex_index_;      //< Vertex index.
};

}       //< namespace _
#endif
#endif

