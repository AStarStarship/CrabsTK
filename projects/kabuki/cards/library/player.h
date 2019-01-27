/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/player.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_KABUKI_ARCADE_CARDS_PLAYER
#define HEADER_FOR_KABUKI_ARCADE_CARDS_PLAYER

#include "hand.h"
#include "card_stack.h"

namespace kabuki { namespace cards {

/** An abstract player in an abstract card game.
*/
class Player : public _::Operand {
    public:

    typedef enum States {
        kStateWaitingForTurn = 0, //< Player waiting for turn state.
        kStatePlayingTurn    = 1, //< Player playing turn state.
        kStateBetting        = 2, //< Player betting state.
    } State;

    /** Default Constructor. */
    Player (id::User* user = nullptr, BOL is_dealer = false);
    
    /** Virtual destructor. */
    virtual ~Player ();

    /** Gets the user attached to this Player. */
    id::User* GetUser ();

    /** Gets the user's handle. */
    const CH1* GetHandle ();

    /** Gets is_turn_. */
    BOL IsTurn ();

    /** Sets is_turn_. */
    virtual void SetIsTurn (BOL is_turn);

    /** Gets is_dealer_. */
    BOL IsDealer ();

    /** Sets is_dealer_. */
    virtual void SetIsDealer (BOL is_dealer);

    /** Gets this Player's state. */
    SI4 GetState ();

    /** Attempts to set the state. 
        @return Returns nil upon success and an error string upon
                failure. */
    virtual const CH1* SetState (SI4 state);

    /** Returns the number of wins. */
    SI4 GetNumWins ();

    /** Returns the number of wins. */
    BOL SetNumWins (SI4 num_wins);

    /** Adds a win to the players numWins. */
    void AddWin ();

    /** Returns the player's hand. */
    Hand& GetHand ();

    /** Pushes a card onto the visible_cards_.
        @return Returns -1 upon failure. */
    SI4 TakeCardUp (Card* card);

    /** Pushes a card onto the hidden_cards_.
        @return Returns -1 upon failure. */
    SI4 TakeCardDown (Card* card);
                   
    /** Resets the numWins to 0. */
    void ResetWins ();

    /** Pure virtual new game logic handler. */
    virtual void RestartGame () = 0;

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
    virtual SI4 Compare (Hand& hand) = 0;

    /** Returns true if this hand wins compared to the other one. */
    virtual BOL Wins (Hand& hand) = 0;

    /** Prints the abridged player stats to the console. */
    virtual _::Text& PrintStats (_::Text& txt) = 0;

    /** Prints the player to the console. */
    virtual _::Text& Print (_::Text& txt = _::Text ()) = 0;
    
    /** Abstract Script Operation(s).
        @param index The index of the expression.
        @param expr  The Expression to read and write from.
        @return      Returns null upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const _::Operation* Star (uint index, _::Expression* expr) = 0;

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const CH1* Sudo (const CH1* text, const CH1* strand_end) = 0;

    protected:

    id::User * user_;       //< User attached to this Player.
    BOL       is_turn_,    //< Flags if it's this player's turn.
               is_dealer_;  //< Flags if this player is the dealer or not.
    SI4        state_,      //< The state of the player.
               num_wins_;   //< Total number of wins.
    Hand       hand_;       //< Player's Hand.
};

}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_CARDS_PLAYER
