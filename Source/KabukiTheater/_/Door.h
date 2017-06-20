/** The Chinese Room
    @version 0.x
    @file    /.../Door.h
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

#ifndef CHINESEROOM_DOOR_H
#define CHINESEROOM_DOOR_H

#include "Config.h"
#include "Terminal.h"
#include "IDevice.h"

namespace _ {

template<typename T>
class Door: public IDevice
/*< An door that connects two rooms, i.e. threads or systems.
    @see https://en.wikipedia.org/wiki/Chinese_room

    A door can lead to one or more rooms. Each room has a Room. A door can 
    be locked, in which case all of the Rooms behind this door are locked 
    out. When a door is locked, no messages are processed or sent to or
    from the door and its subsidiary agents.

    Rooms can be connected by one or more Portal(s). An example of a system
    with one Door and multiple Portals is a WiFi plus USB/Serial connection. 
    This is a commonly used configuration for debugging the system.
    
    Multiple doors also might share the same Portal. An example of this is a 
    connection to multiple systems over a WiFi connection.
*/
{
    public:
    
    enum {
        DefaultBufferSize = 255,    //< The default size of the door buffer.
    };

    Door(const char* roomName = "(:-)>", uint_t slotSize = DefaultBufferSize,
        bool isDynamic = false)
    /*< A door in a Chinese room. */
    :   isDynamic (isDynamic)
    {
        //tx.SetBuffer(adjacentDoor->Rx()->EndAddress() + 1), aSlotSize);
        //rx = new SerialScanner(tx.EndAddress(), aSlot, aSlotSize, aTalkbackSize);
    }

    ~Door ()
    /*< Destructor. */
    {
        if (isDynamic == 0) return;
        delete[] reinterpret_cast<byte*>(this);
    }

    Terminal* getTerminal(int index)
    /*< Gets the terminal at the given index. */
    {
        return 0;
    }

    void addTerminal(Terminal& t)
    /*< Address the given terminal to the Door. */
    {

    }

    Terminal* contains(byte* address)
    /*< Attempts to find a Terminal or Door with the given address. */
    {
        if (numSlots == 0) return nullptr;

        byte numberOfSlots = numSlots;

        Terminal* current = getTerminal (0),
            *end = getTerminal (numberOfSlots - 1);

        do
        {
            if (current->contains (address)) return current;
            current += sizeof (Terminal*);
        } while (current != end);
        return nullptr;
    }

    Terminal* getTerminal (void* address)
    /*< Gets the Terminal that contains the given address. 
        @return Returns null if this Door doesn't contain the given address. */
    {

    }

    virtual const Member<T>* op(Terminal* io, byte index)
    /*< ChineseRoom operations. */
    {
        if (index == '?')
        {
            static const Member<T> m0 = { "Door", setNumMembers<T> (0), 0, 0 };
            return &m0;
        }
        index -= ' ';
        if (index >= numSlots) return invalidMember ();
        return nullptr;
    }

    private:

    //! 2-to-4-byte vtable pointer here in memory!
    uint size;              //< The size of the entire door and all of it's slots.
    byte isDynamic,         //< Flag for this uses dynamic memory.
        doorNumber,         //< This door's number.
        maxNumDoors,        //< The max number of doors in the system.
        numDoors,           //< The number of doors this door is connected too.
        maxNumSlots,        //< The number of slots allocated in memory.
        numSlots,           //< The number of slots in the door.
        currentSlot,        //< Index of the current Slot being iterated through.
        currentDoor;        //< Index of the current Door being iterated through.
    Terminal** slots;       //< The slots in the door that messages pass through.
};

template<typename T>
inline Door<T>& Doors()
/*< Returns the main door of the room. */
{
    static Door<T> frontDoor;
    return frontDoor;
}

}       //< namespace _
#endif  //< CHINESEROOM_DOOR_H
