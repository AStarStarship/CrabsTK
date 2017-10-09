/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/Source/ChineseRoom/room.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
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

#ifndef CHINESE_ROOM_ROOM_H
#define CHINESE_ROOM_ROOM_H

#include "door.h"
#include "exceptions.h"

namespace _ {

/** Returns an array of pointers to strings that describe the program states.
*/
KABUKI const char ** RoomStateStrings () {
    static const char * states[] = {
        "Initializing",
        "Waking up",
        "Running",
        "Going to sleep",
        "Exiting"
    };
    return states;
}

/** A list of Requests that can be sent from Terminal<uint_t, THash> to
    Terminal<uint_t, THash>.  */
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

/** Returns a pointer to an array of pointers to the _::Request strings. */
KABUKI const char ** RequestStrings ();

/** Gets the response string corresponding to the given request. */
KABUKI const char * RequestString (Request r);

/** A Chinese Room.
    An Chinese Room works the same way as in the Chinese Room thought 
    experiment. An Room receives a message through a slot in the door.
    The Room reads the message written in a Chinese, consults a collection of 
    objects, writes a response on a piece of paper, and passes it back through 
    the slot in the door.

    # Memory Layout
    @code
        ________________
        | Star stack
    ^   |_______________
    |   | Header  
    0x0 |_______________

    @endcode

    There are multiple doors in a Chinese Room that lead to other Chinese Rooms.
    Each door has multiple slots in lead to the same room. These slots are the 
    various IO ports of the system.
*/
class Room: public Operation {
    //NONCOPYABLE (Room)

    public:

    typedef enum {
        BootingState = 0,
        GoingToSleepState,
        ShuttingDownState,
    } State;

    /** Constructs a Room with the given size.
        @param size The room size that is bounded between the kMinRoomSize 
                    and kMaxRoomSize.
    */
    Room (uint_t size);

    /** Destructor. */
    virtual ~Room ();
    
    /** Processes a request from another Room.
        @return Returns false upon success and true if there is an error.
    */
    Request HandleNextRequest (Request r);

    /** Clears the log. */
    void ClearLog ();

    /** Processes all of the errors in the error log. */
    void ProcessLog ();

    /** Prints the error log to a terminal. */
    void PrintErrors (MonoidTx* tx);

    /** Function run every main loop cycle to check the system status. */
    virtual void DiagnoseProblems ();

    /** Sets up the Room. */
    virtual ticket_t Init ();

    /** Handler for shut down event. */
    virtual void ShutDown ();

    /** Handler go to sleep event. */
    virtual void Sleep ();

    /** Handler for wake from sleep event. */
    virtual void Wake ();

    /** Handler for recovering from an external crash message. */
    virtual void Crash ();

    /** Main program loop. */
    virtual ticket_t Loop ();

    /** Returns true if the Room is on. */
    virtual bool IsOn ();

    /** The default main function.
        Please feel free to override this with your own main function. */
    virtual int Main (const char ** args, int args_count);

    /** I2P operations. */
    virtual const Superset* Star (char_t index, Superset* io);

    protected:

    // 2-to-4-byte vtable pointer here in memory!
    uint address_;          //< The least significant bytes of the address.
    uint_t address_msb_,    //< The most significant bytes of the address.
        size_;              //< The size of the Room with device stack.
    byte stack_height_,     //< The number of devices on the device stack.
        stack_size_;        //< The max size of the device stack.
                            //< Star Control 1: this.
    Door* door_;            //< Star Control 2: The Door to this room.
    Star* xoff_,          //< Star Control 3: XOFF - XOFF handling device.
          * device_,        //< Star Control 4: the current device control.
          * devices_;       //< Pointer to the current device control.
};

}       //< namespace _
#endif  //< CHINESE_ROOM_ROOM_H
