/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/parameter_set.h
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
 
#ifndef HEADER_FOR_KABUKI_HMI_PARAMETERSET
#define HEADER_FOR_KABUKI_HMI_PARAMETERSET

#include "parameter.h"

namespace kabuki { namespace hmi {

/** A set of Parameter<T> that can be addressed by index.
*/
template<typename T>
class KABUKI ParameterSet {
    public:

    enum {
        MinNumElements = 2,         //< The minimum number of columns.
        MaxNumElements = 256,       //< The minimum number of columns.
    };

    /** Default constructor. */
    ParameterSet (size_type newNumControls = MinNumElements);

    /** Constructor creates a row with pointers to the newControls. */
    ParameterSet (const std::vector<Parameter<T>*>& controls);

    //< Copy constrcutor.
    ParameterSet (const ParameterSet& o);
    
    //< Destructor.
    ~ParameterSet ();

    //< Adds a new control to the array.
    void addParam (Parameter<T>& c);
    
    /** Adds a new control to the array.
        @return Returns 0 upon success, and index if it was out of bounds. */
    size_type setParam (size_type index , Parameter<T>& c);
    
    //< Deletes the Parameter<T> at the given index.
    size_type removeControl (size_type index);
    
    /** Gets the parameter at the given index.
        @return gets nullptr if the index if out of bounds. */
    Parameter<T>* getParam (size_type index);

    size_type getNumParams () const;
    //< gets the number of parameters in the set.
    
    void Print () const;
    /** Prints this object to a stdout. */

    private:

    std::vector<Parameter<T>> params;   //< An array of Parameter<T> pointers.
};      //< class ParameterSet
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_PARAMETERSET