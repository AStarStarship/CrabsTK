/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Blackjack/Dealer.cpp
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

#include "Dealer.hpp"

namespace KabukiCardGames {

Dealer::Dealer (int thisStartingAnte, int thisMinBet, int thisMinNumCardsPerHand, int thisMaxNumCardsPerHand, int thisMaxNumPlayer) :
    pack (Deck ()),
    stock (pack),
    players (OwnedArray<Player>())
{
    setValues (thisStartingAnte, thisMinBet, thisMinNumCardsPerHand, thisMaxNumCardsPerHand, thisMaxNumPlayer);
}

Dealer::~Dealer ()
{
    // Nothing to do here :-)
}

void Dealer::setValues (int newStartingAnte, int newMinBet, int newMinNumCardsPerHand, int newMaxNumCardsPerHand, int newMaxNumPlayer)
{    
    if (newStartingAnte < 0)
        newStartingAnte = 0;

    if (newMinBet < 1)
        newMinBet = 1;

    if (newMaxNumCardsPerHand < 1)
        newMaxNumCardsPerHand = 1;

    if (newMaxNumCardsPerHand < 1)
        newMaxNumCardsPerHand = 1;

    if (newMaxNumPlayer < 1)
        newMaxNumPlayer = 1;

    potTotal = 0;
    startingAnte = newStartingAnte;
    ante = ante = newStartingAnte;
    minBet = newMinBet;
    minNumCardsPerHand = newMinNumCardsPerHand;
    maxNumCardsPerHand = newMaxNumCardsPerHand;
    currentPlayerNumber = newMaxNumPlayer;
}

int Dealer::getNumPlayers ()
{
    return players.size ();
}

int Dealer::getMinNumCardsPerHand ()
{
    return minNumCardsPerHand;
}

int Dealer::getDeckSize ()
{
    return deckSize;
}

int Dealer::setDeckSize (int newNumCards)
{
    if (newNumCards < 1)
        return -1;
    deckSize = newNumCards;
}

int Dealer::getNumDecks ()
{
    return numDecks;
}

int Dealer::setNumDecks (int newNumDecks)
{
    if (newNumCards < 1)
        return -1;
    numDecks = newNumDecks;

    pack = Deck ()
}

int Dealer::getMaxNumCardsPerHand ()
{
    return maxNumCardsPerHand;
}

int Dealer::setMinNumCardsPerHand (int numCards)
{
    if (numCards < 1)
        return -1;
    minNumCardsPerHand = numCards;
}

int Dealer::getMaxNumCardsPerHand ()
{
    return maxNumCardsPerHand;
}

int Dealer::setMinNumCardsPerHand (int numCards)
{
    if (numCards < 1)
        return -1;
    minNumCardsPerHand = numCards;
}

int Dealer::setMaxNumCardsPerHand (int numCards)
{
    if (numCards < minNumCardsPerHand)
        return -1;
    maxNumCardsPerHand = numCards;
}

void Dealer::addPlayer (Player* newPlayer)
{
    players.add (newPlayer);
}

int Dealer::removePlayer (string& playerName)
{
    for (int i = 0; i < players.size (); ++i)
    {
        if (players[i]->getName () == playerName)
        {
            players.remove (i);
            return i;
        }
    }
    return 0;
}

int Dealer::removePlayer (int playerNumber)
{
    // Note: Arrays in C++ always start at element 0 (ie.e players[0])
    if (playerNumber < 0)
        return -1;

    if (playerNumber >= players.size ())
        return 1;

    players.remove (playerNumber);
    return true;
}

Player* Dealer::getPlayer (int playerNumber)
{
    if (playerNumber < 0 || playerNumber > players.size ())
        return nullptr;

    return players[playerNumber];
}

Deck& Dealer::getPack ()
{
    return pack;
}

CardStack& Dealer::getStock ()
{
    return stock;
}

int Dealer::getPotTotal ()
{
    return potTotal;
}

void Dealer::setPotTotal (int newPotTotal)
{
    if (newPotTotal < 0)
        return;
    potTotal = newPotTotal;
}
void Dealer::addToPot (int pointsToAdd)
{
    if (pointsToAdd < 0)
        return;
    potTotal += pointsToAdd;
}

int Dealer::getStartingAnte ()
{
    return startingAnte;
}

void Dealer::setStartingAnte (int newAnte)
{
    if (newAnte < 0)
        return;
    startingAnte = newAnte;
}

int Dealer::getAnte ()
{
    return ante;
}

void Dealer::setAnte (int newAnte)
{
    if (newAnte < 0)
        return;
    ante = newAnte;
}

int Dealer::getMinBet ()
{
    return minBet;
}

void Dealer::setMinBet (int newValue)
{
    if (newValue < 0)
        return;
    minBet = newValue;
}
int Dealer::getMaxPlayers ()
{
    return maxNumPlayers;
}

void Dealer::setMaxPlayers (int newMaxPlayers)
{
    if (newMaxPlayers < 0)
        return;
    maxNumPlayers = newMaxPlayers;
}

void Dealer::startNewGame ()
{
    stock = CardStack (pack);
    stock.shuffle ();
    redeal ();
}

void Dealer::redeal ()
{
    for (Player* currentPlayer : players)
        currentPlayer->dealHand (Hand (stock, 2, Deck::defaultNumCardsWithJokers));
}

string Dealer::tostring ()
{
    return "Dealer:\nPot Total: " + string (potTotal) + "\n";
}

}   //< KabukiCardGames
