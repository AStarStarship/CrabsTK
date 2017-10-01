/** Kabuki Theater
    @file    ~/Source/_hmi/ParameterSet.h
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

#include "Parameter.h"

namespace _hmi {

template<typename T>
class _HMI_API ParameterSet
/*< A set of Parameter<T> that can be addressed by index.
*/
{
    public:

    enum {
        MinNumElements = 2,         //< The minimum number of columns.
        MaxNumElements = 256,       //< The minimum number of columns.
    };

    ParameterSet (size_type newNumControls = MinNumElements);
    /*< Default constructor. */

    ParameterSet (const std::vector<Parameter<T>*>& controls);
    /*< Constructor creates a row with pointers to the newControls. */

    ParameterSet (const ParameterSet& o);
    //< Copy constrcutor.
    
    ~ParameterSet ();
    //< Destructor.

    void addParam (Parameter<T>& c);
    //< Adds a new control to the array.
    
    size_type setParam (size_type index , Parameter<T>& c);
    /*< Adds a new control to the array.
        @return Returns 0 upon success, and index if it was out of bounds. */
    
    size_type removeControl (size_type index);
    //< Deletes the Parameter<T> at the given index.
    
    Parameter<T>* getParam (size_type index);
    /*< Gets the parameter at the given index.
        @return gets nullptr if the index if out of bounds. */

    size_type getNumParams () const;
    //< gets the number of parameters in the set.
    
    void print () const;
    /*< Prints this object to a stdout. */

    private:

    std::vector<Parameter<T>> params;   //< An array of Parameter<T> pointers.
};

}   //< _hmi
