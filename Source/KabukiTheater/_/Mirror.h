/** The Chinese Room
    @version 0.x
    @file    /.../Mirror.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

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

#ifndef CHINESEROOM_MIRROR_H
#define CHINESEROOM_MIRROR_H

#include "portal.h"
#include "print.h"
#include "door.h"

namespace _ {

/** A type of Portal that loops back the tx to rx.
*/
class Mirror: public Portal, public Device {
    public:

    /** Default constructor. */
    Mirror () {
        // Nothing to do here!
    }

    /** Creates a mirror from a pre-created . */
    Mirror* Init (uint_t buffer_size, byte stack_size, Device* a,
                  Door* origin) {
        io_.Init (buffer_size, stack_size, a, origin);
        return this;
    }

    /** Feeds tx messages through the io. */
    virtual void Feed () {
        while (io_.IsReadable ()) t.StreamRxByte (t.StreamTxByte ());
    }

    /** Pulls rx messages through the io. */
    virtual void Pull () {
        Feed ();
    }

    /** Inter-process operations. */
    const Member* Op (Rx* rx, Tx& tx, char index) override {
        return nullptr;
    }

    private:

    //NONCOPYABLE (Mirror)

    Device* device_;     //< The device in front of the mirror.
    Terminal io_; //< The Terminal for this Portal.
};

/** Creates a Mirror with the given buffer and stack size. */
inline Mirror* CreateMirror (uint_t buffer_size, uint_t stack_size) {
    try
    {
        byte* buffer = new byte[buffer_size];
        #if DEBUG_CHINESEROOM
        memset (buffer, '\0', buffer_size);
        #endif

        Mirror* m = new (buffer) Mirror ();

        // Initialize the memory.
        return m->Init (buffer_size, stack_size);
    } catch (...) {
        return nullptr;
    }
}

}       //< namespace _

#endif  //< CHINESEROOM_MIRROR_H
