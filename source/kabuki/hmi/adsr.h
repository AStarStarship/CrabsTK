/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/adsr.h
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
 
#ifndef HEADER_FOR_KABUKI_HMI_ADSR
#define HEADER_FOR_KABUKI_HMI_ADSR

#include "config.h"

namespace kabuki { namespace hmi {

/** A ADSR filter. */
class KABUKI ADSR: public _::Operation {
    /** Constructs an ADSR with all zeroed out controls. */
    ADSR ();
    
    /** Script operations. */
    virtual const _::Operation* Op (uint index, _::Expression* expr);
};

}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_ADSR
