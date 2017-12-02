/** kabuki:cards
    @file    ~/source/kabuki/cards/hand.cc
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

#include "hand.h"

using namespace _;
using namespace std;

namespace kabuki { namespace cards {

Hand::Hand (CardStack& stock, int min_cards, int max_cards) :
    min_cards_ (min_cards),
    max_cards_ (max_cards),
    visible_cards_ (),
    nonvisible_cards_ () {
}

int Hand::GetMinNumCards () {
    return min_cards_;
}

int Hand::SetMinCards (int value) {
    if (value < 0)
        return value;
    min_cards_ = value;
    return 0;
}

int Hand::GetMaxCards () {
    return max_cards_;
}

int Hand::GetCount () {
    return visible_cards_.GetCount () + nonvisible_cards_.GetCount ();
}

int Hand::SetMaxCards (int value) {
    if (value < 0)
        return 1;
    min_cards_ = value;
    return 0;
}

int Hand::Add (Card* card) {
    return GetCards ().Push (card);
}

int Hand::Draw (CardStack& stack) {
    return Add (stack.TakeNextCard ());
}

CardStack Hand::GetCards () {
    auto returnStack = CardStack ();

    return returnStack;
}

CardStack& Hand::GetVisibleCards () {
    return visible_cards_;
}

CardStack& Hand::GetNonVisibleCards () {
    return nonvisible_cards_;
}

void Hand::Organize () {}

void Hand::Print () {
    cout << "\n| Cards:";
    PrintLine ('-');
    visible_cards_.Print ();

    if (nonvisible_cards_.GetCount () > 0) {
        cout << "\n|\n| Non-visible Cards: ";
        nonvisible_cards_.Print ();
    }
}

Hand& Hand::operator= (const Hand& hand) {
    min_cards_ = hand.min_cards_;
    max_cards_ = hand.max_cards_;

    visible_cards_ = hand.visible_cards_;
    nonvisible_cards_ = hand.nonvisible_cards_;
    return *this;
}

}   //< namespace cards
}   //< namespace kabuki
