/** kabuki::cards
    @file       ~/source/kabuki/cards/player.h
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

#ifndef HEADER_FOR_KABUKI_BLACKJACK_PLAYER
#define HEADER_FOR_KABUKI_BLACKJACK_PLAYER

#include "player.h"

namespace kabuki { namespace cards {

/** A player in a Blackjack game. */
class BlackjackPlayer : public Player {
    public:

    enum States {
        kStateWaitingToJoin = 0, //< State when player is waiting to join.
        kStatePlayingRound  = 1, //< State when player is playing normally.
        kStateHolding       = 2, //< State when player is holding.
        kStateOutOfGame     = 3  //< State when player has lost game.
    };

    /** Default Constructor. */
    BlackjackPlayer (id::User* user, CardStack& stock,
                     int start_points = 100, bool is_dealer = false);

    /** Destructor. */
    ~BlackjackPlayer ();

    /** Sets the state of the player. */
    const char* SetState (int state) override;

    /** Compares this hand to the other hand.
        @return Returns 0 if they are equal, 1 if this hand is greater than
        the other Hand, and -1 if the other Hand is greater than this
        Hand. */
    virtual int Compare (Hand& other);

    /** Returns the HighLowScore. */
    //data::Array<CardCombo> GetHandCombos ();

    /** Gets the hand score with the given ace value. */
    int GetScore (int ace_value);

    /** Returns the min score of this hand where Aces are worth 1. */
    int GetMinScore ();

    /** Returns the max score of this hand where Aces are worth 11. */
    int GetMaxScore ();

    /** Function returns if this hand is 21. */
    bool Is21 ();

    /** Function returns true if the hand is a bust.
        A hand is a bust if it is over 21 points. */
    bool IsBust ();

    /** Prints this object to the console. */
    void PrintHand ();

    /** New game logic handler. */
    virtual void NewGame ();

    /** New round logic handler.
        It's not really possible to predict what function parameters this
        this function will need so you will need to pass them into your
        sub-class object constructor. */
    virtual void BeginRound ();

    /** Attempts to take a card from the Deck for a player.
        @pre    The Deck must not be empty.
        @pre    The player must have a max hand score of 21. */
    virtual void PlayRound ();

    /** Processes end of round logic.
        It's not really possible to predict what function parameters this
        this function will need so you will need to pass them into your
        sub-class object constructor. */
    virtual void EndRound ();

    /** Processes end of game logic. */
    virtual void EndGame ();

    /** Checks to see if this hand wins compared to the given player's hand. */
    virtual bool HandWins (Hand& other);

    /** Returns true if the player is holding. */
    bool IsHolding ();

    /** Prints the round stats. */
    void PrintStats ();

    /** Prints the player details. */
    void Print ();

};
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_BLACKJACK_PLAYER
