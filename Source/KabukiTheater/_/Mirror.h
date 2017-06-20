/** The Chinese Room
    @version 0.x
    @file    /.../Mirror.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
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

#include "IPortal.h"
#include "Log.h"
#include "Print.h"

namespace _ {

inline const Member<T>* Door1 (Terminal* io, byte index)
{
    static const byte buffer[255];
    DEVICE (door, "Door 1", 0, "Door 1")

}

template<typename T>
class Mirror: public IPortal, public IDevice
/*< A type of IPortal that loops back the tx to rx.
*/
{
    public:

    Mirror ()
    /*< Default constructor. */
    {
        //! Nothing to do here!
    }

    Mirror* init (uint_t bufferSize, byte stackHeigth, IDevice* deviceA, IDevice* deviceB)
    /*< Creates a mirror from a pre-created . */
    {
        t.init (bufferSize, stackSize, rootDevice, door);
        return this;
    }

    virtual void feed()
    /*< Feeds tx messages through the io. */
    {
        while (t.isReadable ()) t.streamRxByte (t.streamTxByte ());
    }

    virtual void pull()
    /*< Pulls rx messages through the io. */
    {
        feed ();
    }

    void print(Log<T>& log)
    /*< Prints the Mirror to the debug stream. */
    {
        printLine ('_');
        printf ("| Terminal: %p", this);
        t.print ();
    }

    const Member<T>* op(Terminal* io, byte index) override
    /*< Inter-process operations. */
    {
        return nullptr;
    }

    private:

    NONCOPYABLE (Mirror)

    IDevice* device;    //< The device in front of the mirror.
    Terminal<T> t;      //< The Terminal for this Portal.
};

template<typename T>
inline Mirror<T>* createMirror(uint_t terminalSize, uint16_t stackSize)
/*< Creates a Terminal with the given buffer and stack size. */
{
    try
    {
        byte* buffer = new byte[mirrorSize];
        #if DEBUG_CHINESEROOM
        memset (buffer, '\0', mirrorSize);
        #endif
        //! Copy the v-table pointer.
        *reinterpret_cast<void**>(&buffer[0]) = *reinterpret_cast<void**>(&m);
        Mirror* m_ptr = new(buffer) Mirror ();

        //! Initialize the memory.
        return m_ptr->init (mirrorSize, stackSize);
    }
    catch (...)
    {
        return nullptr;
    }
}

template<typename T>
inline void destroy(Mirror<T>* m)
/*< Destroys the given Terminal. */
{
    if (m == nullptr) return;
    delete[] reinterpret_cast<byte*>(m);
}

}       //< namespace _

#endif  //< CHINESEROOM_MIRROR_H
