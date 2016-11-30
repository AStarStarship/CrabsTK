/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/OSCControl.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>
#include "AVControl.hpp"

namespace _UI {

/** A Open SoundControl.
 */
class _KabukiSDK OSCControl : public AVControl
{
    public:

    /** Default constructor. */
    OSCControl  (const char* &newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0,
        int newMaxValue = 255, int newWordSize = 8);
    OSCControl  (const OSCControl& other);
    //< Copy contructor.

    virtual ~OSCControl () {}
    //< Virtual destructor.

    void Trigger () override;
    //< Triggers this DMX event to send out the target device.

    const char* HeaderString () const override;
    //< gets the header for toStringRow ().

    const char* ToStringRow () const override;
    //< gets a column of the values without the labels.
    
    virtual const char* op (I2P::Terminal& slot, int index, int enq);
    /*< Inter-process operations. */
};

}   //< namespace _UI

