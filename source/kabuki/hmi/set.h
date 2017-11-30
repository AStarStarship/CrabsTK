/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/set.h
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
 
#ifndef HEADER_FOR_KABUKI_HMI_SET
#define HEADER_FOR_KABUKI_HMI_SET

#include "project.h"

namespace kabuki { namespace hmi {

/**  */
class KABUKI Set {
    public:

    /** Constructor. */
    Set ();
    
    /** Copy constructor copies the other ojbect. */
    Set  (const Set& s);

    /** Destructor. */
    virtual ~Set ();

    /** Adds a new Project to the set. */
    void Add (Project& w);

    /** Gets the number of projects in the set. */
    int GetNumProjects ();

    /** Prints this object to the terminal. */
    void Print ();

    private:

    std::vector<Project&> projects;     //< Array of Project(s). 
};      //< class Set
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_SET
