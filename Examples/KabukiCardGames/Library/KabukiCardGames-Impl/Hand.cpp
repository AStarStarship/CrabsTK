/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Blackjack/Hand.cpp
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
#include "Hand.hpp"

namespace KabukiCardGames {

Hand::Hand (CardStack& stock, int minCards, int maxNumCards) :
    minNumCards (boundInt1(minCards)),
    maxNumCards (boundInt1(maxNumCards)),
    visibleCards (CardStack ()),
    nonVisibleCards (CardStack ())
{
}

Hand& Hand::operator= (const Hand& other)
{
    minNumCards = other.minNumCards;
    maxNumCards = other.maxNumCards;

    visibleCards = other.visibleCards;
    nonVisibleCards = other.nonVisibleCards;
}

int Hand::getMinNumCards ()
{
    return minNumCards;
}

int Hand::setMinCards (int newNumCards)
{
    if (newNumCards < 0)
        return newNumCards;
    minNumCards = newNumCards;
    return 0;
}

int Hand::getMaxNumCards ()
{
    return maxNumCards;
}

int Hand::getNumCards ()
{
    return visibleCards.size () + nonVisibleCards.size ();
}

int Hand::setMaxCards (int newNumCards)
{
    if (newNumCards < 0)
        return 1;
    minNumCards = newNumCards;
    return 0;
}

int Hand::drawCard (CardStack& stack)
{
    return addCard (stack.takeCard ());
}

CardStack Hand::getCards ()
{
    auto returnStack = CardStack ();

    return returnStack;
}

CardStack& Hand::getVisibleCards ()
{
    return visibleCards;
}

CardStack& Hand::getNonVisibleCards ()
{
    return nonVisibleCards;
}

string Hand::tostring ()
{
    string returnstring = "Cards:\n" + dashedLnBreak + visibleCards.tostring ();

    if (nonVisiblePile.getNumCards () > 0)
        returnstring += "\n\nNon-visible Cards:\n" + nonVisibleCards.tostring ();
}


#if DEBUG_MODE

#include "Deck.hpp"

class HandUnitTest : public UnitTest
{
    HandUnitTest () : UnitTest ("Testing Blackjack::Hand class...") {}

    void runTest ()
    {
        logMessage ("Creating deck...");

        Deck pack (Deck::Culture Deck::french, Deck::acesHighOrLow, Deck::doesNotContainJokers);

        CardStack deck = CardStack (pack);

        deck.shuffle ();

        logMessage ("Creating playerHand");

        Hand playerHand (deck.takeNextCard (), deck.takeNextCard ()),
            dealersHand (deck.takeNextCard (), deck.takeNextCard ());

        logMessage ("Testing string Hard::tostring ()...");
        logMessage (playerHand.tostring);
        logMessage (dealersHand.tostring);

        logMessage ("Creating Hands Player{Ace of Hearts, Ace of Clubs) Dealer{ King of Hearts, Nine of Diamonds } ...");
        playersHand.setCards ();
    }
};
static HandUnitTest cardPileUnitTest;
#endif // DEBUG_MODE

}   //< KabukiCardGames
