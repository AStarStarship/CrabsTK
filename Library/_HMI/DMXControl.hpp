/** Kabuki Theater
    @file    /.../KabukiTheater/_HMI/DMXControl.hpp
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

#include <ChineseRoom.hpp>
#include <KabukiTheater-Config.hpp>
#include "AVControl.hpp"
#include <string>

namespace _HMI {

/** A DMX Control.
 */
class _KabukiTheater DMXControl : public AVControl
{
    public:

    static const int NumChannels = 512;             //< The number of DMX512 channels.

    DMXControl (const string& newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0,
        int newMaxValue = 255, int newWordSize = 8, int newAVControlType = AVControl::DMXControl);
    /** Default constructor. */

    DMXControl (const DMXControl& other);
    //< Copy contructor.

    ~DMXControl () {}
    //< Virtual destructor.

    virtual void trigger () override {}
    //< Triggers this DMX event to send out the target device.
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */
};

}   //  _HMI


