/** kabuki::cards
    @file    ~/source/kabuki/cards/cards_card_game.cc
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

#ifndef HEADER_FOR_KABAUKI_CARDS_CARDGAME
#define HEADER_FOR_KABAUKI_CARDS_CARDGAME

#include "deck.h"
#include "dealer.h"

namespace kabuki { namespace cards {

/** An abstract playing card game.
    Most card games have pretty different rules.
    
    
    */
class Client : public _::Operation {
    public:
    
    enum {
        kDefaultConsoleWidth    = 80, //< Num rows in a DOS console.
        kStateDisconnected      = 0,
        kStateAwaitingConnection = 1,
    };

    /** Default constructor. */
    Client (id::UserList* users, const char* game_name, int num_players = 1,
              int min_players = 1, int max_players = 1);

    /** Constructor. */
    virtual ~Client ();

    /** Returns the name of the game. */
    const char* GetName ();

    /** Gets the FSM state. */
    int GetState ();

    /** Virtual function sets the FSM state to a positive value. */
    virtual bool SetState (int state);

    /** Prints this game out to the console. */
    virtual void Print ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    protected:

    Dealer  * dealer_;             //< Pointer to the Dealer.
    id::User* user_;               //< Global list of User(s).
    data::Array<Player*> players_; //< Array of players.

};      //< class Client

/** Returns the default play again or quit string. */
KABUKI const char* DefaultPlayAgainString ();

}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABAUKI_CARDS_CARDGAME
