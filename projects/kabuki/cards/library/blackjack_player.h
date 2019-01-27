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

#ifndef HEADER_FOR_KABUKI_ARCADE_BLACKJACKPLAYER
#define HEADER_FOR_KABUKI_ARCADE_BLACKJACKPLAYER

#include "player.h"

namespace kabuki { namespace cards {

/** Calculates the blackjack store of the given hand and ace_value.
    @param hand The hand.
    @param ace_value Use value of either 1 or 13.
    @return Returns the score. */
KABUKI SI4 BlackjackScore (Hand& hand, SI4 ace_value);

/** Returns the min score of this hand where Aces are worth 1. */
KABUKI SI4 BlackjackMinScore (Hand& hand);

/** Returns the max score of this hand where Aces are worth 11. */
KABUKI SI4 BlackjackMaxScore (Hand& hand);

/** Compares the two Blackjack hands. */
KABUKI SI4 BlackjackCompareHands (Hand& a, Hand& b);

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
                     BOL is_dealer = false);

    /** Destructor. */
    virtual ~BlackjackPlayer ();

    /** Sets the state of the player. */
    const CH1* SetState (SI4 state) override;

    /** Handles hit signal. */
    void Hit ();

    /** Handles hold signal. */
    void Hold ();

    /** Returns the HighLowScore. */
    //Array<CardCombo> GetHandCombos ();

    /** Returns true if the player is holding. */
    virtual BOL IsHolding ();

    /** Function returns if this hand is 21. */
    BOL Is21 ();

    /** Function returns true if the hand is a bust.
        A hand is a bust if it is over 21 points. */
    BOL IsBust ();

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
    virtual SI4 Compare (Hand& hand);

    /** Returns true if this hand wins compared to the other one. */
    virtual BOL Wins (Hand& hand);

    /** Prints the abridged player stats to the console. */
    virtual _::Text& PrintStats (_::Text& txt);

    /** Prints the player to the console. */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /** Script operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    /** Handles Text input.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const CH1* Sudo (const CH1* text,
                                    const CH1* strand_end);
    protected:

    CardStack& stock_; //< Stock of cards to draw from.

};
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_BLACKJACKPLAYER
