/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/ControlArray.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include "AVControl.hpp"
#include "MIDIControl.hpp"
#include "DMXControl.hpp"
#include "MIDIButton.hpp"
#include "DMXButton.hpp"
#include "MacroButton.hpp"

namespace _Theater { namespace HMI {

class _KabukiTheater_ ControlArray
/*< A row of AVControl(s).
*/
{
    public:

    enum {
        MinNumElements = 2,         //< The minimum number of columns.
        MaxNumElements = 256,       //< The minimum number of columns.
    };

    ControlArray (int newNumControls = MinNumElements);
    /*< Default constructor. */

    ControlArray (const vector<AVControl*>& controls);
    /*< Constructor creates a row with pointers to the newControls. */

    ControlArray (const ControlArray& o);
    //< Copy constrcutor.
    
    ~ControlArray ();
    //< Destructor.

    void addControl (AVControl* c);
    //< Adds a new control to the array.
    
    int setControl (int index , AVControl* c);
    //< Adds a new control to the array.
    
    AVControl* removeControl (int index);
    //< Deletes the AVControl at the given index.
    
    AVControl* getControl (int index);
    /*< Gets the control at the given index.
        @return gets nullptr if the index if out of bounds. */

    int getNumControls () const;
    //< gets the number of AVControl (s) in the row.
    
    inline void print(_::Log& log);
    /*< Prints this object to a terminal. */

    private:

    std::vector<AVControl*> controls;   //< And array of AVControl pointers.
};

}   //  _Theater/HMI
