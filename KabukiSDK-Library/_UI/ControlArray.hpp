/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/ControlArray.hpp
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
#include "MIDIControl.hpp"
#include "DMXControl.hpp"
#include "MIDIButton.hpp"
#include "DMXButton.hpp"
#include "MacroButton.hpp"

namespace _Dev { namespace Controls {

/** A row of AVControl (s). */
class _KabukiSDK ControlArray
{
    public:

    enum {
        MinNumElements = 2,         //< The minimum number of columns.
        MaxNumElements = 256        //< The minimum number of columns.
    };

    ControlArray (int newNumControls = MinNumElements);
    /*< Default constructor. */

    ControlArray (const std::vector<AVControl*>& newControls);
    /*< Constructor creates a row with pointers to the newControls. */

    ControlArray (const ControlArray& other);
    //< Copy constrcutor.
    
    ~ControlArray ();
    //< Destructor.

    void addControl (AVControl* newControl);
    //< Adds a new control to the array.
    
    int setControl (int index , AVControl* newControl);
    //< Adds a new control to the array.
    
    AVControl* removeControl (int index);
    //< Deletes the AVControl at the given index.
    
    AVControl* getControl (int index);
    /*< Gets the control at the given index.
        @return gets nullptr if the index if out of bounds. */
    int getNumControls () const;
    //< gets the number of AVControl (s) in the row.
    
    void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    int numControls;            //< The number of controls in this row.

    AVControl** controls;       //< And array of AVControl pointers.
};
}   //< namespace _UI


