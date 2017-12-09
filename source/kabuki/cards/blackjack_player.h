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

#ifndef HEADER_FOR_KABUKI_BLACKJACKPLAYER
#define HEADER_FOR_KABUKI_BLACKJACKPLAYER

#include "player.h"

namespace kabuki { namespace cards {

/** Calculates the blackjack store of the given hand and ace_value.
    @param hand The hand.
    @param ace_value Use value of either 1 or 13.
    @return Returns the score. */
KABUKI int BlackjackScore (Hand& hand, int ace_value);

/** Returns the min score of this hand where Aces are worth 1. */
KABUKI int BlackjackMinScore (Hand& hand);

/** Returns the max score of this hand where Aces are worth 11. */
KABUKI int BlackjackMaxScore (Hand& hand);

/** Compares the two Blackjack hands. */
KABUKI int BlackjackCompareHands (Hand& a, Hand& b);

/** A player in a Blackjack game. */
class BlackjackPlayer : public Player {
    public:

    enum States {
        kStateObserving     = 0, //< State when player is waiting to join.
        kStatePlayingRound  = 1, //< State when player is playing normally.
        kStateMyTurn        = 2, //< State when it's this player's turn.
        kStateHolding       = 3, //< State when player is holding.
        kStateOutOfGame     = 4  //< State when player has lost game.
    };

    /** Default Constructor. */
    BlackjackPlayer (id::User* user, CardStack& stock,
                     bool is_dealer = false);

    /** Destructor. */
    virtual ~BlackjackPlayer ();

    /** Sets the state of the player. */
    const char* SetState (int state) override;

    /** Handles hit signal. */
    void Hit ();

    /** Handles hold signal. */
    void Hold ();

    /** Returns the HighLowScore. */
    //Array<CardCombo> GetHandCombos ();

    /** Returns true if the player is holding. */
    virtual bool IsHolding ();

    /** Function returns if this hand is 21. */
    bool Is21 ();

    /** Function returns true if the hand is a bust.
        A hand is a bust if it is over 21 points. */
    bool IsBust ();

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

    /** Prints the abridged player stats to the console. */
    virtual void PrintStats ();

    /** Prints the player to the console. */
    virtual void Print ();

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    protected:

    CardStack& stock_; //< Stock of cards to draw from.

};
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_BLACKJACKPLAYER
