/** kabuki::cards
    @file       ~/source/kabuki/cards/player.h
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

#ifndef HEADER_FOR_KABUKI_BLACKJACK_PLAYER
#define HEADER_FOR_KABUKI_BLACKJACK_PLAYER

#include "dealer.h"

namespace kabuki { namespace cards {

/** A player in a Blackjack game. */
class BlackjackPlayer : public Player {
    public:

    enum States {
        kWaitingState = 0,
        kHittingState = 1,
        kHoldingState = 2,
    };

    /** Default Constructor. */
    BlackjackPlayer (const char* player_name = "You", int start_points = 10,
                     bool is_dealer = false);

    /** Destructor. */
    ~BlackjackPlayer ();

    /** Psudo-AI function that determines if a player (usually the dealer) hits 
        or holds. */
    virtual bool PlayOrPass (Dealer& dealer);

    /** Function that performs the logic of playing a hand. */
    void PlayHand ();

    /** Function that attempts to take a card from the Deck for a player.
        @pre    The Deck must not be empty.
        @pre    The player must have a max hand score of 21. */
    void HitMe (CardStack& stock);

    private:

    State state; //< State of the BlackjackPlayer FSM.
};
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_BLACKJACK_PLAYER