/** Kabuki Tek
    @file    ~/library/kabuki/toolkit/app/comparable.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef KABUKI_TEK_UTILS_COMPARABLE_H
#define KABUKI_TEK_UTILS_COMPARABLE_H

#include "config.h"

namespace kabuki { namespace toolkit { namespace app {

/** Interface for a numerically comparable object. */
class Comparable
{
    public:

    /** Numerically compares this object to the other. */
    virtual int compare (Comparable* o) = 0;
};

}       //< namespace app
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_APP_COMPARABLE
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
