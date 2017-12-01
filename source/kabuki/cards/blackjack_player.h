/** kabuki::cards
    @file       ~/source/kabuki/cards/blackjack/player.h
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

#ifndef KABUKI_BLACKJACK_PLAYER_H
#define KABUKI_BLACKJACK_PLAYER_H

#include "../../global.h"

#include "../Dealer.h"

namespace kabuki_cards
{
namespace Blackjack
{
class BlackjackPlayer
{
public:
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    /** Default Constructor. */
    BlackjackPlayer (String playerName = "You", int startPoints = 10, bool playerOrDealer);
    ~BlackjackPlayer ();                    //< Destructor.
    
    bool playOrPass (Dealer& dealer);       /*< AI function that determines if a player (usually the dealer) hits or holds. */
    void playHand ();                       //< Function that performs the logic of playing a hand.
};
}
}
#endif // KABUKI_CARDS__BLACKJACK__PLAYER_H_INCLUDED