/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/Event.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include <KabukiTheater-Config.hpp>

namespace _Theater { namespace HMI {

/* A event with a timestamp.
   An example of an Event is typically like a Control or menu funciton.
 */
class AEvent
{
    public:

    Event () : timestamp (0.0) {}
    //< Default constructor.
    
    virtual ~Event () {}
    //< Virtual destructor.

    timestamp_t timestamp () const { return timestamp; }
    //< gets the timestamp of the Event.

    virtual void trigger () = 0;
    //< Triggers the event.
    
    inline void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    timestamp_t timestamp;    //< The timestamp in milliseconds of the event.
};
}   //  _Theater/HMI

