/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /project/graphics_demo/00_foo.h
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

#if SEAM == GRAPHICS_DEMO_0
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

namespace projects {
namespace graphics_Demo {

class HotAirBalloon : public Shape {
public:

  FLT helliumLift = 0,
    helliumFlow = (FLT) .1;
  Text name;

  HotAirBalloon (Kabuki3D& driver, Point2D initPosition, 
    const CH1* balloonName)
    : Shape (driver, initPosition, "models/sphere2.plg") {
    name = balloonName;
  }

  void increaseHelliumFlow () {
    helliumFlow + = (FLT) .01;
  }

  void decreaseHelliumFlow () {
    helliumFlow + = (FLT) .01;
  }

  void addHellium () {
    helliumLift + = helliumFlow;
  }

  void removeHellium () {
    helliumLift - = helliumFlow;
  }

  void SetUpNextFrame (Kabuki3D driver) {
    translateX (driver.windVector ().x);
    translateZ (driver.windVector ().z);
    translateY (driver.windVector ().z + helliumLift);
  }

  Text toText ()
  {
    return name + ": position = " + position ().toText ();
  }
}
} //< namespace graphics_demo
} //< namespace project
