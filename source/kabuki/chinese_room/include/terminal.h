/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/terminal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough<calemccollough.github.io>
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

#ifndef CHINESE_ROOM_TERMINAL_H
#define CHINESE_ROOM_TERMINAL_H

#include "verifier.h"

namespace _ {

/** A IO Terminal Device with Rx Uniprinter.
    
*/
class Terminal : public Device {
    public:

    /** Default constructor that does nothing. */
    Terminal (Verifier* io);

    ~Terminal ();

    bool Contains (void* address);

    const Member* Op (byte index, Verifier* io) override;

    private:

    Verifier* io_;        //< The all-in-one printer and scanner.
};

}       //< namespace _
#endif  //< CHINESE_ROOM_TERMINAL_H
