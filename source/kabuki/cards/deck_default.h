/** kabuki:cards
    @file    ~/source/kabuki/cards/Deck/DojoCard.h
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

#ifndef KABUKI_CARDS_DECKDEFAULT_H
#define KABUKI_CARDS_DECKDEFAULT_H

#include "card.h"

namespace kabuki { namespace cards {

class DeckDefault : public 
{
    public:

    /** An enumberated list of the different suites on playing cards in various countries. */
    typedef enum Suites { club = 1, acorn = 1, coin = 1, diamond = 2, rose = 2, cup = 2, heart = 3, bell = 3, spade = 4, lief = 4, shield = 4, sword = 4 } Suit;

    /** An enumerated list of different European-style playing card suites.
        Piacentine, Napoletane, Spagnole, Bergamasche suits are also known as Latin suites, because they are from countries that speak Romantic languages. */
    typedef enum Culture { french = 1, german = 2, swissGerman = 3, latin = 4, Piacentine = 4, Napoletane = 4, Spagnole = 4, Bergamasche = 4 };

    /** An enumberated list of the different French playing cards suites. */
    typedef enum FrenchSuits { club = 1, diamond, heart, spade } FrenchSuit;

    /** An enumberated list of the different German playing cards suites. */
    typedef enum GermanSuits { acorn = 1, bell, heart, lief } GermanSuit;

    /** An enumberated list of the different German playing cards suites. */
    typedef enum SwissGermanSuits { acorn = 1, bell, rose, shield } SwissGermanSuit;

    /** An enumberated list of the different Latin playing cards suites. */
    typedef enum LatinSuits  { club = 1, coin, cup, sword } LatinSuit;

    /** Constructor. */
    Card (uint thisValue, Suit suit, Culture thisCulture=french);
    
    /** Virtual destructor so users can make a Card sub-class! */
    virtual ~Card () {}
    
    /** Compares this Card to the other Card and returns 0 if they are identical. */
    int Compare(const Card& other);
    
    /** Compares this Card to the other Card and returns true if the two are identical. */
    bool Equals(const Card& other);

    /** Returns the face value of this Card. */
    uint GetValue ();
    
    /** Returns the Suit of this card. */
    Suit GetSuit ();

    /** Returns the Culture of this Card. */
    Culture GetCulture ();
    
    /** Function sets the culture to the newCulture. */
    void SetCulture (Culture newCulture);

    /** Returns a string representation of the suit. */
    string GetSuitstring ();

    /** Returns a string representation of this Object. */
    void Print (_::Log& log);

    private:

    Suit    suit_;      //< The suit of this Card.
    uint    value_;     //< The value of this Card.
    Culture culture_;   //< The Culture of this Card.
    Image&  suit_icon_; //< The icon of the suit.
};

}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_DECKDEFAULT_H
