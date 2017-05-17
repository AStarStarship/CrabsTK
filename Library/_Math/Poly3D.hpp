/** Kabuki Theater
    @file    /.../KabukiTheater/_Math/Poly3D.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include <KabukiTheater-Config.hpp>

#include "Point.hpp"

namespace _Math {

class Poly3D_f
/*<  */
{
    /**  */   
    Poly3D_f (int* indexOfPoints, _Math::Point_f* pointValues, int aNumPoints);
    
    /**  */  
    _Math::Point_f Vertex (int forThisPoint);
    
    /**  */  
    int VertexIndex (int forThisPoint);
    
    /**  */  
    _Math::Point_f getNormal ();
    
    /**  */  
    void computeNormal ();
    
    /**  */  
    double NormalMagnitude ();
    
    /**  */  
    void ScaleNormalMagnitude (double scaleFactor);

    /**  */  
    double getZAverage ();
    
    /**  */  
    void computeZAverge ();

    void print (Terminal io);
    /*< Prints this object to a Terminal. */
    
    private:

    enum {
        FlatShading = 0,        //< Just draws plain triangles.
        Textured
    };

    bool isTwoSided,
            isClipped,
            isActive,
            isVisible;

    _G::Color color,            //< The color of the polygon.
            shade;              //< The shade of the polygon (Need to fix texture mapping.).
       
    int shadingType,            //< The type of shadding: Flat, Textured.
        numVerticies;
    
    double  normalMagnitude,
            zAverage;  

    _Math::Point_f normal;

    _Math::Point_f[] vertexPoint;

    int* vertexIndex;
};

}   //< _Math
