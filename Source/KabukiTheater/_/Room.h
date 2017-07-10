/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoom/room.h
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

#ifndef CHINESEROOM_ROOM_H
#define CHINESEROOM_ROOM_H

#include "door.h"
#include "exceptions.h"

namespace _ {

/** Returns an array of pointers to strings that describe the program states.
*/
inline const char** RoomStateStrings () {
    static const char* states[] = {
        "Initializing",
        "Waking up",
        "Running",
        "Going to sleep",
        "Exiting"
    };
    return states;
}

/** The handshake header for two systems to connect. */
struct HandShakeHeader {
    byte unit_size;
};

/** A list of Requests that can be sent from Terminal<uint_t, THash> to Terminal<uint_t, THash>.  */
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
inline const char** RequestStrings () {
    static const char* RequestStrings[] = {
        "Open door",
        "Close door",
        "Invalid request"
    };

    return RequestStrings;
}

/** Gets the response string corresponding to the given request. */
inline const char* RequestString (Request r) {
    if (r < 0 || r >= InvalidRequest) return RequestStrings ()[InvalidRequest];
    return RequestStrings ()[r];
}

/** A Chinese Room.
    An Chinese Room works the same way as in the Chinese Room thought 
    experiment. An Room receives a message through a slot in the door.
    The Room reads the message written in a Chinese, consults a collection of 
    objects, writes a response on a piece of paper, and passes it back through 
    the slot in the door.

    # Memory Layout
    @code
        ________________
        | Device stack
    ^   |_______________
    |   | Header  
    0x0 |_______________

    @endcode

    There are multiple doors in a Chinese Room that lead to other Chinese Rooms.
    Each door has multiple slots in lead to the same room. These slots are the 
    various IO ports of the system.
*/
class Room: public Device {
    //NONCOPYABLE (Room)

    public:

    typedef enum {
        BootingState = 0,
        GoingToSleepState,
        ShuttingDownState,
    } State;

    /** Constructs a Room. */
    Room (uint_t size)
    :   address_ (0),
        address_msb_ (0),
        size_ (size),
        stack_height_ (0),
        stack_size_ (0),
        door_ (nullptr),
        xoff_ (nullptr),
        device_ (nullptr),
        devices_ (nullptr) {

    }

    /** Destructor. */
    virtual ~Room () {
    }
    
    /** Processes a request from another Room.
        @return Returns false upon success and true if there is an error.
    */
    Request HandleNextRequest (Request r) {
        return InvalidRequest;
    }

    /** Clears the log. */
    void ClearLog () {

    }

    /** Processes all of the errors in the error log. */
    void ProcessLog () {

    }

    /** Prints the error log to a terminal. */
    void PrintErrors (Tx& tx) {
        //uint_t errorHeader[] = { 0 };
        //return io.prints (errorHeader);
    }

    /** Function run every main loop cycle to check the system status. */
    virtual void DiagnoseProblems () {
        /// Check for remote crash request.
        throw RoomCrashException ();
    }

    /** Sets up the Room. */
    virtual ticket_t Init () {
        return 0;
    }

    /** Handler for shut down event. */
    virtual void ShutDown () {}

    /** Handler go to sleep event. */
    virtual void Sleep () {}

    /** Handler for wake from sleep event. */
    virtual void Wake () {}

    /** Handler for recovering from an external crash message. */
    virtual void Crash () {}

    /** Main program loop. */
    virtual ticket_t Loop () {}

    /** Returns true if the Room is on. */
    bool IsOn () {
        return true;
    }

    Request GetNextRequest (Request r) {
        return InvalidRequest;
    }

    /** The main function. */
    virtual void Main (const char** args, int args_count) {
        printf ("Launching Room with %i args:\n", args_count);
        for (int i = 0; i < args_count; ++i) {
            printf ("%s\n", args[i]);
        }
        printf ("Entering main loop.");

        while (IsOn ()) {
            try {
                Init ();
                ticket_t t;
                while (t = Loop ()){
                    // Doors closed level.
                    Request request = GetNextRequest (request);

                    while (request > 1) {
                        request = GetNextRequest (request);

                    }

                    // By default we execute all of the ESC all at once, but 
                    // they can be split up by creating a sub-class or Room, 
                    // and using execNext () in you're main loop where you 
                    // have free-time to run the operations.
                    ExecAll ();

                    DiagnoseProblems ();
                    return;
                }
                
            } catch (RoomCrashException e) {

            }
        }
    }

    /** Executes one escape sequence from the Rx buffer. */
    ticket_t ExecNext () {

        return 0;
    }

    /** Executes all of the escape sequences from the slots in the Door (s). */
    ticket_t ExecAll () {

        return 0;
    }

    /** I2P operations. */
    virtual const Member* Op (Rx* rx, Tx& tx, byte index) {
        return 0;
    }

    protected:

    // 2-to-4-byte vtable pointer here in memory!
    uint address_;          //< The least significant bytes of the address.
    uint_t address_msb_,    //< The most significant bytes of the address.
        size_;              //< The size of the Room with device stack.
    byte stack_height_,     //< The number of devices on the device stack.
        stack_size_;        //< The max size of the device stack.
                            // Device Control 1: this.
    Door* door_;            //< Device Control 2: The Door to this room.
    Device* xoff_,          //< Device Control 3: XOFF - XOFF handling device.
        * device_,          //< Device Control 4: the current device control.
        * devices_;         //< Pointer to the current device control.
};

}       //< namespace _
#endif  //< CHINESEROOM_ROOM_H
