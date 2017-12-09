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
class KABUKI Dealer : public Player {
    public:

    enum {
        kDefaultAnte       = 10, //< Default ante.
        kDefaultMinBet     = 10, //< Default min bet per round.
        kDefaultMinPlayers = 2,  //< Default min players.
        kDefaultMaxPlayer  = 10, //< Default max players.
    };
    
    /** Constructor. */
    Dealer (id::User* user, int32_t buy_in = kDefaultAnte, int32_t ante = kDefaultAnte,
            int32_t min_bet     = kDefaultMinBet,
            int32_t min_players = kDefaultMinPlayers,
            int32_t max_players = kDefaultMaxPlayer,
            int32_t num_decks   = Deck::kDefaultNumDecks);
    
    /** Destructor. */
    virtual ~Dealer ();

    /** Gets the round_number_. */
    int32_t GetRoundNumber ();

    /** Gets the number of Players. */
    int32_t GetNumPlayers ();

    /** Creates and adds a mew player to the game from the given User. */
    virtual void AddPlayer (Player* player);
    
    /** Removes the Player with the specified handle from the players_.
        @returns the index number of the player if the player is found. */
    virtual int32_t RemovePlayer (const char* handle);
                                                    
    /** Removes a Player with the specified index from the game.
        @return Returns 0 upon success.
        @return Returns -1 if player_number is < 0.
        @return Returns 1 if the player_number is > getNumPlayers ().*/
    virtual int32_t RemovePlayer (int32_t player_number);
                                                    
    /** Gets a pointer to the player with the specified player_number.
        @return Returns nil if the player_number < 0 or > GetNumPlayers (). */
    Player* GetPlayer (int32_t player_number);

    /** Gets the current_player_ */
    int32_t GetCurrentPlayer ();

    /** Sets the current_player_ */
    bool SetCurrentPlayer (int32_t session);

    /** Gets the address of the dealer's pack. */
    Deck& GetPack ();
    
    /** Gets the address of the dealer's stock. */
    CardStack& GetStock ();

    /** Gets the total number of points in the pot. */
    int32_t GetPotTotal ();
    
    /** Sets the pot_total_. */
    virtual void SetPotTotal (int32_t new_total);
    
    /** Adds the pointsToAdd to the pot_total_.
        @pre    pointsToAdd > 0 */
    virtual void AddToPot (int32_t points);

    /** Gets the total number of points in the pot */
    int32_t GetBuyIn ();
    
    /** Sets the pot_total_. */
    void SetBuyIn (int32_t ante);

    /** Gets the total number of points in the pot. */
    int32_t GetAnte ();
    
    /** Sets the pot_total_. */
    void SetAnte (int32_t ante);

    /** Gets the current min bet. */
    int32_t GetMinBet ();
    
    /** Sets the minBet to the value. */
    virtual const char* SetMinBet (int32_t value);

    /** Gets the current min_players_. */
    int32_t GetMinPlayers ();

    /** Gets the current min bet. */
    int32_t GetMaxPlayers ();

    /** Gets the player_number_. */
    int32_t GetPlayerNumber ();
    
    /** Sets the player_number_. */
    bool SetPlayerNumber (int32_t player_number);

    /** Virtual function that shuffles the cards and gets ready for a new
    round. */
    virtual void Redeal ();

    /** Draws a card from the stock_. */
    virtual Card* Draw ();

    /** Pure virtual new game logic handler. */
    virtual void RestartGame ();

    /** Processes beginning of round logic. */
    virtual void BeginRound () = 0;

    /** Performs round logic. */
    virtual void PlayRound () = 0;

    /** Processes beginning of round logic. */
    virtual void EndRound () = 0;

    /** Processes beginning of round logic. */
    virtual void EndGame () = 0;

    /** Compares this hand to the other.
        @return Returns 0 if the hands are equal, > 1 if the other hand beats
                this hand and < 0 if the other hand wins.. */
    virtual int32_t Compare (Hand& other) = 0;

    /** Gets true if this hand wins compared to the other one. */
    virtual bool Wins (Hand& other) = 0;

    /** Prints the abridged player stats to the console. */
    virtual void PrintStats () = 0;

    /** Prints this object to the stdout. */
    virtual void Print ();

    protected:
    
    int32_t        round_number_,  //< Current round number.
                   pot_,           //< Number of points in the pot.
                   buy_in_,        //< Buy in price to start game.
                   ante_,          //< Current ante for the game.
                   min_bet_,       //< Min bet for this round.
                   min_players_,   //< Min players to start game.
                   max_players_,   //< Max allowed in game.
                   current_player_,//< Index of the player who's turn it is.
                   num_responses_; //< Number of responses.
    Deck           pack_;          //< Main pack of Card objects.
    CardStack      stock_;         //< Stock of playing Cards.
    Array<Player*> players_;       //< Array of Player pointers.

};      //< class Dealer
}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_DEALER_H
