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

class Player
{
    public:

    /** Default Constructor. */
    Player (string thisPlayerName = "You", int startPoints = 10, bool playerOrDealer = false);
    
    /** Virtual destructor. */
    virtual ~Player () {}

    /** Virtual function that determines if this Player plays, or passes. */
    virtual bool playOrPass (Hand& other) = 0;
    
    /** Virtual function that performs a Player's turn. */
    virtual void PlayHand () = 0;
    
    /** Virtual function that performs the discard action. */
    virtual Array<Card*> Discard ();

    /** Returns the player_name_. */
    const char* GetName ();
    
    /** Sets the playerName to the newName. */
    void SetName (const char* newName);
    
    /** Returns the player's hand. */
    Hand& GetHand ();
    
    /** Sets the hand to the newHand. */
    void SetHand (Hand& newHand);

    /** Returns the point total. */
    int GetNumPoints ();
    
    /** Adds a specified numPoints to the players point total.
        @return Returns 0 upon success, and -1 if numPoints is less than 1. */
    int AddPoints (int num_points);
                  
    /** Removes the specified numPoints from the players point total.
        @return Returns 0 upon success, -1 if thesePoints is less than 0, or returns the number of missing points if the 
            player doesn't have enough points to take. */                                  
    int RemovePoints (int num_points);
                   
    /** Resets the numWins to 0. */
    void resetWins ();
    
    /** Returns the number of wins. */
    int GetNumWins ();
    
    /** Adds a win to the players numWins. */
    void AddWin ();

    /** Resets the number of wins to 0. */
    void DealHand (Hand& newHand);

    /** Returns a string reprentation of this object. */
    void Print (_::Log& log);

    private:

    string name_;       //< The Player's name.
    bool   is_dealer_;  //< Stores if this player is the dealer or not.
    int    num_points_, //< The number of points.
           num_wins_;   //< The total number of wins.
    Hand&  hand_;       //< The Player's Hand.
};

}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_PLAYER_H
