/** kabuki:cards_
    @file    ~/source/kabuki/cards/cards_card_stack.cc
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

CardStack::CardStack (int size) :
    min_cards_ (0),
    max_cards_ (Deck::kFullDeckSize),
    visible_ (true),
    cards_ (size) {
    // Nothing to do here. ({:-)
}
/*
CardStack::CardStack (const CardStack& cards, int min_cards, int max_cards,
                      bool visible) :
    min_cards_ (min_cards < 0 ? 0 : min_cards),
    max_cards_ (max_cards < 1 ? 1 : max_cards),
    visible_   (visible),
    cards_     (cards) {
    // Nothing to do here. ({:-)
} */

CardStack::CardStack (Deck& deck) :
    max_cards_ (deck.GetCount ()),
    visible_ (false) {
    CardStack temp (54);
    for (int i = 0; i < deck.GetCount (); ++i) {
        temp.Push (deck.GetCard (i));
    }
    for (int i = 0; i < deck.GetCount (); ++i) {
        cards_.Push (temp.TakeRandomCard ());
    }
}

void CardStack::Clear () {
    cards_.Clear ();
}

CardStack::CardStack (const CardStack& other) :
    max_cards_ (other.max_cards_),
    cards_     (other.cards_    ),
    visible_   (other.visible_  ) {
    // Nothing to do here.
}

CardStack& CardStack::operator= (const CardStack& other) {
    min_cards_ = other.min_cards_;
    max_cards_ = other.max_cards_;
    visible_   = other.visible_;
    cards_     = other.cards_;
    return *this;
}

int CardStack::Compare (CardStack& other) {
    int point_value       = GetValue (),
        other_point_value = other.GetValue ();

    if (point_value > other_point_value)
        return 1;
    if (point_value < other_point_value)
        return -1;
    return 0;
}

int CardStack::GetValue () {
    int total = 0;
    for (int i = 0; i < cards_.GetCount (); ++i) {
        total += cards_.Element (i)->GetDenomination ();
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

void CardStack::Shuffle (Deck& deck) {
    int num_cards = deck.GetCount ();
    CardStack cards (deck);
    cards_.Grow (num_cards);
    Clear ();
    for (int i = 0; i < num_cards; ++i) {
        cards_.Push (cards.TakeRandomCard ());
    }
}

int CardStack::GetSize () {
    return cards_.GetSize ();
}

int CardStack::GetCount () {
    return cards_.GetCount ();
}

int CardStack::GetMinNumCards () {
    return min_cards_;
}

int CardStack::GetMaxCards () {
    return max_cards_;
}

int CardStack::Push (Card* card) {
    if (card == nullptr) {
        return -1;
    }
    if (GetCount () + 1 > max_cards_) {
        return GetCount ();
    }
    if (max_cards_ > cards_.GetCount () + 1) {
        // Then we are a the max card number limit.
        return 1;
    }
    cards_.Push (card);
    return cards_.GetCount ();
}

Card* CardStack::Draw () {
    return cards_.Pop ();
}

int CardStack::InsertCard (Card* card, int index) {
    if (index < 0)
        return -1;
    if (index > cards_.GetCount ())
        return -1;
    if (GetCount () + 1 > max_cards_)
        return 2;

    return cards_.Insert (card, index);
}

int CardStack::Push (CardStack& cards) {
    if (GetCount () + cards.GetCount () > max_cards_)
        return 1;

    return cards_.Push (cards.cards_);
}

int CardStack::DrawCards (CardStack& cards, int num_cards_take) {
    if (num_cards_take < 0) { //< Remember the @pre thing???
        return -1;
    }

    if (num_cards_take > cards.GetCount ())
        return 1;

    if (GetCount () + num_cards_take > max_cards_)
        return 2;

    for (int i = 0; i < num_cards_take; ++i)
        Push (cards.TakeNextCard ());

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

    if (num_cards > stack.GetCount ())
        return 1;

    if (num_cards < min_cards_)
        return 2;

    if (num_cards > max_cards_)
        return 3;

    cards_.Clear ();

    if (num_cards == 1) {
        return cards_.Push (stack.GetCard (0));
    }

    for (int i = 0; i < num_cards - 1; ++i)
        cards_.Push (stack.GetCard (i));

    return cards_.Push (stack.GetCard (num_cards - 1));
}

int CardStack::TakeCards (CardStack& stack, int num_cards) {
    if (num_cards < 0)
        return -1;

    if (num_cards > stack.GetCount ()) {
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

    if (num_cards == 1) {
        return cards_.Push (stack.TakeNextCard ());
    }

    for (int i = 0; i < num_cards - 1; ++i) {
        cards_.Push (stack.TakeNextCard ());
    }
    return cards_.Push (stack.TakeNextCard ());
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

    srand ((uint)time (nullptr));

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
