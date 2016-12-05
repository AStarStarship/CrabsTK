/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/ShapesDrawingDemo/BallonTest.hpp
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

#include <KabukiSDK.hpp>

namespace KabukiSDKExamples {

class Ballon
{
    public:

    Ballon (int xValue, int yValue, int zValue, Color shade);

    void setX (int newX;

    void setY (int newY);

    void setZ (int newZ);

    int getZ ();

    void draw (Cell& C);

    void newPos ();

    private:

    int x, 
        y, 
        z, 
        value, 
        place, 
        ballonNumber;
        
    Color color;
   
};

}   //< KabukiSDKExamples
