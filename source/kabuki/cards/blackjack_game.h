/** kabuki::cards
    @file    ~/source/kabuki/cards/hand.h
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

#ifndef HEADER_FOR_KABUKI_BLACKJACK_GAME
#define HEADER_FOR_KABUKI_BLACKJACK_GAME

#include "card_game.h"
#include "blackjack_dealer.h"

namespace kabuki { namespace cards {

/** A blackjack card game. */
class BlackjackGame : public CardGame {
    public:

    enum {
        kDefaultAnte        = 10, //< Default ante.
        kMinPlayers         = 2,  //< Min num players.
        kStateBooting       = 0,  //< State when player is waiting to join.
        kStatePlayingRound  = 1,  //< State when player is playing normally.
        kStateHolding       = 2,  //< State when player is holding.
        kStateOutOfGame     = 3   //< State when player has lost game.
    };

    static const int kDenominations[14];

    /** Constructor.
        The maximum number of players in Blackjack is set by the house. The more
        players there are, the greater the chance of the house loosing points, 
        so the maximum number we are going to set is 13. */
    BlackjackGame (int initNumPlayers = 2);

    /** Raises the ante 10 points. */
    bool RaiseAnteBy10 ();

    /** Starts the game. */
    virtual void StartNewGame ();

    /** Processes the beginning of round logic.
        Any player can hold, and sit out a round, but they still need to ante
        up to stay in the game. */
    virtual void BeginRound ();

    /** Processes the end of round logic. */
    virtual void EndRound ();

    /** Returns the text string for the start of each round. */
    virtual void PrintStats ();

    /** Game loop for card game. */
    virtual bool PlayGameInConsole ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    private:

    int             min_ante_,   //< Initial ante.
                    ante_,       //< Current ante.
                    points_pot_; //< Current number of points in the pot.
    BlackjackDealer dealer_;     //< Dealer.
    data::Array<BlackjackPlayer*> players_; //< Array of Player(s).

};      //< class BlackjackGame
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_BLACKJACK_GAME
