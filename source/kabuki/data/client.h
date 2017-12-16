/** Kabuki Toolkit
    @file    ~/source/kabuki/data/data_client.cc
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

#ifndef HEADER_FOR_KABAUKI_DATA_CLIENT
#define HEADER_FOR_KABAUKI_DATA_CLIENT

#include "config.h"

namespace kabuki { namespace data {

/** Security guard handle datagram encryption.
*/
class Client : public _::Room {
    public:

    typedef enum States {
        kStateDisconnected       = 0,
        kStateAwaitingConnection = 1,
        kStateConnected          = 2
    } State;

    /** Default constructor. */
    Client ();

    /** Virtual destructor. */
    virtual ~Client ();

    /** Gets the FSM state. */
    int GetState ();

    /** Virtual function sets the FSM state to a positive value. */
    virtual bool SetState (int state);

    /** Prints this object to the console. */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end);

    private:
    
    int state_; // Client state.

};      //< class Client
}       //< namespace data
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABAUKI_DATA_CLIENT
