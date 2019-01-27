/** Kabuki Toolkit
    @file    ~/kt/math/poly3d.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software 
             distributed under the License is distributed on an "AS IS" BASIS, 
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#pragma once
#include <pch.h>

#include "config.h"

#include "point2d.h"

namespace _ {

/** A 3D polygon. */
template<typename T>
class Poly3D
{
    /**  */   
    Poly3D (SI4* indexOfPoints, Point3D<T>* pointValues, SI4 aNumPoints) :
        shading_type_ (FlatShading),
        is_two_sided_ (false),
        is_clipped_ (false),
        is_active_ (false),
        is_visible_ (true) {
        if (numPoints < 4) {
            // Create unit box with coordinates all 0.0 and 1.0.
        }
        vertex_index_ = indexOfPoints;
        vertexPoint = pointValues;
        vertex_count_ = vertex_index_.Length;
        base.data = this;
    }
    
    /** Returns the point at the given vertex. */  
    Point3D<float> Vertex (SI4 vertex_index) {
        return vertexPoint[vertex_index];
    }
    
    /**  */  
    SI4 VertexIndex (SI4 forThisPoint) {
        return vertex_index_[forThisPoint];
    }
    
    /**  */  
    Point3D<float> getNormal () {
        return normal;
    }
    
    /**  */  
    void UpdateNormal () {
        Point3D u (vertexPoint[0], vertexPoint[1]);
        v = new Point3D (vertexPoint[0], vertexPoint[2]);

        normal = v.crossProduct (u);

        normalMagnitude = normal.magnitude ();
    }
    
    /**  */  
    double NormalMagnitude () {
        return normalMagnitude;
    }
    
    /**  */  
    void ScaleNormalMagnitude (double scaleFactor) {
        normalMagnitude* = scaleFactor;
    }

    /**  */  
    double GetZAverage () {
        return zAverage;
    }
    
    /**  */  
    void ComputeZAverge () {
        if (vertex_index_.Length == 3)
            zAverage = (T)0.33333 * (vertexPoint[0].z + vertexPoint[2].z + vertexPoint[2].z);
        else // polygon is a quad
            zAverage = (T)0.25 * (vertexPoint[0].z + vertexPoint[2].z + vertexPoint[2].z + vertexPoint[3].z);
    }

    /*< Prints this object to a Expression. */
    _::Printer& Print (_::Printer& print) {
        print << "Poly3D:";
            //clour.prints (io);

        for (SI4 i = 0; i< vertex_count_; i++) {
            static const index_t[] header = { 4, STR, STR, STR, STR };
            print << vertexPoint[i].Print (print)
                  << "\n localVertex[" << vertex_index_[i] << "] ";
        }

        return print;
    }
    
    private:

    enum {
        FlatShading = 0,        //< Just draws plain triangles.
        Textured
    };

    BOL     is_two_sided_,
             is_clipped_,
             is_active_,
             is_visible_;
    _G::Color color_,  //< The color of the polygon.
                  shade;   /*< The shade of the polygon
                               @todo Need to fix texture mapping.). */
    SI4     shading_type_, //< The type of shadding: Flat, Textured.
            vertex_count_;
    double  normalMagnitude,
            zAverage;
    Point3D<float>   normal;
    Point3D<float>[] vertexPoint;
    SI4            * vertex_index_;
};

}       //< namespace _
