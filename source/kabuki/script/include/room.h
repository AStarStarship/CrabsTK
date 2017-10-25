/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/room.h
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

#pragma once
#include <stdafx.h>

#ifndef KABUKI_SCRIPT_ROOM_H
#define KABUKI_SCRIPT_ROOM_H

#include "door.h"
#include "exceptions.h"

namespace _ {

/** A list of Requests that can be sent from Slot<uint_t, THash> to
    Slot<uint_t, THash>.  */
typedef enum Requests {
    OpenDoorRequest = 0,
    CloseDoorRequest,
    ConnectionRequest,
    DisconnectRequest,
    InvalidRequest, 
} Request;

enum
{
    NumRequests = 2,    //< The number of Requests.
};

/** Returns an array of pointers to strings that describe the program states.
*/
KABUKI const char** RoomStateStrings ();

/** Returns a pointer to an array of pointers to the _::Request strings. */
KABUKI const char** RequestStrings ();

/** Gets the response char corresponding to the given request. */
KABUKI const char* RequestString (Request r);

/** A Chinese Room.
    An Chinese Room works the same way as in the Chinese Room thought 
    experiment. An Room receives a message through a slot in the door, the man
    in the room reads does manual optical character recognition with a pen and
    paper stack, some filing cabinets, and a library of books.

    The size of the Expression Stack is defined by the Script Protocol to be a 
    a maximum of 2^15-1 expressions tall.

    # Memory Layout

    @code
         __________________ 
        |                  |
        |      Buffer      |
        |__________________|
        |        ^         |
        |        |         |
        | Expression stack |
    ^   |__________________|
    |   |    Room Header   |
    0x0 |__________________|

    @endcode

    There are multiple doors in a Chinese Room that lead to other Chinese Rooms.
    Each door has multiple slots in lead to the same room. These slots are the 
    various IO ports of the system.

    # Doors

    All doors in the Chinese Room are accessed through one of the four Data 
    Controllers DC1 through DC4.

    @code
    
    @endcode
*/
class Room: public Operand {
    //NONCOPYABLE (Room) //< Not sure why this isn't working?

    public:

    typedef enum {
        BootingState = 0,
        GoingToSleepState,
        ShuttingDownState,
    } State;

    /** Creates a Room with the given size.
        @param size The room size that is bounded between the kMinRoomSize 
                    and kMaxRoomSize. */
    Room (uint_t size);

    /** Destructor. */
    virtual ~Room ();
    
    /** Processes a request from another Room.
        @return Returns false upon success and true if there is an error. */
    Request HandleNextRequest (Request r);

    /** Clears the log. */
    void ClearLog ();

    /** Processes all of the errors in the error log. */
    void ProcessLog ();

    /** Prints the error log to a expression. */
    void PrintErrors (Bout* bout);

    /** Function run every main loop cycle to check the system status. */
    virtual void DiagnoseProblems ();

    /** Sets up the Room. */
    virtual const Operation* Init ();

    /** Handler for shut down event. */
    virtual void ShutDown ();

    /** Handler go to sleep event. */
    virtual void Sleep ();

    /** Handler for wake from sleep event. */
    virtual void Wake ();

    /** Handler for recovering from an external crash message. */
    virtual void Crash ();

    /** Main program loop. */
    virtual const Operation* Loop ();

    /** Returns true if the Room is on. */
    virtual bool IsOn ();

    virtual void* GetClockAddress () = 0;

    /** The default main function.
        Please feel free to override this with your own main function. */
    virtual int Main (const char** args, int args_count);

    /** Script expressions. */
    virtual const Operation* Star (int index, Expression* expr);

    protected:
                            //! vtable pointer here in memory!
    uint      address_;     //< The least significant bytes of the address.
    uint_t    address_msb_, //< The most significant bytes of the address.
              size_;        //< The size of the Room with device stack.
    byte      height_,      //< The number of devices on the device stack.
              stack_size_;  //< The max size of the device stack.
                            //< Star Control 1: this.
    Door    * door_;        //< Star Control 2: The Door to this room.
    Operand* xoff_,         //< Star Control 3: XOFF - XOFF handling device.
            * device_,      //< Star Control 4: the current device control.
            * devices_;     //< Pointer to the current device control.
};

}       //< namespace _
#endif  //< KABUKI_SCRIPT_ROOM_H
