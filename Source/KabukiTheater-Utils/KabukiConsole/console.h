/** Kabuki Theater
    @version 0.x
    @file    /.../Source/KabukiTheater-Utils/Console/Console.h
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

#ifndef KABUKI_CONSOLE_H
#define KABUKI_CONSOLE_H

#include "_/room.h"

namespace Kabuki {
  
/** An OS Console. 
	@todo Write me!
*/
template<typename T>
class Console: public Room<T>
{
    public:

    /** Constructs a Console. */
    Console ()
    :   Room (0) {
		
    }

    /** Destructor. */
    virtual ~Console () {

    }

    /** Sets up the Room. */
    ticket_t Init () override {
        return 0;
    }

    /** Main process loop. */
    ticket_t Loop () override {
        return 0;
    }

    /** ChineseRoom operations. */
    const Member<T>* Op (Rx<T>* rx, Tx<T>& tx, byte index) override {
        return 0;
    }

    private:

    // 2-to-4-byte vtable pointer here in memory!
	
};

}       //< namespace Kabuki
#endif  //< KABUKI_CONSOLE_H
