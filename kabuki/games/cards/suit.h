/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/card.h
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_KABUKI_ARCADE_CARDS_SUIT
#define HEADER_FOR_KABUKI_ARCADE_CARDS_SUIT

#include "config.h"

namespace kabuki { namespace cards {

/** A playing card suit.
*/
class KABUKI Suit {
    public:

    enum {
        kDefaultDenomination = 0,    //< Default suit value:0.
        kBlackSpade          = 2660, //< Unicode char for Black spade suit.
        kWhiteHeart          = 2661, //< Unicode char for White heart suit.
        kWhiteDiamond        = 2662, //< Unicode char for White diamond suit.
        kBlackClub           = 2663, //< Unicode char for Black club suit.
        kWhiteSpade          = 2664, //< Unicode char for White spade suit.
        kBlackHeart          = 2665, //< Unicode char for Black heart suit.
        kBlackDiamond        = 2666, //< Unicode char for Black diamond suit.
        kWhiteClub           = 2667, //< Unicode char for White club suit.
    };

    /** Enumerated list of the different suites on playing cards in various
         countries.
         |    Format    |    0   |    1     |   2    |    3    |
         |:------------:|:------:|:--------:|:------:|:-------:|
         |   Italian    | Cups   |  Coins   | Clubs  | Swords  |
         |   Spanish    | Cups   |  Coins   | Clubs  | Swords  |
         | Swiss-German | Roses  |  Bells   | Acorns | Shields |
         |    German    | Hearts |  Bells   | Acorns | Leaves  |
         |    French    | Hearts | Diamonds | Clubs  | Spades  | */
    enum { 
        kCup   = 0, kRose   = 0, kHeart   = 0,
        kCoin  = 1, kBell   = 1, kDiamond = 1,
        kClub  = 2, kAcorn  = 2,
        kSword = 3, kShield = 3, kLief    = 3, kSpade = 3,
    };

    typedef enum class Colors {
        kRed   = 0, //< Red suit color.
        kBlack = 1, //< Black Unicode suit color.
        kWhite = 2  //< White Unicode suit color.
    } Color;

    /** Enumerated list of the different French playing cards suites.
    typedef enum class FrenchSuits {
        kClub = 1, 
        kDiamond, 
        kHeart, 
        kSpade
    } FrenchSuit; */

    /** Enumerated list of the different German playing cards suites.
    typedef enum class GermanSuits {
        kAcorn = 1,
        kBell,
        kHeart,
        kLief
    } GermanSuit; */

    /** Enumerated list of the different German playing cards suites.
    typedef enum class SwissGermanSuits {
        kAcorn = 1,
        kBell,
        kRose,
        kShield
    } SwissGermanSuit; */

    /** Enumerated list of the different Latin playing cards suites.
    typedef enum class LatinSuits {
        kClub = 1,
        kCoin,
        kCup,
        kSword,
    } LatinSuit; */

    /** Enumerated list of different European-style playing card suites.
         Piacentine, Napoletane, Spagnole, Bergamasche suits are also known as
         Latin suites, because they are from countries that speak Romantic
         languages. */
    typedef enum SuitFormats {
        kFrench = 0,
        kGerman,
        kSwissGerman,
        kPiacentine,
        kNapoletane,
        kSpagnole,
        kBergamasche
    } Format;

    /** A list of the strings "Clubs", "Diamonds", "Hearts", "Spades". */
    static const char* kFrenchSuit[4];

    /** A list of the strings "Acorns", "Bells", "Hearts", "Levies". */
    static const char* kGermanSuit[4];

    /** A list of the strings "Acorns", "Bells", "Roses", "Shields". */
    static const char* kSwissGermanSuit[4];

    /** A list of the strings "Clubs", "Coins", "Cups", "Swords". */
    static const char* kLatinSuit[4];

    /** An array of strings that represent the 7 different directories for the
        suit icon file. */
    static const char* kFormatTexts[];

    /** Default constructor.
        There exists a single unique card for each of the 54 cards in a pack
        of cards. Cards are taken from the pack and shuffled into the deck_
        using pointers. */
    Suit (int column, int denomination = 0, Format format = kFrench,
          Color color = Color::kWhite);

    /** Copy constructor. */
    Suit (const Suit& other);
    
    /** Virtual destructor just in case we want to make a sub-class later. */
    virtual ~Suit () {}

    /** Sets the values of the cards. */
    void Set (int    denomination = kDefaultDenomination,
              Format format       = Format::kFrench,
              Color  color        = Color::kBlack);

    /** Compares this Suit to the other Suit.
        @return Returns 0 if they are identical.
        @return Returns 1 if this Suit is greater than the other Suit. */
    virtual int Compare (Suit* other);
                           
    /** Compares this Suit to the other Suit and returns true if the two are
        identical. */     
    bool Equals (Suit* other);
    
    /** Returns the rank of the denomination of this Suit
        The face value represents how many points this card is worth. */
    int GetDenomination ();

    /** Sets the suit denomination (rank among other suits). */
    void SetDenomination (int denomination);

    /** Returns the Unicode wchar_t color (white or black) for this suit. */
    wchar_t GetChar ();

    /** Gets the suit's color. */
    Color GetColor ();

    /** Sets the Unicode wchar_t color (white or black) for this suit. */
    void SetColor (Color color);

    /** Returns a string representation of the suit. */
    const char* GetLabel ();

    void SetFormat (Format format);

    /** Loads and sets the cardImage to the on in the given directory.
        Yes this goes in the library, just no file IO or hardware dependent
        stuff (besides OpenGL, OpenAL, etc).
        @param  uri Image directory relative to the compiled executable.
        @return Returns 0 upon success.
        @return Returns -1 if the directory was actually a file.
        @return Returns 1 if the directory doesn't exist.
        @return Returns 2 if the file doesn't.
        @return Returns 3 if the image didn't load right. */
    //int SetImage (const char* uri);
                       
    /** Returns the address of this Suit's image. */
    //Image& GetImage ();
    
    /** Returns a the label_. */
    _::Text& Print (_::Text& txt = _::Text ());

    private:

    /** Loads the cardImage from the specified directory.
         We can address images in DLL(s) but not files or anything
         hardware specific.
    */
    //Image LoadCardImage (const char* directory);

    int         column_,        //< The column of the suit.
                denomination_;  //< Rank of this suit compared to the others.
    wchar_t     char_;          //< Suit of this Suit.
    const char* label_;         //< Text label of the suit.
    Color       color_;         //< Color of the suit.
    //Image*    image_;         //< Image of this Suit.

};      //< class Suit

KABUKI Suit* SuitError ();

}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ARCADE_CARDS_SUIT
