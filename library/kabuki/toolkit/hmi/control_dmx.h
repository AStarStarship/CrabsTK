/** Kabuki Toolkit
    @file    ~/source/hmi/dmx_control.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KABUKI_TOOLKIT_HMI_DMXCONTROL
#define HEADER_FOR_KABUKI_TOOLKIT_HMI_DMXCONTROL

#include "parameter.h"

namespace kt { namespace hmi {

/** A DMX Control.
 */
class KABUKI ControlDmx : public Parameter<int> {
    public:

    enum {
        NumChannels = 512   //< The number of DMX512 channels.
    };

    /** Default constructor. */
    ControlDmx (const char* label = "", int channel = 0, int value = 0, 
                int min_value = 0, int max_value = 255, int word_size = 8,
                int control_type = Parameter<int>::ControlDmx);

    /** Copy contructor. */
    ControlDmx (const ControlDmx& other);

    /** Virtual destructor. */
    ~ControlDmx () {}

    /** Triggers this DMX event to send out the target device. */
    virtual void Trigger () {}
    
    /** Prints this object to a string. */
    _::Printer& Print (_::Printer& print) const;
    
};      //< class ControlDmx
}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_HMI_DMXCONTROL
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
