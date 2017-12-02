/** kabuki:cards
    @file    ~/source/kabuki/cards/card_combo.h
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

#ifndef HEADER_FOR_KABUKI_CARDS_CARDCOMBO
#define HEADER_FOR_KABUKI_CARDS_CARDCOMBO

#include "card_stack.h"

namespace kabuki { namespace cards {
    
/** A CardStack point combination in a playing card game.
    There are two major things that will effect a combination's rank besides
    suit rank; the presence of jokers, and if aces are high or low. */
class CardCombo : public CardStack { 
    public:

    /** Default constructor. */
    CardCombo (const CardStack& cards, bool aces_high = false,
               int joker_pip_1 = 0, int joker_suit_1 = 1,
               int joker_pip_2 = 0, int joker_suit_2 = 2);

    /** Sets the given variables settings. */
    void Set (bool aces_high = false, int joker_pip_1 = 0, int joker_suit_1 = 1,
              int joker_pip_2 = 0, int joker_suit_2 = 2);

    /** Function that returns the point value with the wild card and ace
        values. */
    virtual int GetValue () = 0;

    /** Returns the joker_pip_1. */
    int GetJokerPip1 ();
    
    /** Sets the joker_pip_1 to the value. */
    bool SetJokerPip1 (int value);

    /** Returns the joker_suit_1. */
    int GetJokerSuitValue1 ();
    
    /** Sets the joker_suit_1 to the value. */
    bool SetJokerSuitValue1 (int value);

    /** Returns the joker_pip_2. */
    int GetJokerPip2 ();
    
    /** Sets the joker_pip_2 to the value. */
    bool SetJokerPip2 (int value);

    /** Returns the joker_suit_2. */
    int GetJokerSuitValue2 ();
    
    /** Sets the joker_suit_2 to the value. */
    bool SetJokerSuitValue2 (int value);

    /** Returns the joker_pip_1. */
    int AcesHigh ();
    
    /** Sets the joker_pip_1 to the value. */
    void SetAcesHigh (int value);
    
    private:

    bool aces_high_;     //< Flag represents if aces are high (>1) or 
                         //< low (0), or either (<0).
    int  joker_pip_1_,   //< Black joker wildcard pip value.
         joker_suit_1_,  //< Black joker wildcard suit value.
         joker_pip_2_,   //< Red joker wildcard pip value.
         joker_suit_2_;  //< Red joker wildcard suit value.
        
};      //< class CardCombo
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_CARDS_CARDCOMBO
