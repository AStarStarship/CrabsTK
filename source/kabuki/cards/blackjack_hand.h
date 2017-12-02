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

#ifndef HEADER_FOR_KABUKI_BLACKJACK_HAND
#define HEADER_FOR_KABUKI_BLACKJACK_HAND

#include "hand.h"
#include "card_stack.h"

namespace kabuki { namespace cards {

/** Class that represents a Hand in Blackjack.
    A blackjack hand has a minimum of 2 cards. */
class BlackjackHand : public Hand {
    public:

    /** Constructor.
        The game of blackjack starts with each player having two cards. */
    BlackjackHand (CardStack& stock);

    /** Compares this hand to the other hand.
        @return Returns 0 if they are equal, 1 if this hand is greater than 
                the other Hand, and -1 if the other Hand is greater than this
                Hand. */
    virtual int Compare (const Hand& other);

    /** Returns the HighLowScore. */
    //data::Array<CardCombo> GetHandCombos ();

    /** Gets the hand score with the given ace value. */
    int GetScore (int ace_value);

    /** Returns the min score of this hand where Aces are worth 1. */
    int GetMinScore ();

    /** Returns the max score of this hand where Aces are worth 11. */
    int GetMaxScore ();

    /** Adds the new Card to this Hand. */
    int Add (Card* card);

    /** Function returns if this hand is 21. */
    bool Is21 ();

    /** Function returns true if the hand is a bust.
        A hand is a bust if it is over 21 points.
    */
    bool IsBust ();
    /** Performs round logic. */
    void HitMe (CardStack& stock);

    /** Prints this object to the console. */
    void Print ();

};      //< class BlackjackHand
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_BLACKJACK_HAND
