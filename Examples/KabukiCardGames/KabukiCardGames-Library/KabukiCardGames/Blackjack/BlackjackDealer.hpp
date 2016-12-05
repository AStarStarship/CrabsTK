/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Blackjack/Dealer.hpp
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

#include "../Deck.hpp"
#include "../Dealer.hpp"
#include "../Player.hpp"

namespace KabukiCardGames { namespace Blackjack {
    
/** A class that represnts a dealer in a card game. 
    A dealer has the Deck of cards, but is not neccissaraly a player. A dealer needs to keep track of all of the Players.
    A BlackjackDealer is both a Player and a Dealer at the same time.
*/
class BlackjackDealer: public Player, public Dealer
{
public:
    
    enum {
        numDecks = 1,                                       //< The number of decks for this game.
        deckSize = 52,                                      //< The number of cards in the Deck.
        startingPoints = 999999,                            //< The dealers starting points.
        startingAnte = 5,                                   //< The starting ante.
        minBet = 1,                                         //< The minimum bet.
        minNumCardsPerHand = 2,                             //< The minimum number of cards in a hand.
        maxNumCardsPerHand = Deck::defaultNumCards,         //< The maximum number of cards in a hnd.
        maxNumPlayer = 2                                    //< The maximum number of players.
    };

    BlackjackDealer ();
    /*< Constructor. 
        This is why we like polymorphism. Notice all we have is a single constructor here, because all of our code we are recycling from the Dealer class.*/
};
}   //< Blackjack
}   //< KabukiCardGames