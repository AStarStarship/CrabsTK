/** kabuki:cards
    @file    ~/source/kabuki/cards/dealer.h
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

#ifndef KABUKI_CARDS_DEALER_H
#define KABUKI_CARDS_DEALER_H

#include "hand.h"
#include "player.h"

namespace kabuki { namespace cards {

/** A dealer in a card game.
    A dealer has the Deck of cards, but is not necessarily a player. A dealer
    needs to keep track of all of the Players and deal them cards. */
class KABUKI Dealer {
    public:
    
    /** Constructor. */
    Dealer (int ante, int min_bet, int min_cards_per_hand, 
            int max_cards_per_hand, int max_num_player, int deck_size);
    
    /** Destructor. */
    virtual ~Dealer ();

    /** Returns the number of Players. */
    int GetNumPlayers ();

    /** Returns the minNumCardsPerHand. */
    int GetMinNumCardsPerHand ();
    
    /** Returns the maxNumCardsPerHand. */
    int GetMaxNumCardsPerHand ();

    /** Sets the minNumCardsPerHand to the num_cards. */
    bool SetMinNumCardsPerHand (int num_cards);
    
    /** Sets the maxNumCardsPerHand to the num_cards. */
    bool SetMaxNumCardsPerHand (int num_cards);

    void AddPlayer (Player* newPlayer);
    /** Adds the newPlayer to the game.
    
    /** Removes a Player with the specified playerName from the game.
        @returns the index number of the player if the player is found. */
    int RemovePlayer (const char* playerName);
                                                    
    /** Removes a Player with the specified index from the game.
        @return Returns 0 upon success.
        @return Returns -1 if playerNumber is < 0.
        @return Returns 1 if the playerNumber is > getNumPlayers ().*/
    int RemovePlayer (int playerNumber);
                                                    
    /** Returns a pointer to the player with the specified playerNumber.
        @return Returns nullptr if the playerNumber < 0 or > getNumPlayers (). */
    Player* GetPlayer (int playerNumber);

    /** Returns the address of the dealer's pack. */
    Deck& GetPack ();
    
    /** Returns the address of the dealer's stock. */
    CardStack& GetStock ();

    /** Returns the total number of points in the pot. */
    int GetPotTotal ();
    
    /** Sets the pot_total_. */
    void SetPotTotal (int new_total);
    
    /** Adds the pointsToAdd to the pot_total_.
        @pre    pointsToAdd > 0 */
    void AddToPot (int points);

    /** Returns the num_decks_. */
    int GetNumDecks ();
    
    /** Sets the numDecks to the new_num_decks. */
    bool SetNumDecks (int new_num_decks);

    /** Returns the deckSize. */
    int GetDeckSize ();
    
    /** Sets the deckSize.
        @param num_decks The number of decks in use. */
    bool SetDeckSize (int num_decks);

    /** Returns the total number of points in the pot */
    int GetStartingAnte ();
    
    /** Sets the pot_total_. */
    void SetStartingAnte (int ante);

    /** Returns the total number of points in the pot. */
    int GetAnte ();
    
    /** Sets the pot_total_. */
    void SetAnte (int ante);

    /** Returns the current min bet. */
    int GetMinBet ();
    
    /** Sets the minBet to the value. */
    void SetMinBet (int value);

    /** Returns the current min bet. */
    int GetMaxPlayers ();
    
    /** Sets the minBet to the value. */
    void SetMaxPlayers (int newMaxPlayers);

    /** Virtual function that shuffles the cards and gets ready to starts a
        new game. */
    virtual void StartNewGame ();
    
    /** Virtual function that shuffles the cards and gets ready for a new
        round. */
    virtual void Redeal ();

    /** Deals the given number of cards into the specified stack. */
    void Deal (CardStack& stack, int num_cards = 1);

    /** Prints this object to the stdout. */
    virtual void PrintDealer ();

    private:

    /** Sets all of the dealer's values in one call. */
    void SetValues (int ante, int min_bet, int min_cards_per_hand,
                    int max_cards_per_hand, int max_num_player, int deck_size);
    
    int  num_decks_,           //< Number of decks in this game.
         deck_size_,           //< Size of the deck for this game.
         pot_total_,           //< Number of points in the pot.
         starting_ante_,       //< Starting ante for the game.
         ante_,                //< Current ante for the game.
         min_bet_,             //< Min bet for this round.
         min_cards_per_hand_,  //< Min number of Cards per Hand.
         max_cards_per_hand_,  //< Max number of Cards per Hand.
         max_players_,         //< Max number of players in this game.
         player_number_;       //< Index of current player who's turn it is.
    Deck pack_;                //< Main pack of Card objects.
    CardStack stock_;          //< Stock of playing Cards.
    data::Array<Player*> players_;  //< Array of Player pointers.

};      //< class Dealer
}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_DEALER_H
