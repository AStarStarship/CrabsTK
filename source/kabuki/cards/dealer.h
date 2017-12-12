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
    Dealer (id::User* user,
            int64_t buy_in      = kDefaultAnte,
            int64_t ante        = kDefaultAnte,
            int64_t min_bet     = kDefaultMinBet,
            int     min_players = kDefaultMinPlayers,
            int     max_players = kDefaultMaxPlayer,
            int     num_decks   = Deck::kDefaultNumDecks);
    
    /** Destructor. */
    virtual ~Dealer ();

    /** Gets the round_number_. */
    int GetRoundNumber ();

    /** Gets the number of Players. */
    int GetNumPlayers ();

    /** Creates and adds a mew player to the game from the given User. */
    virtual void AddPlayer (Player* player);
    
    /** Removes the Player with the specified handle from the players_.
        @returns the index number of the player if the player is found. */
    virtual int RemovePlayer (const char* handle);
                                                    
    /** Removes a Player with the specified index from the game.
        @return Returns 0 upon success.
        @return Returns -1 if player_number is < 0.
        @return Returns 1 if the player_number is > getNumPlayers ().*/
    virtual int RemovePlayer (int player_number);

    /** Gets the player_number_. */
    Player* GetPlayer ();
                                                    
    /** Gets a pointer to the player with the specified player_number.
        @return Returns nil if the player_number < 0 or > GetNumPlayers (). */
    Player* GetPlayer (int player_number);

    /** Gets the current_player_ */
    int GetCurrentPlayer ();

    /** Sets the current_player_ */
    bool SetCurrentPlayer (int session);

    /** Gets the address of the dealer's pack. */
    Deck& GetPack ();
    
    /** Gets the address of the dealer's stock. */
    CardStack& GetStock ();

    /** Gets the total number of points in the pot. */
    int64_t GetPotTotal ();
    
    /** Sets the pot_total_. */
    virtual void ResetPot ();
    
    /** Adds the pointsToAdd to the pot_total_.
        @pre    pointsToAdd > 0 */
    virtual void AddToPot (int64_t points);

    /** Gets the total number of points in the pot */
    int64_t GetBuyIn ();
    
    /** Sets the pot_total_. */
    void SetBuyIn (int64_t ante);

    /** Gets the total number of points in the pot. */
    int64_t GetAnte ();
    
    /** Sets the pot_total_ to the given value. */
    void SetAnte (int64_t value);

    /** Raises the ante_ by subtracting the given value. */
    void RaiseAnte (int64_t value);

    /** Gets the current min bet. */
    int64_t GetMinBet ();
    
    /** Sets the minBet to the value. */
    virtual const char* SetMinBet (int64_t value);

    /** Gets the current min_players_. */
    int GetMinPlayers ();

    /** Gets the current min bet. */
    int GetMaxPlayers ();
    
    /** Sets the player_number_. */
    bool SetPlayerNumber (int player_number);

    /** Draws a card from the stock_. */
    virtual Card* Draw ();

    /** Virtual function that shuffles the cards and gets ready for a new
        round. */
    virtual void Redeal ();

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
                this hand and < 0 if the other hand wins. */
    virtual int Compare (Hand& other) = 0;

    /** Gets true if this hand wins compared to the other one. */
    virtual bool Wins (Hand& other) = 0;

    /** Prints the abridged player stats to the console. */
    virtual void PrintStats () = 0;

    /** Prints this object to the stdout. */
    virtual void Print ();

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param text_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* HandleText (const char* text,
                                    const char* text_end);

    protected:
    
    int            round_number_,  //< Current round number.
                   min_players_,   //< Min players to start game.
                   max_players_,   //< Max allowed in game.
                   current_player_,//< Index of the player who's turn it is.
                   num_responses_; //< Number of responses.
    int64_t        pot_,           //< Number of points in the pot.
                   buy_in_,        //< Buy in price to start game.
                   ante_,          //< Current ante for the game.
                   min_bet_;       //< Min bet for this round.
    Deck           pack_;          //< Main pack of Card objects.
    CardStack      stock_;         //< Stock of playing Cards.
    std::vector<Player*> players_; //< Array of Player pointers.

};      //< class Dealer
}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_DEALER_H
