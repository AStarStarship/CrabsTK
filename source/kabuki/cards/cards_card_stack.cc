/** kabuki:cards_
    @file    ~/source/kabuki/cards/card_stack.cc
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

#include "card_stack.h"

namespace kabuki { namespace cards {

CardStack::CardStack () :
    min_cards_ (0),
    max_cards_ (Deck::kFullDeckSize),
    visible_ (true),
    cards_ () {
    // Nothing to do here. ({:-)
}

CardStack::CardStack (CardStack& cards, int min_cards, int max_cards,
                      bool visible) :
    min_cards_ (min_cards < 0?0:min_cards),
    max_cards_ (max_cards < 1?1:max_cards),
    visible_ (visible) {
    // Nothing to do here. ({:-)
}

CardStack::CardStack (Deck& deck) :
    max_cards_ (deck.GetNumCards ()),
    visible_ (false) {
    for (int i = 0; i < deck.GetNumCards (); ++i)
        AddCard (deck.GetCard (i));
}

CardStack::CardStack (const CardStack& other) :
    max_cards_ (other.max_cards_),
    cards_ (other.cards_),
    visible_ (other.visible_) {
    // Nothing to do here.
}

CardStack& CardStack::operator= (const CardStack& other) {
    min_cards_ = other.min_cards_;
    max_cards_ = other.max_cards_;
    visible_ = other.visible_;
    cards_ = other.cards_;
}

int CardStack::Compare (CardStack& other) {
    int point_value       = GetPointValue (),
        other_point_value = other.GetPointValue ();

    if (point_value > other_point_value)
        return 1;
    if (point_value < other_point_value)
        return -1;
    return 0;
}

int CardStack::GetPointValue () {
    int total = 0;
    for (int i = 0; i < cards_.GetCount (); ++i) {
        total += cards_.Element (i)->GetPointValue ();
    }
    return total;
}

void CardStack::Shuffle () {
    // Create an identical copy of this CardStack.
    CardStack cards (*this);

    cards_.Clear (); //< Delete all of the cards_ in this CardStack

                    // Then for each Card in the CardStack, 
    for (int i = 0; i < cards_.GetCount (); ++i) {
        cards_.Push (cards.TakeRandomCard ());
    }
}

int CardStack::GetNumCards () {
    return cards_.GetCount ();
}

int CardStack::GetMinNumCards () {
    return min_cards_;
}

int CardStack::GetMaxNumCards () {
    return max_cards_;
}

int CardStack::AddCard (Card* card) {
    if (card == nullptr) {
        return -1;
    }
    if (GetNumCards () + 1 > max_cards_) {
        return GetNumCards ();
    }
    if (max_cards_ > cards_.GetCount () + 1) {
        // Then we are a the max card number limit.
        return 1;
    }
    cards_.Push (card);
    return cards_.GetCount ();
}

int CardStack::InsertCard (Card* card, int index) {
    if (index < 0)
        return -1;
    if (index > cards_.GetCount ())
        return -1;
    if (GetNumCards () + 1 > max_cards_)
        return 2;

    cards_.Insert (card, index);
}

int CardStack::AddCards (CardStack& cards) {
    if (GetNumCards () + cards.GetNumCards () > max_cards_)
        return 1;

    cards_.Push (cards.cards_);
}

int CardStack::DrawCards (CardStack& stackToDrawFrom, int num_cards_take) {
    if (num_cards_take < 0) { //< Remember the @pre thing???
        return -1;
    }

    if (num_cards_take > stackToDrawFrom.GetNumCards ())
        return 1;

    if (GetNumCards () + num_cards_take > max_cards_)
        return 2;

    for (int i = 0; i < num_cards_take; ++i)
        AddCard (stackToDrawFrom.TakeNextCard ());

    return 0;
}

bool CardStack::RemoveCard (Card* card) {
    if (!cards_.Contains (card))
        return false;
    cards_.RemoveFirstInstanceOf (card);
    return true;
}

int CardStack::SetCards (CardStack& stack, int num_cards) {
    if (num_cards < 0)
        return -1;

    if (num_cards > stack.GetNumCards ())
        return 1;

    if (num_cards < min_cards_)
        return 2;

    if (num_cards > max_cards_)
        return 3;

    cards_.Clear ();

    for (int i = 0; i < num_cards; ++i)
        cards_.Push (stack.GetCard (i));
}

int CardStack::TakeCards (CardStack& stack, int num_cards) {
    if (num_cards < 0)
        return -1;

    if (num_cards > stack.GetNumCards ()) {
        // Not enough cards_ on the stack.
        return 1;
    }

    if (num_cards < min_cards_) { //< Min number of cards_.
        return 2;
    }

    if (num_cards > max_cards_) {
        return 3;
    }

    cards_.Clear ();

    for (int i = 0; i < num_cards; ++i) {
        cards_.Push (stack.TakeNextCard ());
    }
}

Card* CardStack::GetCard (int index) {
    int num_cards = cards_.GetCount ();

    if (num_cards == 0 || index > num_cards)
        return nullptr;

    return cards_[index];
}

Card* CardStack::TakeCard (int index) {
    int num_cards = cards_.GetCount ();

    if (num_cards <= 0 || index >= num_cards) // Arrays start at 0, so if we used cards_[num_cards], it would be out of bounds.
        return nullptr;

    Card* return_card = cards_[index];
    cards_.Remove (index);
    return return_card;
}

Card* CardStack::TakeNextCard () {
    if (cards_.GetCount () == 0) // Then there are no cards_ in the cards_.
        return nullptr;

    int lastIndex = cards_.GetCount () - 1; // Golden rule: If you use a variable in a function more than once, store a local copy to get a performance increase.
    Card* next_card = cards_[lastIndex];
    cards_.Remove (lastIndex);
    return next_card;
}

Card* CardStack::TakeRandomCard () {
    if (cards_.GetCount () == 0) // Then there are no cards_ in the cards_.
        return nullptr;

    srand (time (nullptr));

    int randomIndex = rand () % cards_.GetCount ();
    Card* return_card = cards_[randomIndex];
    cards_.Remove (randomIndex);
    return return_card;
}

bool CardStack::IsEmpty () {
    return cards_.GetCount () == 0;
}

bool CardStack::IsVisible () {
    return visible_;
}

void CardStack::SetVisiblity (bool visiblity) {
    visible_ = visiblity;
}

void CardStack::Print () {
    for (int i = 0; i < cards_.GetCount (); ++i) {
        cout << "\n| " << i << ".) ";
        cards_[i]->Print ();
    }
}

}   //< namespace cards
}   //< namespace kabuki
