/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Blackjack/Hand.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include "CardStack.hpp"
#include "CardCombo.hpp"

namespace KabukiCardGames {
    
class Hand : public CardStack
/*< Class that represents a hand in a playing card game.
    In most cases, the player will have a visible set of cards, and a non-visible set of cards.
*/
{ 
public:

    /** Default constructor. */
    Hand ();

    Hand (CardStack& stock, int minCards = 1, int maxNumCards = Deck::defaultNumCardsWithJokers);
    /*< Constructs an empty hand with the given parameters.
        In order to create a Hand, first create a CardStack with a maxNumCards, and add the minimumNumCards to it.
        @pre    stock.getNumCards must be > minCards!!!
    */
    
    virtual ~Hand () {}
    //< Virtual destructor.

    Hand& operator= (const Hand& other);
    //< Operator= overloader deep copiers the state of the other object.

    int getMinNumCards ();
    //< Returns the minNumCards in a Hand.
    
    int setMinCards (int newNumCards);
    //< Sets the minNumCards to the newNumCards.

    int getMaxNumCards ();
    //< Returns the maxNumCards in a Hand.
    
    int setMaxCards (int newNumCards);
    //< Sets the minNumCards to the newNumCards.

    int getNumCards ();
    //< Retruns the number of Cards in this Hand.

    virtual int addCard (Card* newCard);
    //< Adds the newCard to this Hand.

    int drawCard (CardStack& stack);
    //< Adds the newCard to this Hand.
    
    CardStack getCards ();
    //< Returns a combination of the visible and nonvisible cards.
    
    CardStack& getVisibleCards ();
    //< Returns the visibleCards.
    
    CardStack& getNonVisibleCards ();
    //< Returns the nonVisibleCards.

    virtual Array<CardCombo> getHandCombos ();
    //< Returns a list different hand point combinations for this game.

    virtual int compare (const Hand& other);
    /*< Compares this hand to the other hand.
        @return Returns 0 if they are equal, 1 if this hand is greater than 
            the other Hand, and -1 if the other Hand is greater than this Hand. */

    virtual void organize () {}
    //< Virtual function that organizes the player's hand.
    
    string tostring ();
    //< Returns a string representation of this Object.

private:

    int minNumCards,            //< The minimum number of cards in a Hand.
        maxNumCards;            //< The maximum number of cards in a Hand.

    CardStack visibleCards,     //< The visible cards.
        nonVisibleCards;        //< The non-visible cards.
};

}   //< KabukiCardGames

