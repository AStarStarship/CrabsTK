/** kabuki:cards
    @file    ~/source/kabuki/cards/player.h
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

#ifndef KABUKI_CARDS_PLAYER_H
#define KABUKI_CARDS_PLAYER_H

#include "hand.h"
#include "card_stack.h"

namespace kabuki { namespace cards {

/** An abstract player in an abstract card game. */
class Player {
    public:

    /** Default Constructor. */
    Player (CardStack& stock, const char* player_name = "Butt Face", int start_points = 100,
            bool is_dealer = false);
    
    /** Virtual destructor. */
    virtual ~Player () {}

    /** Pure virtual new game logic handler. */
    virtual void NewGame () = 0;

    /** Processes beginning of round logic. */
    virtual void BeginRound () = 0;

    /** Performs round logic. */
    virtual void PlayRound () = 0;

    /** Processes beginning of round logic. */
    virtual void EndRound () = 0;

    /** Processes beginning of round logic. */
    virtual void EndGame () = 0;

    /** Returns true if this hand wins compared to the other one. */
    virtual bool HandWins (Hand& other) = 0;

    /** Gets the Player's status_. */
    const char* GetDislpayName ();

    /** Sets the Player's status_. */
    const char* SetDislpayName (const char* name);

    /** Gets is_dealer_. */
    bool IsDealer ();

    /** Sets is_dealer_. */
    void SetIsDealer (bool is_dealer);

    /** Gets this Player's state. */
    int GetState ();

    /** Attempts to set the state. 
        @return Returns nullptr upon success and an error string upon
                failure. */
    virtual const char* SetState (int state);

    /** Gets the num_points. */
    int GetNumPoints ();

    /** Sets the num_points.
        @return Returns false if num_points is < 0. */
    bool SetNumPoints (int num_points);
    
    /** Adds a specified num_points to the players point total.
        @return Returns 0 upon success, and -1 if num_points is less than 1. */
    int AddPoints (int num_points);
                  
    /** Removes the specified num_points from the players point total.
        @return Returns false if num_pointers is less than 0 or if the player 
                doesn't have enough points to take. */
    bool RemovePoints (int num_points);

    /** Returns the number of wins. */
    int GetNumWins ();

    /** Returns the number of wins. */
    bool SetNumWins (int num_wins);

    /** Adds a win to the players numWins. */
    void AddWin ();

    /** Returns the player's hand. */
    Hand& GetHand ();
                   
    /** Resets the numWins to 0. */
    void ResetWins ();

    virtual void PrintStats () = 0;

    /** Prints this object to the console. */
    virtual void Print ();

    protected:

    const char* name_;       //< Player's name.
    bool        is_dealer_;  //< Flags if this player is the dealer or not.
    int         state_,      //< The state of the player.
                num_points_, //< Number of points.
                num_wins_;   //< Total number of wins.
    Hand        hand_;       //< Player's Hand.
    CardStack& stock_;       //< Stock of Card(s) to draw from.
};

}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_PLAYER_H
