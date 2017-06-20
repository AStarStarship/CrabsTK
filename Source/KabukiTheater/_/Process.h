/** The Chinese Room
     @version 0.x
     @file /.../Process.h
     @author Cale McCollough <cale.mccollough@gmail.com>
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

#ifndef CHINESEROOM_PROCESS_H
#define CHINESEROOM_PROCESS_H

#include "IDevice.h"

namespace _ {

inline const char** programStates()
/*< Returns an array of pointers to strings that describe the program states.*/
{
    static const char* states[] = {
        "Initializing",
        "Waking up",
        "Running",
        "Going to sleep",
        "Exiting"
    };
    return states;
}

class Process: public IDevice
/** A process in a ChineseRoom.
    All implementations will have at least one process for the Agent.
    If the OS supports multiple threads, then the process will be able to 
    communicant with eachother.
*/
{
    public:

    typedef enum {
        InitializingState = 0,    //< 0. Initializing program state.
        WakingUpState     = 1,    //< 1. Process waking up state.
        RunningState      = 2,    //< 2. Process running state.
        GoingToSleepState = 3,    //< 3. Process going to sleep state.
        ExitingState      = 4,    //< 4. Process exiting state.
    } State;

    Process()
    /*< Simple constructor. */
    {

    }

    virtual ~Process()
    /*< Destructor. */
    {

    }

    virtual ticket_t init (Terminal& io)
    /*< Initializes the program. */
    {
        return 0;
    }

    virtual ticket_t wakeUp(Terminal& io)
    /*< Wakes up the program from sleep. */
    {
        return 0;
    }

    virtual ticket_t crash()
    /*< Main program loop that might or might not do anything. */
    {
        return 0;
    }

    virtual ticket_t goToSleep(Terminal& io)
    /*< Puts the program to sleep. */
    {
        return 0;
    }

    virtual ticket_t exit(Terminal& io)
    /*< Exits the program. */
    {
        return 0;
    }

    virtual const Member<T>* op(Terminal* io, byte index)
    /*< Inter-process operations. */
    {
        return 0;
    }

    private:

    NONCOPYABLE(Process)

    //! 2-to-4-byte vtable pointer here in RAM!

    int reserved; //< Reserved for 64-bit memory aligment.
};

inline Process* program ()
/*< Gets the currently selected program. */
{
    return currentProcess;
}

inline ticket_t setProcess (Process* p)
/*< Attempts to set the current program to the given one. 
    @return Returns 0 upon success and an error ticket number upon failure.
*/
{
    if (p == nullptr) return errors ()->report (NullProcessError, 0, 0, 0);
}

} //< namespace _

#endif  //< CHINESEROOM_PROCESS_H
