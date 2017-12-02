/** kabuki::cards
    @file       ~/source/kabuki/cards/cards_blackjack_hand.cc
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
#include "blackjack_hand.h"

namespace kabuki {
namespace cards {

BlackjackHand::BlackjackHand (CardStack& startingCards) :
    Hand (startingCards, 2, Deck::kDefaultNumCards) {
}

int BlackjackHand::Compare (const Hand& other) {
    return 0;
}

/*
data::Array<CardCombo> BlackjackHand::GetHandCombos () {
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

int BlackjackHand::GetScore (int ace_value) {
    int score = 0;  //< Always set the variable before you start using it!!!

    Card* card;
    for (int i = 0; i < visible_cards_.GetCount (); ++i) {
        card = visible_cards_.GetCard (i);
        int face_value = card->GetFace ();
        score += ((face_value == Card::kAce) ? ace_value : face_value);
    }

    return score;
}

int BlackjackHand::GetMinScore () {
    return GetScore (1);
}

int BlackjackHand::GetMaxScore () {
    return GetScore (11);
}


int BlackjackHand::Add (Card* card) {
    if (GetCount () + 1 > GetMaxCards ())
        return 1;
    return nonvisible_cards_.Push (card);
}

bool BlackjackHand::Is21 () {
    return GetMinScore () == 21 || GetMaxScore () == 21;
}

bool BlackjackHand::IsBust () {
    return GetMaxScore () > 21;
}

void BlackjackHand::Print () {
    cout << "\n| BlackjackHand:"
         << "\n| High score: " << GetScore (Deck::kAcesHigh) << " Low Score: "
         << GetScore (Deck::kAcesLow);
    CardStack::Print ();
}

}   //< namespace cards
}   //< namespace kabuki
