/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/CardStack.cpp
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
#include "CardStack.hpp"

namespace KabukiCardGames {

const CardStack CardStack::empty = CardStack ();

CardStack::CardStack () :
    minNumCards (0),
    maxNumCards (Deck::defaultNumCardsWithJokers),
    stackIsVisible (true),
    cards (Array<Card*> ())
{
    // Nothing to do here. ({:-)
}

CardStack::CardStack (CardStack& cardStack, int thisMinNumCards = 0, int thisMaxNumCards, bool thisStackIsVisible) :
    minNumCards (thisMinNumCards < 0 ? 0 : thisMinNumCards),
    maxNumCards (thisMaxNumCards < 1 ? 1 : thisMaxNumCards),
    stackIsVisible (thisStackIsVisible)
{
    // Nothing to do here. ({:-)
}

CardStack::CardStack (Deck& theDeck) :
    maxNumCards (theDeck.getNumCards()),
    stackIsVisible (false)
{
    for (int i = 0; i < theDeck.getNumCards (); ++i)
        addCard (theDeck.getCard (i));
}

CardStack::CardStack (const CardStack& other) :
    maxNumCards (other.maxNumCards),
    cards (other.cards),
    stackIsVisible (other.stackIsVisible)
{
    // Nothing to do here.
}

CardStack& CardStack::operator= (const CardStack& other)
{
    minNumCards = other.minNumCards;
    maxNumCards = other.maxNumCards;

    stackIsVisible = other.stackIsVisible;

    cards = other.cards;
}

int CardStack::compare (CardStack& other)
{
    int pointValue = getPointValue (),
        otherPointValue = other.getPointValue ();

    if (pointValue > otherPointValue)
        return 1;
    if (pointValue < otherPointValue)
        return -1;
    return 0;
}

int CardStack::getPointValue ()
{
    int total = 0;
    for (auto currentCard : cards)
        total += currentCard->getPointValue ();
    return total;
}

void CardStack::shuffle ()
{
    // Create an identical copy of this CardStack.
    CardStack unshuffledPile = CardStack (*this);

    cards.clear (); //< Delete all of the cards in this CardStack

    // Then for each Card in the CardStack, 
    for (int i = 0; i < cards.size (); ++i)
        cards.add (unshuffledPile.takeRandomCard ());
}

int CardStack::getNumCards ()
{
    return cards.size ();
}

int CardStack::getMinNumCards ()
{
    return minNumCards;
}

int CardStack::getMaxNumCards ()
{
    return maxNumCards;
}

int CardStack::addCard (Card* newCard)
{
    if (getNumCards () + 1 > maxNumCards)
        return 2;

    if (maxNumCards > cards.size () + 1) // Then we are a the max card number limit.
        return 1;

    cards.add(newCard);
}

int CardStack::insertCard (Card* newCard, int index)
{
    if (index < 0)
        return -1;

    if (index >= getNumCards ())
        return 1;

    if (getNumCards () + 1 > maxNumCards)
        return 2;

    cards.insert (index, newCard);
}

int CardStack::addCards (CardStack& newCards)
{
    if (getNumCards () + newCards.getNumCards () > maxNumCards)
        return 1;

    cards.addArray (newCards.cards);
}

int CardStack::drawCards (CardStack& stackToDrawFrom, int numCardsToTake)
{
    if (numCardsToTake < 0) //< Remember the @pre thing???
        return -1;          //< And the @return thing??? Then help you write code better and faster!

    if (numCardsToTake > stackToDrawFrom.getNumCards ())
        return 1;

    if (getNumCards () + numCardsToTake > maxNumCards)
        return 2;

    for (int i = 0; i < numCardsToTake; ++i)
        addCard (stackToDrawFrom.takeNextCard ());

    return 0;
}

bool CardStack::removeCard (Card* thisCard)
{
    if (!cards.contains (thisCard))
        return false;
    cards.removeFirstMatchingValue (thisCard);
    return true;
}

int CardStack::setCards (CardStack& stack, int numCards)
{
    if (numCards < 0)
        return -1;

    if (numCards > stack.getNumCards ())
        return 1;

    if (numCards < minNumCards)
        return 2;

    if (numCards > maxNumCards)
        return 3;

    cards = Array<Card*> ();

    for (int i = 0; i < numCards; ++i)
        cards.add (stack.getCard (i));
}

int CardStack::takeCards (CardStack& stack, int numCards)
{
    if (numCards < 0)
        return -1;

    if (numCards > stack.getNumCards ())    //< There aren't enough cards on the stack.
        return 1;

    if (numCards < minNumCards)             //< The minmum number of cards.
        return 2;

    if (numCards > maxNumCards)
        return 3;

    cards = Array<Card*> ();

    for (int i = 0; i < numCards; ++i)
        cards.add (stack.takeNextCard ());
}

Card* CardStack::getCard (int index)
{
    int numCards = cards.size ();

    if (numCards == 0 || index > numCards)
        return nullptr;

    return cards[index];
}

Card* CardStack::takeCard (int index)
{
    int numCards = cards.size ();

    if (numCards <= 0 || index >= numCards) // Arrays start at 0, so if we used cards[numCards], it would be out of bounds.
        return nullptr;

    Card* returnCard = cards[index];
    cards.remove (index);
    return returnCard;
}

Card* CardStack::takeNextCard ()
{
    if (cards.size () == 0) // Then there are no cards in the cards.
        return nullptr;

    int lastIndex = cards.size ()-1; // Golden rule: If you use a variable in a function more than once, store a local copy to get a performance increase.
    Card* nextCard = cards[lastIndex];
    cards.remove (lastIndex);
    return nextCard;
}

Card* CardStack::takeRandomCard ()
{
    if (cards.size () == 0) // Then there are no cards in the cards.
        return nullptr;

    Random generator = Random ();

    int randomIndex = generator.nextInt (cards.size ());
    Card *returnCard = cards[randomIndex];
    cards.remove (randomIndex);
    return returnCard;
}

bool CardStack::isEmpty ()
{
    return cards.size () == 0;
}

bool CardStack::isVisible ()
{
    return stackIsVisible;
}

void CardStack::setVisiblity (bool visiblityState)
{
    stackIsVisible = visiblityState;
}

string CardStack::tostring ()
{
    string returnstring = "";

    for (int i = 0; i < cards.size (); ++i)
        returnstring += string (i) + ".) " + cards[i]->tostring () + "\n";
}

#if DEBUG_MODE

class CardStackUnitTest : public UnitTest
{
    CardStackUnitTest () : UnitTest ("Testing CardStack class...") {}

    void runTest ()
    {
        logMessage ("Creating testDeck...");

        Deck testDeck = Deck ();

        logMessage ("Creating testPile from testDeck...");

        CardStack testPile = CardStack (testDeck);

        logMessage (testPile.tostring ());

        logMessage ("Testing CardStack::shuffle()...");

        testPile.shuffle ();

        logMessage (testPile.tostring ());
    }
};
static CardStackUnitTest cardPileUnitTest;
#endif // DEBUG_MODE

}   //< KabukiCardGames
