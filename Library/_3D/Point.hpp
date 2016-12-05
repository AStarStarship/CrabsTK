/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_3D/Point.hpp
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

#incude <KabukiSDK-Config.hpp>

namespace _3D {

class Point_f
/*< A 32-bit floating-point 3D point.
*/
{
    public:
    
    float x,
        y,
        z;
    
    Point_f (float initX, float initY, float initZ);
    /*< Translated the z position by the given delta. */

    void TranslateX (float deltaX);
    /*< Translated the x position by the given delta. */

    void TranslateY (float deltaY);
    /*< Translated the y position by the given delta. */

    void TranslateZ (float deltaZ);
    /*< Translated the z position by the given delta. */

    void print (Terminal io);
    /*< Prints this object to a Terminal. */
};

}   //< _3D
