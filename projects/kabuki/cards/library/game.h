/* Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/arcade_card_game.cc
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

#ifndef KABAUKI_ARCADE_CARDS_CARDGAME
#define KABAUKI_ARCADE_CARDS_CARDGAME

#include "deck.h"
#include "dealer.h"
#include "player_proxy.h"

namespace kabuki { namespace cards {

/* A playing card game client that can play many types of games in the console.
    The server does most of the game logic and feeds the client data on a 
    need to know basis. */
class Game : public _::Operation {
    public:

    typedef enum States {
        kStateDisconnected       = 0,
        kStateAwaitingConnection = 1,
        kStateConnected          = 2
    } State;

    enum {
        kDefaultMinPlayers = 1,  //< Default min players.
    };
    
    enum {
        kMaxPlayers          = 64 * 1024, //< Man number of players.
        kDefaultConsoleWidth = 80,        //< Num rows in a DOS console.  
        kMaxMessageLength    = 140,       //< Max message length.
        kMaxGameNameLength   = 63,        //< Max game name length.
    };

    /* Default constructor. */
    Game (id::UserList& users, const CH1* game_name, SI4 min_players,
              SI4 max_players);

    /* Constructor. */
    virtual ~Game ();

    /* Gets the game_name_. */
    const CH1* GetName ();

    /* Gets the FSM state. */
    int32_t GetState ();

    /* Virtual function sets the FSM state to a positive value. */
    virtual BOL SetState (SI4 state);

    /* The number of players_.
    min_players_ must be between the min and max number of players_. */
    SI4 GetNumPlayers ();

    /* The minimum number of players_.
    min_players_ can not be greater than max_players_. */
    SI4 GetMinPlayers ();

    /* The maximum number of players_. */
    SI4 GetMaxPlayers ();

    /* Restart the game to a new state with a preset number of players_. */
    virtual void RestartGame () = 0;

    /* Processes the beginning of round logic. */
    virtual void BeginRound () = 0;

    /* Processes the end of round logic. */
    virtual void EndRound () = 0;

    /* Prints the observers to the console. */
    void PrintObservers ();

    /* Attempts to add an observer user to the game.
        @return Returns -1 upon failure and the number of observers upon
                success. */
    virtual SI4 Join (id::User* user);

    /* Attempts to add an observer user to the game.
        @return Returns -1 upon failure and the number of observers upon
                success. */
    virtual SI4 Leave (id::User* user);

    /* Gets a reference to the observers_. */
    TArray<id::User*>& GetObservers ();

    /* Prints this game out to the console. */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /* Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    /* Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const CH1* Sudo (const CH1* text,
                            const CH1* strand_end) = 0;
    protected:

    const CH1           * name_;  //< Game name.
    int32_t                state_,      //< Game state.
                           min_players_;//< Min players.
    id::UserList         & users_;      //< Server UserList.
    TArray<id::User*> observers_;  //< Array of Player.

};      //< class Game

/* Returns the default play again or quit string. */
KABUKI const CH1* DefaultPlayAgainText ();

}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABAUKI_ARCADE_CARDS_CARDGAME
