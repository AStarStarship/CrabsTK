/** Kabuki Theater
    @file    /.../Source/_HMI/ParameterSet.hpp
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

#include "Parameter.hpp"

namespace _HMI {

class _KabukiTheater_ ParameterSet
/*< A row of Parameter<int>(s).
*/
{
    public:

    enum {
        MinNumElements = 2,         //< The minimum number of columns.
        MaxNumElements = 256,       //< The minimum number of columns.
    };

    ParameterSet (int newNumControls = MinNumElements);
    /*< Default constructor. */

    ParameterSet (const std::vector<Parameter<int>*>& controls);
    /*< Constructor creates a row with pointers to the newControls. */

    ParameterSet (const ParameterSet& o);
    //< Copy constrcutor.
    
    ~ParameterSet ();
    //< Destructor.

    int getCount ();
    /*< Gets the number of controls in the array. */

    void addControl (Parameter<int>* c);
    //< Adds a new control to the array.
    
    int setControl (int index , Parameter<int>* c);
    //< Adds a new control to the array.
    
    Parameter<int>* removeControl (int index);
    //< Deletes the Parameter<int> at the given index.
    
    Parameter<int>* getControl (int index);
    /*< Gets the control at the given index.
        @return gets nullptr if the index if out of bounds. */

    int getNumControls () const;
    //< gets the number of Parameter<int> (s) in the row.
    
    void print () const;
    /*< Prints this object to a stdout. */

    private:

    std::vector<Parameter<int>*> controls;   //< An array of Parameter<int> pointers.
};

}   //< _HMI
