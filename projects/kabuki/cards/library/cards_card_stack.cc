/** kabuki:cards_
    @file    ~/source/kabuki/arcade/cards/arcade_card_stack.cc
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

#include "card_stack.h"

using namespace _;
using namespace std;

namespace kabuki { namespace cards {

CardStack::CardStack (SI4 size) :
    cards_     () {
    cards_.reserve (size);
    // Nothing to do here. ({:-)
}

CardStack::CardStack (const CardStack& other) :
    cards_     (other.cards_) {
    // Nothing to do here. ({:-)
}

CardStack::CardStack (Deck& deck, SI4 min_cards) :
    cards_   () {
    cards_.reserve (deck.GetSize ());
    Shuffle (deck);
}

CardStack::~CardStack () {
    // Nothing to do here. ({:-)
}

void CardStack::Clear () {
    cards_.clear ();
}

CardStack& CardStack::operator= (const CardStack& other) {
    cards_     = other.cards_;
    return *this;
}

SI4 CardStack::Compare (CardStack& other) {
    SI4 point_value = GetValue ();
    SI4 other_point_value = other.GetValue (); // THis is giving me weird errors.

    if (point_value > other_point_value) {
        return 1;
    }
    if (point_value < other_point_value) {
        return -1;
    }
    return 0;
}

SI4 CardStack::GetValue () {
    SI4 total = 0;
    for (SI4 i = 0; i < Length (); ++i) {
        Card* card = cards_[i];
        total += card->GetDenomination ();
    }
    return total;
}

void CardStack::Shuffle () {
    // Create an identical copy of this CardStack.
    CardStack cards (*this);

    cards_.clear (); //< Delete all of the cards_ in this CardStack.

    // Then for each Card in the CardStack, 
    for (SI4 i = 0; i < Length (); ++i) {
        cards_.push_back (cards.TakeRandomCard ());
    }
}

void CardStack::Shuffle (Deck& deck) {
    SI4 num_cards = deck.GetSize ();
    //cout << "\n| Shuffling deck with count " << num_cards;
    CardStack unshuffled_cards (num_cards);
    for (SI4 i = num_cards - 1; i >= 0; --i) {
        Card* card = deck.GetCard (i);
        unshuffled_cards.Push (deck.GetCard (i));
    }
    cout << unshuffled_cards.Print ();
    cards_.reserve (num_cards);
    Clear ();
    //cout << "\n|\n Taking random cards\n|";
    for (SI4 i = num_cards - 1; i >= 0; --i) {
        Card* card = unshuffled_cards.TakeRandomCard ();
        //cout << "\n| " << i << ".) Pushing ";
        //card->Print (txt);
        cards_.push_back (card);
    }
    //cout << "\n|\n Shuffled cards:\n|";
    //Print (txt);
    //cout << "\n|\n CardStack successfully shuffled! :-)\n|";
}

SI4 CardStack::GetSize () const {
    return cards_.capacity ();
}

SI4 CardStack::Length () const {
    return cards_.size ();
}

SI4 CardStack::Push (Card* card) {
    if (card == nullptr) {
        return -1;
    }
    //cout << "\n| Pushing "; card->Print (txt);
    cards_.push_back (card);
    return Length ();
}

Card* CardStack::Draw () {
    Card* card = cards_[Length ()];
    cards_.pop_back ();
    return card;
}

SI4 CardStack::InsertCard (Card* card, SI4 index) {
    auto it = cards_.begin ();
    cards_.insert (it + index, card);
    return Length ();
}

SI4 CardStack::Push (CardStack& new_cards) {
    size_t new_count = Length () + new_cards.Length ();
    if (new_count > cards_.capacity ())
        return 1;

    auto it = cards_.begin ();
    cards_.insert (it, new_cards.cards_.begin (), new_cards.cards_.end ());
    //for (it = cards_.begin (); it != cards_.end ();
    //     ++it) {
    //    cards_.push_back (*it);
    //}
    return Length ();
}

SI4 CardStack::DrawCards (CardStack& cards, SI4 num_cards_take) {
    if (num_cards_take < 0) { //< Remember the @pre thing???
        return -1;
    }

    if (num_cards_take > cards.Length ())
        return 1;

    size_t value = Length () + num_cards_take;
    if (value > cards_.capacity ())
        return 2;

    for (SI4 i = 0; i < num_cards_take; ++i)
        Push (cards.TakeNextCard ());

    return 0;
}

SI4 CardStack::Count (Card* card) {
    SI4 count = 0;
    for (SI4 i = 0; i < Length (); ++i) {
        count += cards_[i]->Equals (card)?1:0;
    }
    return count;
}

SI4 CardStack::Contains (Card* card) {
    SI4 count = 0;
    for (SI4 i = 0; i < Length (); ++i) {
        if (cards_[i]->Equals (card)) {
            return true;
        }
    }
    return false;
}

BOL CardStack::RemoveCard (Card* card) {
    SI4 count = 0;
    for (SI4 i = 0; i < Length (); ++i) {
        if (cards_[i]->Equals (card)) {
            auto it = cards_.begin ();
            cards_.erase (it + i);
            return true;
        }
    }
    return false;
}

SI4 CardStack::SetCards (CardStack& stack, SI4 num_cards) {
    if (num_cards < 0) {
        return -1;
    }
    if (num_cards > stack.Length ()) {
        return 1;
    }
    if (num_cards > Length ()) {
        return 3;
    }
    cards_.clear ();

    if (num_cards == 1) {
        cards_.push_back (stack.GetCard (0));
        return Length ();
    }

    for (SI4 i = 0; i < num_cards - 1; ++i)
        cards_.push_back (stack.GetCard (i));

    cards_.push_back (stack.GetCard (num_cards - 1));
    return Length ();
}

SI4 CardStack::TakeCards (CardStack& cards, SI4 num_cards) {
    if (num_cards < 1)
        return -1;

    SI4 count = cards.Length ();
    if (num_cards > count) {
        // Not enough cards_ on the stack.
        return 1;
    }
    size_t new_count = count + num_cards;
    if (new_count > cards_.capacity ()) {
        return 3;
    }

    for (SI4 i = 0; i < num_cards - 1; ++i) {
        cards_.push_back (cards.TakeNextCard ());
    }
    cards_.push_back (cards.TakeNextCard ());
    return Length ();
}

Card* CardStack::GetCard (SI4 index) {
    SI4 num_cards = Length ();
    if (num_cards == 0 || index >= num_cards) {
        return nullptr;
    }
    Card* card = cards_[index];
    return card;
}

Card* CardStack::TakeCard (SI4 index) {
    SI4 num_cards = Length ();

    if (num_cards <= 0 || index >= num_cards) // Arrays start at 0, so if we used cards_[num_cards], it would be out of bounds.
        return nullptr;

    Card* return_card = cards_[index];
    cards_.erase (cards_.begin () + index);
    return return_card;
}

Card* CardStack::TakeNextCard () {
    if (Length () == 0) // Then there are no cards_ in the cards_.
        return nullptr;

    SI4 index = Length ();
    // Golden rule: If you use a variable in a function more than once, store a
    // local copy to get a performance increase.
    Card* next_card = cards_[index];
    cards_.erase (cards_.begin () + index);
    return next_card;
}

Card* CardStack::TakeRandomCard () {
    if (Length () <= 0) // Then there are no cards_!
        return nullptr;

    srand ((uint)time (nullptr));
    SI4 index = rand () % Length ();
    Card* card = cards_[index];
    cards_.erase (cards_.begin () + index);
    //cout << "\n| Taking card:" << index << ' ';
    //card->Print ();
    return card;
}

BOL CardStack::IsEmpty () {
    return Length () == 0;
}

_::Text& CardStack::Print (_::Text& txt) {
    PrintLine ('-', "\n>");
    cout << "\n| CardStack: Count:" << Length ();
    for (SI4 i = 0; i < Length (); ++i) {
        cout << "\n| " << i << ".) ";
        Card* card = cards_[i];
        card->Print (txt);
    }
}

}   //< namespace cards
}   //< namespace kabuki
