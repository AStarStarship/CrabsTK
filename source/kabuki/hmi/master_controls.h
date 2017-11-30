/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/master_controls.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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
 
#ifndef HEADER_FOR_KABUKI_HMI_MASTERCONTROLS
#define HEADER_FOR_KABUKI_HMI_MASTERCONTROLS

#include "parameter.h"

namespace kabuki { namespace hmi {

/** Used for Quad Isymmetric control over a Device.
*/
class MasterControls {
    public:
    
    /** Constructs a blank set of master controls. */
    MasterControls ();
    
    /** Prints this object to a terminal. */
    inline void Print() const;
    
    private:

    Parameter<int>* a,
                  * b,
                  * c,
                  * d;
};

}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_MASTERCONTROLS
