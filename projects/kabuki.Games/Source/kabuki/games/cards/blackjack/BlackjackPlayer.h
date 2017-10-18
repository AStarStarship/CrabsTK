/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/KabukiCardGames/Blackjack/BlackjackPlayer.hpp
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

#include "../../global.hpp"

#include "../Dealer.hpp"

namespace KabukiCardGames { namespace Blackjack {

class BlackjackPlayer
/*< A player in the game Blackjack. */
{
public:
    
    /** Default Constructor. */
    BlackjackPlayer (string playerName = "You", int startPoints = 10, bool playerOrDealer);
    
    ~BlackjackPlayer ();
    //< Destructor.
    
    bool playOrPass (Dealer& dealer);
    /*< AI function that determines if a player (usually the dealer) hits or holds. */
    
    void playHand ();
    //< Function that performs the logic of playing a hand.
};

}   //< Blackjack
}   //< KabukiCardGames
