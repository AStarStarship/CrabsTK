/** The Chinese Room
    @version 0.x
    @file    ~/chinese_room/include/mirror.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_REFLECTION_H
#define CHINESE_ROOM_REFLECTION_H

#include "portal.h"
#include "monoid.h"

namespace _ {

class Reflection : public Portal {
    public:

    Reflection ();

    virtual void Prime ();

    virtual uint_t Length ();

    virtual void Feed (byte b);

    virtual byte Pull ();

    private:

    Monoid* io;     //< The async monoid.
};

ticket_t Read (Reflection* io, const uint_t* header, void** args);

ticket_t Write (Reflection* io, const uint_t* header, void** args);

}       //< namespace _
#endif  //< CHINESE_ROOM_REFLECTION_H
