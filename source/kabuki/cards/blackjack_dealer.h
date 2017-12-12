/** kabuki::cards
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

#ifndef HEADER_FOR_KABUKI_CARDS_BLACKJACKDEADER
#define HEADER_FOR_KABUKI_CARDS_BLACKJACKDEADER

#include "dealer.h"

namespace kabuki { namespace cards {

/** A dealer in a card game.
    A dealer has the Deck of cards, but is not necessarily a player. A
    dealer needs to keep track of all of the Players. A BlackjackDealer is both
    a Player and a Dealer at the same time. */
class BlackjackDealer : public Dealer {
    public:
    
    enum {
        kNumDecks           = 1,      //< Number of decks for this game.
        kDeckSize           = 52,     //< Number of cards in the Deck.
        kStartingAnte       = 5,      //< Starting ante.
        kMinBet             = 1,      //< Min bet.
        kMinCardsPerHand    = 2,      //< Min cards in a hand.
        kMaxCardsPerHand    = Deck::kDefaultDeckSize, //< Max cards in a hand.
        kDefaultMinPlayers  = 1,      //< Default min num players.
        kDefaultMaxPlayers  = 10,     //< Default max num players.
        kKeyboardBufferSize = 79,     //< Size of the keyboard input buffer.
    };

    /** Default constructor. */
    BlackjackDealer (id::User* user, int64_t buy_in = Dealer::kDefaultAnte,
                     int64_t ante    = Dealer::kDefaultAnte,
                     int64_t min_bet = Dealer::kDefaultMinBet,
                     int min_players = kDefaultMinPlayers,
                     int max_players = kDefaultMaxPlayer);

    virtual ~BlackjackDealer ();

    /** Raises the ante by the given amount. */
    bool RaiseAnte (int64_t value);

    /** Gets the hand score with the given ace value. */
    int GetScore (int ace_value);

    /** Pure virtual new game logic handler. */
    virtual void RestartGame ();

    /** Processes beginning of round logic. */
    virtual void BeginRound ();

    /** Performs round logic. */
    virtual void PlayRound ();

    /** Processes beginning of round logic. */
    virtual void EndRound ();

    /** Processes beginning of round logic. */
    virtual void EndGame ();

    /** Compares this hand to the other.
        @return Returns 0 if the hands are equal, > 1 if the other hand beats
        this hand and < 0 if the other hand wins.. */
    virtual int Compare (Hand& hand);

    /** Returns true if this hand wins compared to the other one. */
    virtual bool Wins (Hand& hand);

    /** Adds a BlackjackPlayerAi to the . */
    virtual int AddAiPlayer (id::User* user);

    /** Prints the abridged player stats to the console. */
    virtual void PrintStats ();

    /** Prints the player to the console. */
    virtual void Print ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param text_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* HandleText (const char* text,
                                    const char* text_end);

};      //< class BlackjackDealer
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_CARDS_BLACKJACKDEADER
