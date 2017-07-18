/** The Chinese Room
    @version 0.x
    @file    /.../mirror.h
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
#include "door.h"

namespace _ {

/** A type of Portal that .
*/
class Mirror: public Portal, public Device {
    public:

    /** Default constructor. */
    Mirror () {
        // Nothing to do here!
    }

    /** Creates a mirror from a pre-created . */
    Mirror* Init (uint_t buffer_size, byte stack_size, Device* a) {
        UniprinterInit (io_, buffer_size, stack_size, a);
        return this;
    }

    /** Feeds tx messages through the io. */
    virtual void Feed () {
        Rx* rx = GetRx (io_);
        while (IsReadable (rx)) StreamRxByte (io_, StreamTxByte (io_));
    }

    /** Pulls rx messages through the io. */
    virtual void Pull () {
        Feed ();
    }

    /** Inter-process operations. */
    const Member* Op (Rx* rx, Tx* tx, byte index) override {
        return nullptr;
    }

    private:

    //NONCOPYABLE (Mirror)

    Device* device_;    //< The device in front of the mirror.
    Uniprinter* io_;    //< The Terminal for this Portal.
};

}       //< namespace _

#endif  //< CHINESEROOM_MIRROR_H
