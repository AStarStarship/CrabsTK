/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/CardStack.hpp
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

#include "Card.hpp"
#include "Deck.hpp"

namespace KabukiCardGames {

/** Class that represents a stack of playing cards.
    A CardStack stores pointers to the Card objects in the Deck. When a card is removed from the stack, the pointer is 
    deleted, and not the actual Card object in the Deck. You can extend the CardStack class to program the various 
    elements of a card game use as a Hand and discardstack.
*/
class CardStack
{
public:
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    static const CardStack empty;

    CardStack ();
    /*< Constructs an empty card stack with a max of 54 cards. */
    
    CardStack (CardStack& cardStack, int thisMinNumCards = 0, int thisMaxNumCards = Deck::defaultNumCardsWithJokers, 
        bool thisstackIsVisible = false);
    /** Verbose constructor. */
    
    CardStack (Deck& theDeck);
    //< Creates a CardStack from theDeck.
    
    CardStack (const CardStack& other);
    /*< Copy constructor deep copies the state of the other object.
        The copy constructor is use shuffle the CardStack. */
        
    virtual ~CardStack () {}
    //< Destructor.
    
    CardStack& operator= (const CardStack& other);
    //< Operator= overloader makes this object = other object.

    virtual int compare (CardStack& other);
    /*< Compares this CardStack to the other CardStack.
        @return Returns 0 if the two have equal values, 1 if this stack is greater, and -1 if the other stack is 
        greater. */
        
    virtual int getPointValue ();
    //< Returns the point value total of this stack of cards.

    void shuffle ();
    //< Shuffles this CardStack.

    int getNumCards ();
    //< Returns the number of Card(s) in this stack.
    
    int getMinNumCards ();
    //< Returns the minNumCards.
    
    int getMaxNumCards ();
    //< Returns the maxNumCards.

    int addCard (Card* newCard);
    //< Adds the newCard to top of the stack.
    
    int insertCard (Card* newCard, int index);
    /*< Inserts the newCard into the stack at the specified index.
        @return Returns 0 upon success
        @return Returns 1 if the index is out of bounds.
        @return Returns 2 if adding a card about exceed the manNumCards. */
        
    int addCardStack (CardStack& newCards);
    /*< Adds the newCards to the stack.
        Functions takes the numCardsToTake from the cardsToTakeFrom and adds them to this Stack.
        @pre    cardsToTakeFrom.getNuMCards () must be > numCardsToTake.
        @pre    numCardsToTake must be less than the maximum number of cards allowed.
        @return Returns 0 upon success.
        @return Returns -1 if the numCardsToTake is < 0.
        @return Returns 1 if the numCardsToTake is greater than the cardsToTakeFrom.getNumCards ()
        @return Returns 2 if the numCardsToTake would put the user over the maxNumCards.
    */
    int drawCards (CardStack& cardsToTakeFrom, int numCardsToTake);

    bool removeCard (Card* thisCard);
    /*< Removes thisCard from the stack.
       @return Returns true upon success and false if this stack doesn't contain thisCard. */

    int setCards (CardStack& stack, int numCards);
    /*< Copies the numCards from the stack
        @pre    numCards must be greater than the number of cards in the stock. */
        
    int takeCards (CardStack& stock, int numCards);
    /*< Draws the specified numCards and adds them to this stack
        @pre    numCards must be greater than the number of cards in the stock.
        @return Returns -1 if num*/
        
    Card* getCard (int index);
    /*< Returns a poinnter to the Card at the specified index.
        @return Returns nullptr if the index is greater than the number of Cards in this Hand. */
        
    Card* takeCard (int index);
    //< Returns and removes the Card at the specified index.
    
    Card* takeNextCard ();
    //< Returns and removes the next Card off of the top of the Deck.
    
    Card* takeRandomCard ();
    //< Returns and removes a random Card from the stack.

    bool isEmpty ();
    //< Returns true if there are no more cards in the deck.
    
    bool isVisible ();
    //< Returns if te stackIsVisible.
    
    void setVisiblity (bool visiblityState);
    //< Sets stackIsVisible to the new visiblityState.

    string tostring ();
    //< Returns a string representation of this Object.

private:

    int minNumCards,        //< The minimum number of cards in a CardStack.
        maxNumCards;        //< The number of cards in the cardstack.

    bool stackIsVisible;    //< Flag that represents if the CardStack is visible or not.

    Array<Card*> cards;     //< The stack (i.e. Array) of Card pointers.
};

}   //< KabukiCardGames
