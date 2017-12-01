/** kabuki::cards
    @file       ~/source/kabuki/cards/blackjack/blackjack.cc
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
#include "BlackjackHand.h"

using namespace kabuki_cards::Blackjack;

BlackjackHand::BlackjackHand (CardStack& startingCards) :
    Hand (startingCards, 2, Deck::defaultNumCards)
{
}
   
Array<CardCombo> BlackjackHand::getHandCombos ()
{
    auto lowScore = BlackjackCardCombo (,
        highScore;

    scores.low = 0;     //< scores.low is the total of the cards with aces as 1
    scores.high = 0;    //< scores.high is the number of aces to add on an opt 10.

    for (int s=0; s < getNumCards (); ++s)
    {
        int currentValue = getCard(s)->getPipValue ();
         
        if (currentValue == 1) // First check if its an ace
        {
            ++scores.low;       //< Aces low means add 1 to the low total.
            scores.high += 10;  //< Aces high means add a 10 to the high total.
        }
        else if (currentValue >= 10) // Next check to see if its a Jack, Queen, or King because they are all 10 points.
        {
            scores.low += 10;
            scores.high += 10;
        }
        else // then its just number card so add it to both the low and high totals.
        {
            scores.low += currentValue;
            scores.high += currentValue;
        }
    }
      
    return scores;
}


int BlackjackHand::addCard (Card* card)
{
    if (getNumCards () + 1 > maxNumCards)
        return 1;
    nonVisibleCards.addCard (card);
}

String BlackjackHand::toString ()
{
    HighLowScore highLowScore = getHighLowScore ();
    
    return "BlackjackHand:\nHigh score: " + String (highLowScore.high) + " Low Score: " + String (highLowScore.low) + "\n" + CardStack::toString ();
}


#if DEBUG_MODE

#include "../Deck.h"

class BlackjackHandUnitTest : public UnitTest
{
    BlackjackHandUnitTest () : UnitTest ("Testing Blackjack::BlackjackHand class...") {}

    void runTest ()
    {
        logMessage ("Creating deck...");

        Deck stackedDeck (Deck::Culture Deck::french, Deck::acesHighOrLow, Deck::doesNotContainJokers);

        CardStack deck (stackedDeck);

        deck.shuffle ();

        logMessage ("Creating playerBlackjackHand");

        BlackjackHand playerBlackjackHand (deck.takeNextCard (), deck.takeNextCard ()),
            dealersBlackjackHand (deck.takeNextCard (), deck.takeNextCard ());

        logMessage ("Testing String Hard::toString ()...");
        logMessage (playerBlackjackHand.toString);
        logMessage (dealersBlackjackHand.toString);

        logMessage ("Creating BlackjackHands Player{Ace of Hearts, Ace of Clubs) Dealer{ King of Hearts, Nine of Diamonds } ...");
        playersBlackjackHand.setCards ();
    }
};
static CardStackUnitTest cardPileUnitTest;
#endif // DEBUG_MODE
