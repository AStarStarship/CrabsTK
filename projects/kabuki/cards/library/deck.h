/* Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/deck.h
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

#ifndef KABUKI_ARCADE_CARDS_DECK_H
#define KABUKI_ARCADE_CARDS_DECK_H

#include "card.h"

namespace kabuki { namespace cards {

using pack_ptr = std::unique_ptr<Card*>;
    
/* A Deck of playing cards.
    A Deck of cards usually has 52 cards, but can also have 2 jokers, a black
    one, and a red one. Not all card games have this many cards, so this class
    is setup for you to create decks of cards for just about any card game you
    can think of. */
class Deck {
    public:

    enum {
        kDefaultDeckSize = 52, //< Default num cards in a deck excluding Jokers.
        kFullDeckSize    = 54, //< Default num cards in a deck with Jokers.
        kAcesHigh        = 14, //< Flag for if Aces are high.
        kAcesLow         = 0,  //< Flag for if Aces are low.
        kHasJokers       = 1,  //< Flag for whether the deck has Jokers.
        kNoJokers        = 0,  //< Flag for whether the deck has no Jokers.
        kDefaultNumDecks = 1,  //< Default num decks.
    };

    // The default suit value order, Clubs=1, Diamonds=2, Hearts=3, and Spades=4.
    static const SI4 kDefaultSuitValues[4];

    /* The default deck art directory. */
    static const CH1 kDefaultDeckArtDirectory[],
                      kDefaultRearDeckImage[]; //< Default rear deck image.
    
    /* Constructor.
        In most card games, jokers are no used, and aces are high, so we use
        those are the default values.
        @param  contains_jokers True if the deck contains jokers, and false
                                otherwise.
        @param  aces_high       0 means that aces are low, a positive number
                                means that aces are high, a negative number
                                means that aces are low.
        @param  format          Used to determine what suites we are using. */
    Deck (SI4 num_decks = 1, BOL has_jokers = kNoJokers, 
          SI4 aces_high = kAcesHigh,
          Suit::Format format = Suit::kFrench,
          const CH1* deck_name = kDefaultRearDeckImage,
          const CH1* directory_path = kDefaultDeckArtDirectory,
          Suit::Color color = Suit::Color::kWhite);

    /* Destructor. */
    virtual ~Deck ();

    /* Sets the given parameters of the deck. */
    void Set (BOL has_jokers = kNoJokers, SI4 aces_high = kAcesHigh,
              Suit::Format format = Suit::kFrench,
              const CH1* deck_name = kDefaultRearDeckImage,
              const CH1* directory_path = kDefaultDeckArtDirectory,
              Suit::Color color = Suit::Color::kWhite);

    /* Returns true if this deck has Jokers. */
    BOL HasJokers ();

    /* Returns the number of cards in the deck. */
    SI4 GetSize ();

    /* Returns a pointer to a static Suit for the column 0 types AKA
        Hearts. */
   Suit* Heart ();

    /* Returns a pointer to a static Suit for the column 1 types AKA
        Diamonds. */
   Suit* Diamond ();

    /* Returns a pointer to a static Suit for the column 2 types AKA
        Clubs. */
    Suit* Club ();

    /* Returns a pointer to a static Suit for the column 3 types AKA
        Spades. */
    Suit* Spade ();

    /* Returns an array of the static Card objects for the 4 columns 0-3. */
    Suit** Suits ();

    /* Function sets the suit values to the new values.
        @pre    valueN must be between 1-4.
        @return Returns 0 upon success, and a number 1-4 if the valueN is not
        between 1-4. The number will be negative if the inputed number
        was, and vice-versa. */
    void SetSuitDenominations (SI4 column_0, SI4 column_1, SI4 column_2, SI4 column_3);

    /* Sets the format to the one given. */
    void SetFormat (Suit::Format format);
    
    /* Returns the if aces are high or low. */
    BOL AcesHigh ();

    /* Returns a pointer to the Card with the given suit and pip.
        @return Returns nil if the index is out of bounds. */
    Card* GetCard (SI4 suit, SI4 pip);
    
    /* Returns a pointer to the Card at the specified index.
        @return Returns nil if the index is out of bounds. */
    Card* GetCard (SI4 index);

    /* Sets the frontImage to a new Image from thisFile.
        @pre    thisFile must be a valid existing Image file.
        @return returns -1 if thisFile is not a valid Image. */
    //SI4 SetRearImage (const File& thisFile);
        
    /* Loads the deck art from the directory_path.
        @pre    directory_path must contain all 54 card Images in 
                [1-13]-[1-4].svg/J-1.svg/J-2.svg format.
        @return Returns 0 upon success, 54 if the directory doesn't contain 
                54 .svg files, and -x if images are not named correctly,
                where x is the first failed filename. */
    //SI4 SetDeckArt (const CH1* directory_path);
    
    /* Prints this object to the console. */
    _::Text& Print (_::Text& txt = _::Text ());

    private:

    /* Function that checks the artFolder to see if the it has 54 Images in 
        [1-13]-[1-4].svg/J-1.svg/J-2.svg format. */
    //SI4 CheckDeckArtFolder (const CH1* directory_path);

    BOL     has_jokers_;    //< Flag for if the deck has Jokers.
    SI4      num_decks_,     //< Num decks in the pack.
             num_cards_,     //< Number of cards.
             aces_high_,     //< Flags for aces high or low.
             lowest_value_,  //< Lowest allowed card value.
             highest_value_; //< Highest allowed card value.
    Suit     heart_,         //< The heart suit.
             diamond_,       //< The diamond suit.
             club_,          //< The club suit.
             spade_;         //< The spade suit.
    Suit   * suits_[4];      //< Array of Suits.
    TArray<Card*> pack_; //< Unique array of single cards.
    //Image rear_image_;     //< The rear Image of the Deck.

};      //< class Deck
}       //< namespace cards
}       //< namespace kabuki
#endif  //< KABUKI_ARCADE_CARDS_DECK_H
