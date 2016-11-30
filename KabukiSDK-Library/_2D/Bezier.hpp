/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_2D/Bezier.hpp
    @author     Cale McCollough
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

#include "Point.hpp"

#include <vector>
#include <string>
#include <memory>

namespace _2D {

class _KabukiSDK Bezier_f
/*< Class that represents a fourth order Bezier. */
{
    public:

    enum { 
        MinmumNumPoints = 3     //< The minimum number of points reqruied for a bezier curve.
    };

    Bezier_f  (float* theXPoints, float* theYPoints, int theNumPoints);
    /*< Constructor.
    @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */

    Bezier_f  (const float* theXPoints, const float* theYPoints, int theNumPoints);
    /*< Constructor.
    @pre The length of XPoints and YPoints must equal NumPoints or curve will not be created correctly. */

    Bezier_f  (float X0, float Y0, float X1, float Y1, float X2, float Y2, float X3, float Y3);
    /*< Constructs a bezier curve with the given points. */

    Bezier_f  (const Bezier_f& O);
    /*< Copy constructor initializes curve with the given points. */

    ~Bezier_f ();
    /*< Destructs dynamic memory if used. */

    Point_f getPoint  (float P);
    /*< Gets a Point_f along the path of the of curve at time t. */

    int getNumPoints ();
    /*< Gets the number of points. */

    void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */

    private:

    float* xPoints,                 //< Array of X points.
        * yPoints;                  //< Array of Y points.

    int numPoints;                  //< The number of points in the polygon.
    bool isDynamic;                 //< Flag for if this uses dynamic memory.
};

}   //< namespace _2D
