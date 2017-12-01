/** kabuki::cards
    @file       ~/source/kabuki/cards/blackjack/blackjackCardCombo.cc
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
#include "BlackjackCardCombo.h"

using namespace kabuki_cards;

BlackjackCardCombo::BlackjackCardCombo (CardStack& copyStack, int acesHighLowORNA) :
    CardCombo (copyStack, acesHighLowORNA)
{
    setValues (0, 1, 0, 1, acesHighLowORNA);
}

int BlackjackCardCombo::getPointValue ()
{
    int total = 0;
    Card* currentCard;
    for (int i = 0; i < getNumCards (); ++i)
    {
        int currentPipValue = currentCard->getPipValue ();

        if (currentPipValue == 1) // If its an ace.
        {
            if (getAcesHigh ())
                total += 10;
            else
                total += 1;
        }
        else
            total += currentCard->getPointValue (); //< Then just add the point to to the score like normal.
    }
    return total;
}


#if DEBUG_MODE

#include "Deck.h"

class BlackjackCardComboUnitTest : public UnitTest
{
    BlackjackCardComboUnitTest () : UnitTest ("Testing Blackjack::BlackjackCardCombo class...") {}

    void runTest ()
    {

    }
};
static BlackjackCardComboUnitTest cardComboUnitTest;
#endif // DEBUG_MODE
