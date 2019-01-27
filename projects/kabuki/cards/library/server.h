/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/cards_server.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "blackjack_game.h"

namespace kabuki { namespace cards {

/** An abstract playing card game server.
    
    # Session System

    When players are created, then are given a unique id (uid). Players will 
    join and leave the server, thus their index in the array of users will
    more than likely change. When a user logs on they are given a session,
    public_key, and a private_key:
    |       Tuple | Description                                          |
    |------------:|:----------------------------------------------------:|
    |     session | The index in the UserList array                      |
    |  public_key | A randomly generated publicly visible session key.   |
    | private_key | A randomly generated private encryption session key. |

    To preserve server state, then next uid must be saved. This is currently 
    done by writing the number to and from a disk, but this is primitive
    and will change ASAP.

    A user will log into the server and a User object will be created in the
    users_ UserList.

*/
class Server : public _::Room {
    public:

    typedef enum States {
        kStateShuttingDown= 0, //< Server State 0: ShuttingDown.
        kStateInitializing,    //< Server State 1: Initializing. 
        kStateServingClients,  //< Server State 2: Serving clients. 
    } State;
    
    enum {
        kDefaultPort         = 2048,    //< Default TCP port number.
        kDefaultMaxUsers     = 1024,    //< Default max users.
        kDefaultConsoleWidth = 80,      //< Default num rows in a DOS console.
        kMaxTextLength       = 2 * 1024,//< Max string length.
        kDefaultMaxGames     = 256,     //< Default max games.
    };

    /** Default constructor. */
    Server (const CH1* password  = id::Password::kDefault,
            uint32_t    port      = kDefaultPort,
            SI4         max_games = kDefaultMaxGames);

    /** Constructor. */
    virtual ~Server ();

    /** Quits the server. */
    const CH1* Exit ();

    /** Restarts the server. */
    const CH1* Restart ();

    /** Gets the FSM state. */
    SI4 GetState ();

    /** Virtual function sets the FSM state to a positive value. */
    State SetState (State state);

    /** Gets the FSM state. */
    uint32_t GetPort ();

    /** Virtual function sets the FSM state to a positive value. */
    virtual BOL SetPort (uint32_t port_number);

    /** Gets the users_. */
    id::UserList& Users ();

    /** Gets the number of game tables on the server. */
    SI4 GetNumTables ();

    /** Adds a new Game to the games_. */
    SI4 AddGame (Game* game);

    /** Adds a new BlackjackGame.
        @return The index of the new game SI4 he games_. */
    SI4 AddBlackjackGame ();

    /** Removes the game at the given index. */
    BOL RemoveGame (SI4 index);

    /** Gets the directions string. */
    const CH1* GetDirections ();

    /** Sets the directions string. */
    BOL SetDirections (const CH1* directions);

    /** Registers a new agent with the given handle prefix and User uid. */
    virtual SI4 AddAgent (const CH1* handle_prefix,
                          double balance = 1 * 1000.00 * 1000.00,
                          uint64_t value = 1 * 1000 * 1000);

    /** Prints this game out to the console. */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const CH1* Sudo (const CH1* text,
                                    const CH1* strand_end);

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    protected:

    id::AuthenticatorDefault authenticator_;//< Handle & Password authenticator.
    id::Password             password_;     //< Sever root-level password.
    SI4                      state_;        //< Server state.
    uint32_t                 port_;         //< Server port number.
    id::UserList             users_;        //< Global list of User(s).
    TArray<Game*>       games_;        //< Array of Game.
    CH1                   * directions_;   //< Console input directions.
    store::Client            store_;        //< Store client.

};      //< class Server
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABAUKI_CARDS_SERVER
