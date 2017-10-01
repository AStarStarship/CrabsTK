/** Kabuki Theater
    @file    /.../Source/_id/Date.h
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

#include <KabukiTheater-Config.h>

namespace _id {

/** An eventful date. 
    @todo Create a list of events. Is it an Event?
*/
class ID_API Date
{
    public:
    
    /** Default constructor. */
    Date (time_t time);

    /** Gets the date time. */
    time_t GetTime ();

    /** Sets the date to the Unix timestamp. */
    void SetTime (time_t t);
    
    /** Prints this object to the stdout. */
    void Print ();

    private:

    time_t time;        //< The data time.
};

}   //< _id
