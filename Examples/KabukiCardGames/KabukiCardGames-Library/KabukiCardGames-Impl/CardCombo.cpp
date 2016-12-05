/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Blackjack/CardCombo.cpp
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

#include "CardCombo.hpp"

namespace KabukiCardGames {

CardCombo::CardCombo (CardStack& copyStack, int thisJokerPipValue1, int thisJokerSuitValue1, int thisJokerPipValue2, int thisJokerSuitValue2, int thisAcesHigh) 
{
    setValues (thisJokerPipValue1, thisJokerSuitValue1, thisJokerPipValue2, thisJokerSuitValue2, thisAcesHigh);
}

void CardCombo::setValues (int thisJokerPipValue1, int thisJokerSuitValue1, int thisJokerPipValue2, int thisJokerSuitValue2, int thisAcesHigh)
{
    if (thisJokerPipValue1 < 1)
        thisJokerPipValue1 = 1;
    else if (thisJokerPipValue1 > 13)
        thisJokerPipValue1 = 13;
    
    if (thisJokerPipValue2 < 1)
        thisJokerPipValue2 = 1;
    else if (thisJokerPipValue2 > 13)
        thisJokerPipValue2 = 13;
    
    if (thisJokerSuitValue1 < 1)
        thisJokerSuitValue1 = 1;
    else if (thisJokerSuitValue1 > 4)
        thisJokerSuitValue1 = 4;
    
    if (thisJokerSuitValue2 < 1)
        thisJokerSuitValue2 = 1;
    else if (thisJokerSuitValue2 > 4)
        thisJokerSuitValue2 = 4;

    jokerPipValue1 = thisJokerPipValue1;
    jokerSuitValue1 = thisJokerSuitValue1;
    jokerValue2 = thisJokerPipValue2;
    jokerSuitValue2 = thisJokerSuitValue2;
    acesHigh = thisAcesHigh;
}

int CardCombo::getJokerPipValue1 ()
{
    return jokerPipValue1;
}

int CardCombo::setJokerPipValue1 (int newValue)
{
    if (newValue < 0)
        return -1;
    if (newValue > 13)
        return 1;
    jokerPipValue1 = newValue;
}

int CardCombo::getJokerSuitValue1 ()
{
    return jokerSuitValue1;
}

int CardCombo::setJokerSuitValue1 (int newValue)
{
    if (newValue < 0)
        return -1;
    if (newValue > 13)
        return 1;
    jokerSuitValue1 = newValue;
}

int CardCombo::getJokerPipValue2 ()
{
    return jokerPipValue2;
}

int CardCombo::setJokerPipValue2 (int newValue)
{
    if (newValue < 0)
        return -1;
    if (newValue > 13)
        return 1;
    jokerPipValue2 = newValue;
}

int CardCombo::getJokerSuitValue2 ()
{
    return jokerSuitValue2;
}

int CardCombo::setJokerSuitValue2 (int newValue)
{
    if (newValue < 0)
        return -1;
    if (newValue > 13)
        return 1;
    jokerSuitValue2 = newValue;
}

int CardCombo::getAcesHigh ()
{
    return acesHigh;
}

int CardCombo::setAcesHigh (int newValue)
{
    acesHigh = newValue;
}


#if DEBUG_MODE

#include "Deck.hpp"

class CardComboUnitTest : public UnitTest
{
    CardComboUnitTest () : UnitTest ("Testing Blackjack::CardCombo class...") {}

    void runTest ()
    {

    }
};
static CardComboUnitTest cardComboUnitTest;
#endif // DEBUG_MODE

}   //< KabukiCardGames
