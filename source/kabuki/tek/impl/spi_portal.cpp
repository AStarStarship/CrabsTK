/** Kabuki Tek
    @version 0.9
    @file    /.../Source-Impl/Portals/SpiPortal.cpp
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

#include <tek/portals/spi_portal.h>

namespace tek {

SpiPortal::SpiPortal (int BufferSize)
{
    if (BufferSize < MinBufferSize)
    {
        buffer = new byte[MinBufferSize];
        return;
    }

    buffer = new byte[BufferSize];
}

SpiPortal::~SpiPortal ()
{
    if (buffer != 0) delete buffer;
}

const char* SpiPortal::GetError ()
{
    return 0;
}

void SpiPortal::Feed ()
{
    //while (IsWritable (io)) UniprinterStreamByte ();
}

void SpiPortal::Pull ()
{
	//Uniprinter* io;
    //while (UniprinterIsReadable (io)) UniprinterStream (io, slot.UniprinterStream (slot.getc ());
}

}   //< namespace tek
