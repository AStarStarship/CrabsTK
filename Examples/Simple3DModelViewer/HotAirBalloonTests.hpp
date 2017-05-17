/** Kabuki Theater
    @file    /.../KabukiTheater-Examples/Software3DWireFrameViewer/HotAirBalloonTests.hpp
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

namespace KabukiTheaterExamples {
    
class HotAirBalloon: public Shape
{
    public:
    
    float helliumLift = 0,
          helliumFlow = (float) .1;
    Text name;
    
    HotAirBalloon (Engine& driver, Point2D initPosition, const string& balloonName)
    {
        super (driver, initPosition, "Models/sphere2.plg");
        name = balloonName;
    }
    
    void increaseHelliumFlow ()
    {
        helliumFlow + =  (float) .01;
    }
    
    void decreaseHelliumFlow ()
    {
        helliumFlow + =  (float) .01;
    }
    
    void addHellium ()
    {
        helliumLift + =  helliumFlow;
    }
    
    void removeHellium ()
    {
        helliumLift - =  helliumFlow;
    }
    
    void setUpNextFrame (DeepSerinity3DnGen driver)
    {
        translateX (driver.windVector().x);        
        translateZ (driver.windVector().z);
        translateY (driver.windVector().z + helliumLift);
    }
    
    Text toText ()
    {
        return name + ": position = " + position().toText();
    }
}