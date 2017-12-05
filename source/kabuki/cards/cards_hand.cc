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

Hand::Hand (int max_cards, int min_cards) :
    max_cards_ (max_cards),
    min_cards_ (min_cards),
    visible_cards_ (max_cards),
    hidden_cards_ (max_cards) {
}

void Hand::Clear () {
    visible_cards_.Clear ();
    hidden_cards_.Clear ();
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
    return visible_cards_.GetCount () + hidden_cards_.GetCount ();
}

int Hand::SetMaxCards (int value) {
    if (value < 0)
        return 1;
    min_cards_ = value;
    return 0;
}

CardStack& Hand::GetVisibleCards () {
    return visible_cards_;
}

CardStack& Hand::GetHiddenCards () {
    return hidden_cards_;
}

void Hand::Print () {
    cout << "\n| Cards:";
    PrintLine ('-');
    visible_cards_.Print ();

    if (hidden_cards_.GetCount () > 0) {
        cout << "\n|\n| Non-visible Cards: ";
        hidden_cards_.Print ();
    }
}

}   //< namespace cards
}   //< namespace kabuki
