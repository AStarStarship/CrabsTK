/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /project/store/cserver.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#ifndef HEADER_FOR_KABUKI_STORE_SERVER
#define HEADER_FOR_KABUKI_STORE_SERVER

#include "inventory.h"

namespace _ {

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
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an
        error_t ticket upon Read-Write failure. */
    virtual const _::Op* Star (uint index, _::Expr* expr);

    private:

    Inventory inventory_; //< Inventory root.

};      //< class Server
}       //< namespace _
#endif  //< HEADER_FOR_KABUKI_STORE_SERVER
