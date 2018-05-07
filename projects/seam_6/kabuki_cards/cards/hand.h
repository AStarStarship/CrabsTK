/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/hand.h
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

#ifndef HEADER_FOR_KABUKI_ARCADE_CARDS_HAND
#define HEADER_FOR_KABUKI_ARCADE_CARDS_HAND

#include "card_stack.h"
#include "card_combo.h"

namespace kabuki { namespace cards {
    
/** Class that represents a hand in a playing card game.
    In most cases, the player will have a visible set of cards, and a
    non-visible set of cards. */
class Hand : public CardStack { 
    public:

    /** Constructs an empty hand with the given parameters.
        In order to create a Hand, first create a CardStack with a
        max_num_cards, and add the minimumNumCards to it.
        @pre    stock.getNumCards must be > minCards!!! */
    Hand (int max_cards, int min_cards = 1);
    
    /** Virtual destructor. */
    virtual ~Hand () {}

    /** Clears all the cards it the hand. */
    void Clear ();

    /** Returns the number of Cards in this Hand. */
    int Length () const;

    /** Returns the minNumCards in a Hand. */
    int GetMinNumCards ();
    
    /** Sets the minNumCards to the newNumCards. */
    int SetMinCards (int newNumCards);

    /** Returns the max_num_cards in a Hand. */
    int GetMaxCards ();
    
    /** Sets the minNumCards to the newNumCards. */
    int SetMaxCards (int newNumCards);
    
    /** Returns the visibleCards. */
    CardStack& GetVisibleCards ();
    
    /** Returns the nonVisibleCards. */
    CardStack& GetHiddenCards ();

    /** Returns a list different hand point combinations for this game. */
    //virtual Array<CardCombo> GetHandCombos ();
    
    /** Returns a string representation of this Object. */
    _::Text& Print (_::Text& txt = _::Text ());

    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end);

    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const  _::Operation* Star (uint index, _::Expression* expr);

    protected:

    int       max_cards_,     //< Max number of cards in a Hand.
              min_cards_;     //< Min number of cards in a Hand.
    CardStack visible_cards_, //< Visible cards.
              hidden_cards_;  //< Hidden cards.

};      //< class Hand
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_CARDS_HAND
