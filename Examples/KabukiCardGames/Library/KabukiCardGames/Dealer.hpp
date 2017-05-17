/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/KabukiCardGames/Blackjack/Dealer.hpp
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

#include "Hand.hpp"
#include "Player.hpp"

namespace KabukiCardGames {

/** A class that represnts a dealer in a card game. 
    A dealer has the Deck of cards, but is not neccissaraly a player. A dealer needs to keep track of all of the Players.
*/
class Dealer
{
public:
    
    /** Constructor. */
    Dealer (int thisDeckSize, int thisStartingAnte, int thisMinBet, int thisMinNumCardsPerHand, int thisMaxNumCardsPerHand, int thisMaxNumPlayer) ;


    virtual ~Dealer ();
    //< Destructor.

    void setValues (int newStartingAnte, int newMinBet, int newMinNumCardsPerHand, int newMaxNumCardsPerHand, int newMaxNumPlayer);

    int getNumPlayers ();
    //< Returns the number of Players.

    int getMinNumCardsPerHand ();
    //< Returns the minNumCardsPerHand.
    
    int getMaxNumCardsPerHand ();
    //< Returns the maxNumCardsPerHand.

    int setMinNumCardsPerHand (int numCards);
    //< Sets the minNumCardsPerHand to the numCards.
    
    int setMaxNumCardsPerHand (int numCards);
    //< Sets the maxNumCardsPerHand to the numCards.

    void addPlayer (Player* newPlayer);
    //< Adds the newPlayer to the game.
    
    int removePlayer (string& playerName);
    /*< Removes a Player with the specified playerName from the game.
        @returns the index number of the player if the player is found. */
                                                    
    int removePlayer (int playerNumber);
    /*< Removes a Player with the specified index from the game.
        @return Returns 0 upon success.
        @return Returns -1 if playerNumber is < 0.
        @return Returns 1 if the playerNumber is > getNumPlayers ().*/
                                                    
    Player* getPlayer (int playerNumber);
    /*< Retruns a pointer to the player with the specified playerNumber.
        @return Returns nullptr if the playerNumber < 0 or > getNumPlayers (). */

    Deck& getPack ();
    //< Returns the address of the dealer's pack.
    
    CardStack& getStock ();
    //< Returns the address of the dealer's stock.

    int getPotTotal ();
    //< Returns the total number of points in the pot.
    
    void setPotTotal (int newPotTotal);
    //< Sets the potTotal.
    
    void addToPot (int pointsToAdd);
    /*< Adds the pointsToAdd to the potTotal.
        @pre    pointsToAdd > 0 */

    int getNumDecks ();
    //< Returns the numDecks.
    
    void setNumDecks (int newNumDecks);
    //< Sets the numDecks to the newNumDecks.

    int getDeckSize ();
    //< Returns the deckSize.
    
    void setDeckSize (int newMaxPlayers);
    //< Sets the deckSize.

    int getStartingAnte ();
    //< Returns the total number of points in the pot
    
    void setStartingAnte (int newAnte);
    //< Sets the potTotal.

    int getAnte ();
    //< Returns the total number of points in the pot.
    
    void setAnte (int newAnte);
    //< Sets the potTotal.

    int getMinBet ();
    //< Returns the current min bet.
    
    void setMinBet (int newValue);
    //< Sets the minBet to the newValue.

    int getMaxPlayers ();
    //< Returns the current min bet.
    
    void setMaxPlayers (int newMaxPlayers);
    //< Sets the minBet to the newValue.

    virtual void startNewGame ();
    //< Virtual function that shuffles the cards and gets ready to starts a new game.
    
    virtual void redeal ();
    //< Virtual function that shuffles the cards and gets ready for a new round.

    virtual string tostring ();
    //< Returns a string representation of this Object.

private:
    
    int numDecks,                               //< The number of decks in this game.
        deckSize,                               //< The size of the deck for this game.
        potTotal,                               //< The number of points in the pot.
        startingAnte,                           //< The starting ante for the game.
        ante,                                   //< The current ante for the game.
        minBet,                                 //< The minimum bet for this round.
        minNumCardsPerHand,                     //< The minimum number of Cards per Hand.
        maxNumCardsPerHand,                     //< The maximum number of Cards per Hand.
        maxNumPlayers,                          //< The maximum number of players in this game.
        currentPlayerNumber;                    //< The player number of the current player who is playing in this round.

    Deck pack;                                  /*< The main pack of Card objects; the Deck owns the Card objects.
                                                    A pack of Cards is the entire deck of cards. Note: can be a DoubleDeck (which you should make). */
    CardStack stock;                            /*< The stock of playing Card pointers.
                                                    The stock is the main pile of cards in a playing game. */
    OwnedArray<Player> players;                 //< The ownwed array of Player objects.
};

}   //< KabukiCardGames
