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
#include "remote_player.h"

namespace kabuki { namespace cards {

/** A playing card game client that can play many types of games in the console.
    The server does most of the game logic and feeds the client data on a 
    need to know basis. */
class Client : public id::ValidatorDefault, public _::Operation {
    public:

    typedef enum States {
        kStateDisconnected       = 0,
        kStateAwaitingConnection = 1,
        kStateConnected          = 2
    } State;
    
    enum {
        kMaxPlayers          = 64 * 1024, //< Man number of players.
        kDefaultConsoleWidth = 80,        //< Num rows in a DOS console.  
        kMaxMessageLength    = 140,       //< Max message length.
        kMaxGameNameLength   = 63,        //< Max game name length.
    };

    /** Default constructor. */
    Client ();

    /** Constructor. */
    virtual ~Client ();

    void DeleteRemotePlayers ();

    /** Gets the FSM state. */
    uint GetState ();

    /** Virtual function sets the FSM state to a positive value. */
    virtual bool SetState (int state);

    /** Prints out the RemotePlayer(s). */
    void Client::PrintPlayers ();

    /** Prints the round stats string. */
    void PrintRoundStatsString ();

    /** Prints this game out to the console. */
    virtual void Print ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    protected:
    
    char     game_name_[kMaxGameNameLength]; //< Current game name.
    uint     state_,        //< State of the app or current game index.
             round_number_; //< Current round number.
    id::User user_;         //< User.
    Deck     pack_;         //< Pack of Card(s).
    data::Array<RemotePlayer*> players_; //< Array of RemotePlayer(s).

};      //< class Client

/** Returns the default play again or quit string. */
KABUKI const char* DefaultPlayAgainString ();

}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABAUKI_CARDS_CARDGAME
