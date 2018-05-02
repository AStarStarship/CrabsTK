/** kabuki::pro
    @file    ~/source/id/imp/date.cc
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

#include <stdafx.h>
#include "../date.h"

namespace kabuki { namespace id {

Date::Date (time_t aTime) :
    time (aTime) {

}

time_t Date::GetTime () {
    return time;
}

void Date::SetTime (time_t t) {
    time = t;
}

void Date::Print (_::Log& log) {

}

}       //< id
}       //< namespace toolkit}       //< namespace kabuki
