/** Kabuki Tek
    @file    /.../Source/tek/sensuators/flying_fader.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2017 [Cale McCollough](calemccollough.github.io)

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


#include <tek/config.h>

namespace tek { namespace Sensuators {

/** A bank of one or more groups of flying faders.
    Flying faders on most mixers have pages full of controls, and work similar 
    to the RotaryEncoder class.
*/
class FlyingFader {
    public:
    
    /** Constructs a blank flying fader. */
    FlyingFader ();
    
    /** Prints this object to a console. */
    void Print ();
    
    private:
};
}   //< namespace Sensuators
}   //< namespace tek

