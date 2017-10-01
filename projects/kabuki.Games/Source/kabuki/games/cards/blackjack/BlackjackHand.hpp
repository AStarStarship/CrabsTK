/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/KabukiCardGames/Blackjack/BlackjackHand.hpp
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

#include "../Hand.hpp"
#include "../CardStack.hpp"

namespace KabukiCardGames { namespace Blackjack {
    
class BlackjackHand : public Hand
/*< Class that represents a Hand in Blackjack.
    A blackjack hand has a minmum of 2 cards.
*/
{ 
public:

    BlackjackHand (CardStack& stock);
    /*< Constructor.
        The game of blackjack starts with each player having two cards. */
        
    int compare (const Hand& other);
    /*< Compares this hand to the other hand.
        @return Returns 0 if they are equal, 1 if this hand is greater than 
            the other Hand, and -1 if the other Hand is greater than this Hand. */
            
    Array<CardCombo> getHandCombos ();
    //< Returns the HighLowScore.

    int addCard (Card* newCard);
    //< Adds the new Card to this Hand.

    string tostring ();
    //< Resturns a string representation of this object.
};

}   //< Blackjack
}   //< KabukiCardGames
