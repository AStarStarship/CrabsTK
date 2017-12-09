/** kabuki::cards
    @file    ~/source/kabuki/cards/cards_server.cc
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

#ifndef HEADER_FOR_KABAUKI_CARDS_SERVER
#define HEADER_FOR_KABAUKI_CARDS_SERVER

#include "deck.h"
#include "dealer.h"

namespace kabuki { namespace cards {

/** An abstract playing card game server.
    Most card games have pretty different rules*/
class Server : public _::Room {
    public:

    typedef enum States {
        kStateAwaitingConnection = 0,
    } State;
    
    enum {
        kDefaultConsoleWidth = 80, //< Num rows in a DOS console.
    };

    /** Default constructor. */
    Server (uint port_number, const char* server_name);

    /** Constructor. */
    virtual ~Server ();

    /** Gets the name of the server. */
    const char* GetServerName ();

    /** Sets the name of the server. */
    const char* SetServerName (const char* name);

    /** Gets the FSM state. */
    int GetState ();

    /** Virtual function sets the FSM state to a positive value. */
    virtual bool SetState (int state);

    /** Prints this game out to the console. */
    virtual void Print ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr) = 0;

    protected:

    id::Authenticator* authenticator_; //< Name, Handle, & Password authenticator.
    const char   * server_name_;   //< Server name.
    uint32_t       port_number_;   //< Server port number.
    int            state_;         //< Server state.
    id::UserList   users_;         //< Global list of User(s).
    Array<Player*> players_; //< Array of players.

};      //< class Server

}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABAUKI_CARDS_SERVER
