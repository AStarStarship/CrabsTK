/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/terminal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#ifndef CHINESE_ROOM_TERMINAL_H
#define CHINESE_ROOM_TERMINAL_H

#include "monoid.h"
#include "operation.h"

namespace _ {

/** An async monoid Terminal.
    
*/
class Terminal : public Operation {
    public:

    /** Default constructor. */
    Terminal (MonoidTx* io);

    /** Destructs the Terminal. */
    ~Terminal ();

    /** Returns true if the set contains the given address pointer. */
    bool Contains (void* address);

    /** A*MonoidRx Operations. */
    const Set* Star (char_t index, Automata* b) override;

    private:

    MonoidTx* io_;        //< Async mondoid ring buffer socket.
};

}       //< namespace _
#endif  //< CHINESE_ROOM_TERMINAL_H
