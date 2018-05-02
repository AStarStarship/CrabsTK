/** Kabuki Toolkit
    @file    ~/source/id/profile.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
 
#ifndef KABUKI_ID_PROFILE_H
#define KABUKI_ID_PROFILE_H


#include "config.h"
#include "entity.h"

namespace kabuki { namespace id {

/** An online profile of a person. */
class KABUKI Profile
{
    public:

    /** Constructor an anonymous Profile. */
    Profile ();

    /** Prints this object to the console. */
    _::Printer& Print (_::Printer& print);

    private:


};
}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_PROFILE_H