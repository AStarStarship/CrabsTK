/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoom/Room.h
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

#ifndef CHINESEROOM_ROOM_H
#define CHINESEROOM_ROOM_H

#include "Error.h"
#include "Terminal.h"
#include "IDevice.h"

namespace _ 
{

struct HandShakeHeader
/*< The handshake header for two systems to connect. */
{
    byte unitSize;
};

/** A list of Requests that can be sent from Terminal to Terminal.  */
typedef enum {
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

inline const char** requestStrings()
/*< Returns a pointer to an array of pointers to the _::Request strings. */
{
    static const char* requestStrings[] = {
        "Open door",
        "Close door",
        "Invalid request"
    };

    return requestStrings;
}

inline const char* requestString(Request r)
/*< Gets the response string corresponding to the given request. */
{
    if (r < 0 || r >= InvalidRequest) return requestStrings ()[InvalidRequest];
    return requestStrings ()[r];
}

template<typename T>
class Room: public IDevice
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
{
    NONCOPYABLE (Room)

    public:

    typedef enum {
        BootingState = 0,
        GoingToSleepState,
        ShuttingDownState,
    } State;

    Room(uint_t size)
    /*< Constructs a Room. */
    :   address (0),
        addressMSB (0),
        size (size),
        stackHeight (0),
        stackSize (0),
        door (nullptr),
        xoff (nullptr),
        device (nullptr),
        devices (nullptr)
    {

    }

    virtual ~Room()
    /*< Destructor. */
    {
    }

    Request processNextRequest(Request r)
    /*< Processes a request from another Room.
        @return Returns false upon success and true if there is an error.
    */
    {
        return InvalidRequest;
    }

    void clearLog()
    /*< Clears the log. */
    {

    }

    void processLog()
    /*< Processes all of the errors in the error log. */
    {

    }

    const char* printErrors(Terminal& io)
    /*< Prints the error log to a terminal. */
    {
        //uint_t errorHeader[] = { 0 };
        //return io.prints(errorHeader);
        return 0;
    }
    
    virtual ticket_t diagnoseProblems()
    /*< Function run every main loop cycle to check the system status. */
    {
        /// Check for remote crash request.
        /// set stack pointer to stackPointer.
        return 0;
    }
    
    virtual ticket_t init()
    /*< Sets up the Room. */
    {
        return 0;
    }

    void* getStackPointer ()
    {
        #if PLATFORM == ARM32
        //  register word sp = asm("sp");
        asm volatile(
            "mov r14, sp"
            );
        #elif PLATFORM == ARM64
        //  register word sp = asm("sp");
        asm volatile(
            "mov r30, sp"
            );
        #elif PLATFORM == X86 || PLATFORM == X64
        asm volatile(
            "movl %eax, %esp"
        );
        #endif
    }

    virtual ticket_t loop()
    /*< Main program loop. */
    {
        //! Save stack pointer to crash the program if need be.


        //! Doors closed level.
        Request request = processNextRequest (request);

        while (request > 1)
        {
            request = processNextRequest (request);

        }

        //! By default we execute all of the ESC all at once, but they can be 
        //! split up by creating a sub-class or Room, and using execNext () 
        //! in you're main loop where you have free-time to run the operations.
        execAll ();

        return diagnoseProblems ();
    }

    ticket_t execNext ()
    /*< Executes one escape sequence from the Rx buffer. */
    {

        return 0;
    }

    ticket_t execAll ()
    /*< Executes all of the escape sequences from the slots in the Door(s). */
    {

        return 0;
    }

    const Member<T>* op(Terminal* io, byte index) override
    /*< ChineseRoom operations. */
    {
        /*
        static const char* bin = "bin",
        * boot  = "boot",
        * dev   = "dev",
        * etc   = "etc",
        * home  = "home",
        * lib   = "lib",
        * media = "media",
        * mnt   = "mnt",
        * opt   = "opt",
        * proc  = "proc",
        * sbin  = "sbin",
        * srv   = "srv",
        * tmp   = "tmp",
        * usr   = "usr",
        * var   = "var";

        if(find(Query)) index = Find(Query, &bin, 14);

        switch(index)
        {
        case 0: return Query ? ChineseRoom::equerry(Query, bin,
        "User binaries.");
        : return Select(A, VarObject);
        }

        return Query ? ChineseRoom::equerry(Query, "Room X");
        : ChineseRoom::InvalidIndex();
        */
        return 0;
    }

    protected:

    //! 2-to-4-byte vtable pointer here in memory!
    uint address;               //< The least significant bytes of the address.
    uint_t addressMSB,          //< The most significant bytes of the address.
        size;                   //< The size of the Room with device stack.
    byte stackHeight,           //< The number of devices on the device stack.
        stackSize;              //< The max size of the device stack.
                                //! Device Control 1: this.
    Door* door;                 //< Device Control 2: The Door to this room.
    IDevice* xoff,              //< Device Control 3: XOFF - The XOFF handling device.
        * device,               //< Device Control 4: the current device control.
        * devices;              //< Pointer to the current device control.
};

template<typename T>
void destroy (Room<T>* r)
/*< Destructs the given room. */
{
    if (r == nullptr) return;
    delete[] reinterpret_cast<byte*>(r);

}

}       //< namespace _
#endif  //< CHINESEROOM_ROOM_H
