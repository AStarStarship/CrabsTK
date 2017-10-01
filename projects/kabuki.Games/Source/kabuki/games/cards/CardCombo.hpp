/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/KabukiCardGames/Blackjack/CardCombo.hpp
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

namespace KabukiCardGames {
    
/** Class that represents a CardStack point combination in a playing card game.
    There are two major things that will effect a combination's rank besides suit rank; the presence of jokers, and if aces are high or low.
*/
class CardCombo : public CardStack
{ 
public:

    CardCombo (CardStack& copyStack, int thisAcesHigh, int thisJokerPipValue1 = 0, int thisJokerSuitValue1 = 1, 
        int thisJokerPipValue2 = 0, int thisJokerSuitValue2 = 2);
    /*< Default constructor. */

    void setValues (int thisAcesHigh, int thisJokerPipValue1 = 0, int thisJokerSuitValue1 = 1, 
        int thisJokerPipValue2 = 0, int thisJokerSuitValue2 = 2);
    /*< Function sets values to the new values. */

    virtual int getPointValue () = 0;
    //< Function that returns the point value with the wild card and ace values.

    int getJokerPipValue1 ();
    //< Returns the jokerPipValue1.
    
    int setJokerPipValue1 (int newValue);
    //< Sets the jokerPipValue1 to the newValue.

    int getJokerSuitValue1 ();
    //< Returns the jokerSuitValue1.
    
    int setJokerSuitValue1 (int newValue);
    //< Sets the jokerSuitValue1 to the newValue.

    int getJokerPipValue2 ();
    //< Returns the jokerPipValue2.
    
    int setJokerPipValue2 (int newValue);
    //< Sets the jokerPipValue2 to the newValue.

    int getJokerSuitValue2 ();
    //< Returns the jokerSuitValue2.
    
    int setJokerSuitValue2 (int newValue);
    //< Sets the jokerSuitValue2 to the newValue.

    int getAcesHigh ();
    //< Returns the jokerPipValue1.
    
    int setAcesHigh (int newValue);
    //< Sets the jokerPipValue1 to the newValue.
    
private:

    int jokerPipValue1,                             //< The black joker wildcard pip value.
        jokerSuitValue1,                            //< The black joker wildcard suit value.
        jokerPipValue2,                             //< The red joker wildcard pip value.
        jokerSuitValue2,                            //< The red joker wildcard suit value.
        acesHigh;                                   //< Flag represents if aces are high (>1) or low (0), or either (<0).
};

}   //< KabukiCardGames
