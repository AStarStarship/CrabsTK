/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/DMXControl.hpp
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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>
#include "AVControl.hpp"

namespace _UI {

/** A DMX Control.
 */
class _KabukiSDK DMXControl : public AVControl
{
    public:

    static const int NumChannels = 512;             //< The number of DMX512 channels.

    /** Default constructor. */
    DMXControl (const char* &newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0,
        int newMaxValue = 255, int newWordSize = 8, int newAVControlType = AVControl::DMXControl);
    DMXControl (const DMXControl& other);           //< Copy contructor.
    ~DMXControl () {}                               //< Virtual destructor.

    virtual void trigger () override {}             //< Triggers this DMX event to send out the target device.
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
};

}   //< namespace _UI


