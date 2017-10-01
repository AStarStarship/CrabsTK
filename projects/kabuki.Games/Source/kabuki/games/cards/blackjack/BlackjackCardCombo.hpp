/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/KabukiCardGames/Blackjack/BlackjackCardCombo.hpp
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

#include "../CardCombo.hpp"

namespace KabukiCardGames { namespace Blackjack {
/** Class that represents a CardCombo in a game.
    There are two major things that will effect a combination's rank besides suit rank; the presence of jokers, and if aces are high or low.
*/
class BlackjackCardCombo : public CardCombo
{ 
public:

    /** Default constructor.
    */
    BlackjackCardCombo (CardStack& copyStack, int acesHighLowORNA);

    int getPointValue ();                           //< Function that returns the point value based on the ace value.
};

}   //< Blackjack
}   //< KabukiCardGames
