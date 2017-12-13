/** Kabuki Toolkit
    @file    ~/kabuki/store/cards_server.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>


#ifndef HEADER_FOR_KABUKI_STORE_SERVER
#define HEADER_FOR_KABUKI_STORE_SERVER

#include "inventory.h"

namespace kabuki { namespace store {

class Server: public _::Room {
    public:

    typedef enum States {
        kStateShuttingDown = 0, //< Server State 0: Shutting down.
        kStateBooting      = 1, //< Server state 1: Booting.
        kStateServing      = 2, //< Server state 2: Serving.
    } State;

    /* Constructs the default server. */
    Server ();

    /** Gets the inventory. */
    Inventory& GetInventory ();

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer.
        @param text_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Do (const char* text, const char* text_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an
        error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    private:

    Inventory inventory_; //< Inventory root.

};      //< class Server
}       //< namespace store
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_STORE_SERVER
