/** Kabuki Tek
    @version 0.9
    @file    /.../Source/_Tech/Portals/SPIPortal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#pragma once

#include "../Portal.h"

namespace _Tech { namespace Portals {

class SPIPortal : public Portal, public SPI
/** A type of Portal that allows for reading and writting to the local system. */
{
public:

    SPIPortal (int BufferSize);
    /*< Constructs a SPIPortal. */

    virtual ~SPIPortal ();
    /*< Destructs the terminal. */

    virtual const char* getError ();
    /*< gets the last error originating from this Portal. */

    virtual void feed ();
    //< Feeds (Tx) punch cards through the slot.

    virtual void pull ();
    //< Pulls (Rx) punch cards through the slot.

private:

    byte* buffer;       //< The byte buffer.

    int startIndex,     //< The starting index of the terminal.
        stopIndex,
        bufferSize;    //< The buffer size in bytes.
};

}   //< Portals
}   //< tek
