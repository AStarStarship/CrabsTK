/** kabuki:cards
    @file    ~/source/kabuki/arcade/cards/hand.cc
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

int Hand::Length () const {
    return visible_cards_.Length () + hidden_cards_.Length ();
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

_::Text& Hand::Print (_::Text& txt) {
    cout << "\n| Cards:";
    PrintLine ();
    visible_cards_.Print (txt);

    if (hidden_cards_.Length () > 0) {
        cout << "\n|\n| Non-visible Cards: ";
        hidden_cards_.Print (txt);
    }
}

const char* Hand::Sudo (const char* text, const char* strand_end) {
    const char* token = TextSkipSpaces (text, strand_end);
    if (!token) {
        return nullptr;
    }
    char c = *text;
    if (++text > strand_end) {
        return nullptr;
    }
    if (c == '+') {
        return visible_cards_.Sudo (text, strand_end);
    }
    if (c == '-') {
        return hidden_cards_.Sudo (text, strand_end);
    }
    return token;
}

const Operation* Hand::Star (uint index, Expression* expr) {
    static const Operation This = { "Hand",
        OperationCount (0), OperationFirst ('A'),
        "", 0 };

    switch (index) {
        case '?': return ExpressionOperand (expr, &This);
        case 'A': {
            static const Operation OpA = { "OpA",
                Params<0> (), Params<0> (),
                "OpA", 0 };
            if (!expr) return &OpA;
            return nullptr;
        }
    }
    return nullptr;
}

}   //< namespace cards
}   //< namespace kabuki
