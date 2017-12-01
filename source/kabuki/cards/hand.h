/** kabuki:cards
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

#ifndef KABUKI_CARDS_HAND_H
#define KABUKI_CARDS_HAND_H

#include "card_stack.h"
#include "card_combo.h"

namespace kabuki { namespace cards {
    
/** Class that represents a hand in a playing card game.
    In most cases, the player will have a visible set of cards, and a non-visible set of cards.
*/
class Hand : public CardStack { 
    public:

    /** Default constructor. */
    Hand ();

    /** Constructs an empty hand with the given parameters.
        In order to create a Hand, first create a CardStack with a maxNumCards, and add the minimumNumCards to it.
        @pre    stock.getNumCards must be > minCards!!!
    */
    Hand (CardStack& stock, int minCards = 1, int maxNumCards = Deck::defaultNumCardsWithJokers);
    
    /** Virtual destructor. */
    virtual ~Hand () {}

    /** Operator= overloader deep copiers the state of the other object. */
    Hand& operator= (const Hand& other);

    /** Returns the minNumCards in a Hand. */
    int GetMinNumCards ();
    
    /** Sets the minNumCards to the newNumCards. */
    int SetMinCards (int newNumCards);

    /** Returns the maxNumCards in a Hand. */
    int GetMaxNumCards ();
    
    /** Sets the minNumCards to the newNumCards. */
    int SetMaxCards (int newNumCards);

    /** Retruns the number of Cards in this Hand. */
    int GetNumCards ();

    /** Adds the card to this Hand. */
    virtual int AddCard (Card* card);

    /** Adds the card to this Hand. */
    int DrawCard (CardStack& stack);
    
    /** Returns a combination of the visible and nonvisible cards. */
    CardStack GetCards ();
    
    /** Returns the visibleCards. */
    CardStack& GetVisibleCards ();
    
    /** Returns the nonVisibleCards. */
    CardStack& GetNonVisibleCards ();

    /** Returns a list different hand point combinations for this game. */
    virtual Array<CardCombo> GetHandCombos ();

    /** Compares this hand to the other hand.
        @return Returns 0 if they are equal, 1 if this hand is greater than 
            the other Hand, and -1 if the other Hand is greater than this Hand. */
    virtual int Compare (const Hand& other);

    /** Virtual function that organizes the player's hand. */
    virtual void Organize () {}
    
    /** Returns a string representation of this Object. */
    void Print (_::Log& log);

    private:

    int       minNumCards,      //< Min number of cards in a Hand.
              maxNumCards;      //< Max number of cards in a Hand.

    CardStack visibleCards,     //< The visible cards.
              nonVisibleCards;  //< The non-visible cards.
};

}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_HAND_H

