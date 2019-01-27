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

#ifndef HEADER_FOR_KABUKI_ARCADE_CARDS_REMOTEPLAYER
#define HEADER_FOR_KABUKI_ARCADE_CARDS_REMOTEPLAYER

#include "player.h"

namespace kabuki { namespace cards {

/** A Remote player in an abstract card game.
    It would be a poor design choice to allow the client app to have any data
    about the other player's hands, so the PlayerProxy is on a need to know
    basis only.
*/
class PlayerProxy: public Player {
    public:

    /** Default Constructor. */
    PlayerProxy (Deck& pack);
    
    /** Virtual destructor. */
    virtual ~PlayerProxy ();

    /** Gets the Player's status_. */
    const CH1* GetDislpayName ();

    /** Sets the Player's status_. */
    const CH1* SetDislpayName (const CH1* name);

    /** Gets the Player's status_. */
    const CH1* GetHandle ();

    /** Sets the Player's status_. */
    const CH1* SetHandle (const CH1* name);

    /** Gets is_dealer_. */
    BOL IsDealer ();

    /** Sets is_dealer_. */
    void SetIsDealer (BOL is_dealer);

    /** Gets this Player's state. */
    SI4 GetState ();

    /** Attempts to set the state. 
        @return Returns nil upon success and an error string upon
                failure. */
    virtual const CH1* SetState (int32_t state);

    /** Gets the num_points. */
    SI4 GetNumPoints ();

    /** Sets the num_points.
        @return Returns false if num_points is < 0. */
    BOL SetNumPoints (SI4 num_points);

    /** Returns the number of wins. */
    SI4 GetNumWins ();

    /** Returns the number of wins. */
    BOL SetNumWins (SI4 num_wins);

    /** Gets the visible_cards_. */
    CardStack& GetVisibleCards ();

    /** Deletes the visible cards. */
    void DeleteVisibleCards ();

    /** Adds a new Card to the visible_cards_.
        @return Returns -1 upon failure and the new number of visible cards upon
                success. */
    SI4 AddCard (byte pip, byte suit);

    /** Prints this object to the console. */
    virtual _::Text& Print (_::Text& txt = _::Text ());

    /** Script Operations. */
    virtual const _::Operation* Star (uint index, _::Expression* expr);

    protected:

    CH1      * status_,        //< PlayerProxy's name.
              * handle_;        //< Display handle.
    BOL        is_dealer_;     //< Flags if this player is the dealer.
    int32_t     state_,         //< The state of the player.
                num_points_,    //< Number of points.
                num_wins_;      //< Total number of wins.
    Deck      & pack_;          //< The pack of cards.
    CardStack   visible_cards_; //< Stock of Card(s) to draw from.

};      //< class PlayerProxy
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_CARDS_REMOTEPLAYER
