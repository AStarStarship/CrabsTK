/** kabuki:cards
    @file    ~/source/kabuki/cards/deck.h
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

#ifndef KABUKI_CARDS_DECK_H
#define KABUKI_CARDS_DECK_H

#include "card.h"
#include "../../data/array.h"

namespace kabuki { namespace cards {
    
/** A Deck of playing cards.
    A Deck of cards usually has 52 cards, but can also have 2 jokers, a black
    one, and a red one. Not all card games have this many cards, so this class
    is setup for you to create decks of cards for just about any card game you
    can think of. */
class Deck {
    public:

    enum {
        kDefaultNumCards = 52, //< Default num cards in a deck excluding Jokers.
        kFullDeckSize    = 54, //< Default num cards in a deck with Jokers.
        kAcesHigh        = 14, //< Flag for if Aces are high.
        kAcesLow         = 0,  //< Flag for if Aces are low.
        kHasJokers       = 0,  //< Flag for whether the deck has Jokers.
        kNoJokers        = 1,  //< Flag for whether the deck has no Jokers.
    };

    static const int kDefaultSuitValues[4]; //< The default suit value order, Clubs=1, Diamonds=2, Hearts=3, and Spades=4.

    /** The default deck art directory. */
    static const char kDefaultDeckArtDirectory[],
                      kDefaultRearDeckImage[]; //< Default rear deck image.
    
    /** Constructor.
        In most card games, jokers are no used, and aces are high, so we use
        those are the default values.
        @param  contains_jokers True if the deck contains jokers, and false
                                otherwise.
        @param  aces_high       0 means that aces are low, a positive number
                                means that aces are high, a negative number
                                means that aces are low.
        @param  culture         Used to determine what suites we are using.
    */
    Deck (bool has_jokers = kNoJokers, int aces_high = kAcesHigh,
          Card::SuitCulture culture = Card::kFrench, 
          const char* deck_name = kDefaultRearDeckImage,
          const char* directory_path = kDefaultDeckArtDirectory);

    bool Set (bool has_jokers = kNoJokers, int aces_high = kAcesHigh,
              Card::SuitCulture culture = Card::kFrench,
              const char* deck_name = kDefaultRearDeckImage,
              const char* directory_path = kDefaultDeckArtDirectory);
        
    /** Destructor. */
    virtual ~Deck () {}

    /** Changes the SuitCulture to the given one. */
    void SetSuitCulture (Card::SuitCulture culture);

    /** Resets the deck to the initial state. */
    void Reshuffle ();
    
    /** Function sets the suit values to the new values.
        @pre    valueN must be between 1-4.
        @return Returns 0 upon success, and a number 1-4 if the valueN is not
                between 1-4. The number will be negative if the inputed number
                was, and vice-versa. */
    bool SetSuitValues (int value1, int value2, int value3, int value4);

    /** Gets the suit value of the given suit.
        @pre    suit must be between 1-4.
        @return Returns a number between 1-4 upon success, and -1 if the index
                was out of bounds. */
    int GetSuitValue (int suit);
    
    /** Returns true if this deck has Jokers. */
    bool HasJokers ();

    /** Returns the number of cards in the deck. */
    int GetCount ();
    
    /** Returns the if aces are high or low. */
    bool AcesHigh ();
    
    /** Returns a pointer to the Card at the specified index.
        @return Returns nullptr if the index is out of bounds. */
    Card* GetCard (int index);

    /** Sets the frontImage to a new Image from thisFile.
        @pre    thisFile must be a valid existing Image file.
        @return returns -1 if thisFile is not a valid Image. */
    //int SetRearImage (const File& thisFile);
        
    /** Loads the deck art from the directory_path.
        @pre    directory_path must contain all 54 card Images in 
                [1-13]-[1-4].svg/J-1.svg/J-2.svg format.
        @return Returns 0 upon success, 54 if the directory doesn't contain 
                54 .svg files, and -x if images are not named correctly,
                where x is the first failed filename. */
    //int SetDeckArt (const char* directory_path);
        
    /** Returns a string representation of the suit. */
    const char* GetSuitstring (Card::Suit suit);
    
    /** Prints this object to the console. */
    void Print ();

    private:

    /** Function that checks the artFolder to see if the it has 54 Images in 
        [1-13]-[1-4].svg/J-1.svg/J-2.svg format. */
    //int CheckDeckArtFolder (const char* directory_path);

    bool               has_jokers_;      //< Flag for if the deck has Jokers.
    Card::SuitCulture  culture_;         //< Culture of the suits.
    int                aces_high_,       //< Flags for aces high or low.
                       num_cards_,       //< Num cards in the deck.
                       lowest_value_,    //< Lowest allowed card value.
                       highest_value_,   //< Highest allowed card value.
                       suit_values_[4];  //< Values of the 4 suites.
    Card               pack_[54];        //< Unique array of single cards.
    //Image         rear_image_;         //< The rear Image of the Deck.
};

}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_CARDS_DECK_H
