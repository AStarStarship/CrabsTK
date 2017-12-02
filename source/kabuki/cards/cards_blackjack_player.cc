/** kabuki::cards
    @file       ~/source/kabuki/cards/cards_blackjack_player.cc
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
#include "blackjack_player.h"

#include "hand.h"
#include "card_stack.h"
#include "blackjack_dealer.h"
#include "blackjack_hand.h"

using namespace _;
using namespace std;

namespace kabuki { namespace cards {

BlackjackPlayer::BlackjackPlayer (const char* player_name, int starting_points,
                                  bool is_dealder) :
    Player (player_name, starting_points, is_dealder) {
    // Nothing to do here!
}

BlackjackPlayer::~BlackjackPlayer () {
    // Nothing to do here.
}

bool BlackjackPlayer::PlayOrPass (Dealer& dealer) {
    /** Estimated chances of winning. 
    static const float chanes_of_winning_low[] = {
        0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f
    };
    static const float chanes_of_winning_high[] = {
        0.6f, 0.6f, 0.5f, 0.5f, 0.4f, 0.4f, 0.2f, 0.2f, 0.2f
    };

    int this_high_score  = GetScore (Deck::kAcesHigh),
        this_low_score   = GetScore (Deck::kAcesLow),
        other_high_score = other_score.GetScore (Deck::kAcesHigh),
        other_low_score  = other_score.GetScore (Deck::kAcesLow);

    if (this_low_score > 21) //< Then we definitely lost.
        return false;

    if (this_low_score == 21 || this_high_score == 21) {
        // Then we might have won so don't play again or we'll loose!
        return false;
    }

    if (this_high_score <= 21 - 10) {
        // The highest card value is 10, so if our highest score is less than
        // 11, then we can take any card and not go over 21.
        return true;
    }

    if (this_high_score < other_high_score) {
        // Than we have to play or we loose.
        return true;
    }
    */
    /** From here on out, we don't actually have to take another card because
        we might go over and loose, so we're playing a numbers game.

        At this point in time, we are less than 10 points away from going bust,
        and each number of points away from 21 we are has an x percent chance
        loosing. In this case, we should figure out what the percent chance is,
        and only take another card if the chances are high enough. 
    float chance_of_winning;

    if (this_high_score < 21) {
        int numPointsLeft = 21 - this_high_score;
        chance_of_winning = chanes_of_winning_low[numPointsLeft];
    } else {
        int numPointsLeft = 21 - this_low_score;
        chance_of_winning = chanes_of_winning_high[numPointsLeft];
    }

    if (chance_of_winning >= 0.5)
        return true;*/

    /** To do this, we're going to use a random number between 0 and 1, and
        multiply it by a number between 1-10 to get a percent.
        For example, a 50% chance of winning would get the number 5, then if you
        multiplied that by a random number between 0 and 1, then we have a 50%
        chance of getting a number greater than 0.5. 

    float randomNumberBetween0and1 = RandomFloat ();
    // We want to pick a random number between 0 and 1

    return chance_of_winning * randomNumberBetween0and1 >= 0.5; // If the percent chance is greater than 50%, then we should take another card.*/
    return 0;
}

void BlackjackPlayer::HitMe (CardStack& stock) {
    cout << "\n| " << GetName () << "'s turn.";
    if (stock.IsEmpty ()) {  // Always check for error conditions first!
        cout << "\n| Error! The Deck was empty.";
        state = State::kHoldingState;
        return;
    }
    BlackjackHand* hand = dynamic_cast <BlackjackHand*> (GetHand ());
    if (hand->IsBust () || hand->Is21 ()) {
        // If the player's hand is a bust, than the dealer is not allowed to
        // deal that player another card.
        state = State::kHoldingState;
        return;
    }

    Card* card = stock.Draw ();
    hand->Push (card);

    if (hand->IsBust ()) {
        // If the player's hand is a bust, than the dealer is not allowed to
        // deal that player another card.
        state = State::kHoldingState;
    }
}

void BlackjackPlayer::PlayHand () {
    // Nothing to do here. ({:-)
}

}   //< namespace cards
}   //< namespace kabuki
