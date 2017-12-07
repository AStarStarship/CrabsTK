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

/** An abstract playing card game server.
    Most card games have pretty different rules*/
class Server : public _::Operation {
    public:
    
    enum {
        kDefaultConsoleWidth    = 80  //< Num rows in a DOS console.
        kDisconnected           = 0,
        kStateAwaitingConnection = 1,
    };

    /** Default constructor. */
    Server (id::UserList* users, const char* game_name, int num_players = 1,
            int min_players = 1, int max_players = 1);

    /** Constructor. */
    virtual ~Server ();

    /** Returns the name of the game. */
    const char* GetName ();

    /** Gets the FSM state. */
    int GetState ();

    /** Virtual function sets the FSM state to a positive value. */
    virtual bool SetState (int state);

    /** Getter and set tor for the stock (Deck) of cards. */
    Deck& GetPack ();

    /** The minimum number of players_.
        The round number can not be less than 1. */
    int GetRoundNumber ();

    /** The number of players_.
        min_players_ must be between the min and max number of players_. */
    int GetNumPlayers ();

    /** Sets the min number of players_ to the new value.
        @return Returns 0 upon success.
        @return Returns 1 if the value is greater the number of players_
        @return Returns -1 if the new value is less greater than the max
                value. */
    virtual int SetNumPlayers (int value);

    /** The minimum number of players_.
        min_players_ can not be greater than max_players_. */
    int GetMinPlayers ();

    /** Sets the min number of players_ to the new value.
        @return Returns 0 upon success and -1 if the new value is less greater
                than or equal the max value. */
    int SetMinPlayers (int value);

    /** The maximum number of players_. */
    int GetMaxPlayers ();

    /** Sets the max number of players_ to the new value.
        @return Returns 0 upon success and -1 if the new value is less than
                the min value. */
    int SetMaxPlayers (int value);

    /** Restart the game to a new state with a preset number of players_. */
    virtual void NewGame ();

    /** Game loop for card game. */
    virtual bool PlayGameInConsole() = 0;

    /** Processes the beginning of round logic. */
    virtual void BeginRound () = 0;

    /** Processes the end of round logic. */
    virtual void EndRound () = 0;

    /** Prints the players out to the console. */
    virtual void PrintPlayers ();

    /** Returns the text string for the start of each round. */
    virtual void PrintRoundStatsString ();

    /** Prints this game out to the console. */
    virtual void Print ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr) = 0;

    protected:

    const char  * name_;           //< Name of this game.
    int           state_,          //< Game state.
                  num_players_,    //< Num players.
                  min_players_,    //< Min number of players.
                  max_players_,    //< Max number of players.
                  round_number_;   //< Current round number.
    Deck          pack_;           //< Pack of cards.
    CardStack     stock_;          //< The stock of cards.
    Dealer      * dealer_;         //< Pointer to the Dealer.
    id::UserList* users_;          //< Global list of User(s).
    data::Array<Player*> players_; //< Array of players.

};      //< class Server

/** Returns the default play again or quit string. */
KABUKI const char* DefaultPlayAgainString ();

}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABAUKI_CARDS_CARDGAME
