/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/hand.h
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

#ifndef HEADER_FOR_KABUKI_ARCADE_BLACKJACK_GAME
#define HEADER_FOR_KABUKI_ARCADE_BLACKJACK_GAME

#include "blackjack_dealer.h"
#include "game.h"

namespace kabuki { namespace cards {

/** A blackjack card game. */
class BlackjackGame : public Game {
    public:

    typedef enum States {
        kStateObserving     = 0,  //< State when player has lost game.
        kStatePlayingRound  = 1,  //< State when player is playing normally.
        kStateHolding       = 2,  //< State when player is holding.
    } State;

    enum {
        kMinPlayers = 1,  //< Min players.
    };

    static const SI4 kDenominations[14];

    /** Constructor.
        The maximum number of players in Blackjack is set by the house. The more
        players there are, the greater the chance of the house loosing points, 
        so the maximum number we are going to set is 13. */
    BlackjackGame (id::UserList& users, id::User* user,
                   int64_t buy_in      = Dealer::kDefaultAnte,
                   int64_t ante        = Dealer::kDefaultAnte,
                   int64_t min_bet     = Dealer::kDefaultMinBet,
                   SI4     min_players = kMinPlayers,
                   SI4     max_players = Dealer::kDefaultMaxPlayer);

    /** Virtual destructor. */
    virtual ~BlackjackGame ();

    /** Restarts the game. */
    virtual void RestartGame ();

    /** Gets the round_number_. */
    SI4 GetRoundNumber ();

    /** Sets the round_number_. */
    BOL SetRoundNumber (SI4 value);

    /** Gets the pot_. */
    SI4 GetPot ();

    /** Sets the pot_. */
    BOL SetPot (SI4 value);

    /** Gets the dealer_. */
    BlackjackDealer* GetDealer ();

    /** Processes the beginning of round logic.
        Any player can hold, and sit out a round, but they still need to ante
        up to stay in the game. */
    virtual void BeginRound ();

    /** Processes the end of round logic. */
    virtual void EndRound ();

    /** Prints this object to the console */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const CH1* Sudo (const CH1* text, const CH1* strand_end);

    private:

    SI4              round_number_, //< Current round number.
                     pot_,          //< Current number of points in the pot.
                     num_ai_players;//< Num AI players.
    BlackjackDealer* dealer_;       //< Dealer.

};      //< class BlackjackGame
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_BLACKJACK_GAME
