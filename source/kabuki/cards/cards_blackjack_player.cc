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

using namespace _;
using namespace std;

namespace kabuki { namespace cards {

BlackjackPlayer::BlackjackPlayer (CardStack& stock, const char* player_name,
                                  int starting_points,  bool is_dealder) :
    Player (stock, player_name, starting_points, is_dealder) {
    // Nothing to do here!
}

BlackjackPlayer::~BlackjackPlayer () {
    // Nothing to do here.
}

const char* BlackjackPlayer::SetState (int state) {
    static const char* error = "Invalid state";
    if (state < 0) {
        return error;
    }
    if (state > 3) {
        return error;
    }
    state_ = state;
    return nullptr;
}

bool BlackjackPlayer::IsHolding () {
    return state_ == kStateHolding;
}



int BlackjackPlayer::Compare (Hand& other) {
    BlackjackPlayer& other_hand = dynamic_cast<BlackjackPlayer&> (other);

    int min_score = GetMinScore (),
        max_score = GetMaxScore (),
        other_min_score = other_hand.GetMinScore (),
        other_max_score = other_hand.GetMaxScore ();

    if (min_score == 21 || max_score == 21)
        if (other_min_score == 21 || other_max_score == 21) // We both got 21!
            return 2;
        else
            return 3; //< We won!

    if (other_min_score == 21 || other_max_score == 21) //< We lost.
        return -2;

    int best_score = max_score > 21?min_score:max_score,
        other_best_score = other_max_score > 21 ? other_min_score:other_max_score;
    
    if (best_score > 21)           //< We lost :-(
        if (other_best_score > 21) //< We both lost!
            return -2;
        else
            return -1;

    if (other_best_score > 21)     //< We won!!!
        return 1;

    if (best_score > other_best_score)
        return 1;

    //if (best_score == other_best_score)
    return 0;
}

/*
data::Array<CardCombo> BlackjackPlayer::GetHandCombos () {
    auto low_score,
        high_score;

    scores.low = 0;  //< scores.low is the total of the cards with aces as 1
    scores.high = 0; //< scores.high is the number of aces to add on an opt 10.

    for (int s = 0; s < GetCount (); ++s) {
        int currentValue = GetCard (s)->GetPip ();

        if (currentValue == 1) // First check if its an ace
        {
            ++scores.low;       //< Aces low means add 1 to the low total.
            scores.high += 10;  //< Aces high means add a 10 to the high total.
        } else if (currentValue >= 10) {
            // Next check to see if its a Jack, Queen, or King because they
            // are all 10 points.

            scores.low += 10;
            scores.high += 10;
        } else {
            // then its just number card so add it to both the low and high totals.
            scores.low += currentValue;
            scores.high += currentValue;
        }
    }

    return scores;
}*/

int BlackjackPlayer::GetScore (int ace_value) {
    int score = 0;  //< Always set the variable before you start using it!!!

    Card* card;
    for (int i = 0; i < hand_.GetVisibleCards ().GetCount (); ++i) {
        card = hand_.GetVisibleCards ().GetCard (i);
        int denomination = card->GetDenomination ();
        score += ((denomination == Card::kAce) ? ace_value : denomination);
    }

    return score;
}

int BlackjackPlayer::GetMinScore () {
    return GetScore (1);
}

int BlackjackPlayer::GetMaxScore () {
    return GetScore (11);
}

bool BlackjackPlayer::Is21 () {
    return GetMinScore () == 21 || GetMaxScore () == 21;
}

bool BlackjackPlayer::IsBust () {
    return GetMaxScore () > 21;
}

void BlackjackPlayer::PrintHand () {
    cout << "\n| BlackjackPlayer:"
         << "\n| High score: " << GetScore (Deck::kAcesHigh) << " Low Score: "
         << GetScore (Deck::kAcesLow);
    hand_.Print ();
}
/**
bool BlackjackPlayer::PlayOrPass (Dealer& dealer) {
    // Estimated chances of winning. 
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

    return chance_of_winning * randomNumberBetween0and1 >= 0.5; // If the percent chance is greater than 50%, then we should take another card.
    return 0;
}*/

void BlackjackPlayer::NewGame () {

}

void BlackjackPlayer::BeginRound () {

}

void BlackjackPlayer::PlayRound () {
    cout << "\n| " << GetName () << "'s turn.";
    if (stock_.IsEmpty ()) {  // Always check for error conditions first!
        cout << "\n| Error! The Deck was empty.";
        state_ = kStateHolding;
        return;
    }
    if (IsBust () || Is21 ()) {
        // If the player's hand is a bust, than the dealer is not allowed to
        // deal that player another card.
        state_ = kStateHolding;
        return;
    }

    hand_.GetVisibleCards ().Push (stock_.Draw ());

    if (IsBust ()) {
        // If the player's hand is a bust, than the dealer is not allowed to
        // deal that player another card.
        state_ = kStateHolding;
    }
}

void BlackjackPlayer::EndRound () {

}

void BlackjackPlayer::EndGame () {

}

bool BlackjackPlayer::HandWins (Hand& other) {
    return hand_.Compare (other) > 0;
}

void BlackjackPlayer::PrintStats () {

}

void BlackjackPlayer::Print () {
    PrintLine ("|", '_');
    cout << "Player: " << name_ <<
        "\n| Num Points: " << num_points_ << " Num Wins: " << num_wins_;

    hand_.GetVisibleCards ().Print ();
    PrintLine ("|", '_');
}

}   //< namespace cards
}   //< namespace kabuki
