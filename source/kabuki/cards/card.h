/** kabuki:cards
    @file    ~/source/kabuki/cards/card.h
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

#ifndef HEADER_FOR_KABUKI_CARDS_CARD
#define HEADER_FOR_KABUKI_CARDS_CARD

#include "config.h"

namespace kabuki { namespace cards {

/** Class that represents a card in a card game.
    Cards as we know them originated in Europe. We traditionally think of cards
    as only being clubs, hearts, diamonds, and spades, but there are also some
    other variations from other European countries. For more information on
    them, please Wiki "Playing Card Suits". The nifty thing about these extra
    card decks, is that we can make a double deck of cards, and use the suit
    culture to tell the decks apart.
*/
class Card {
    public:

    enum {
        kDefaultSuitValue = 1, //< Default suit value.
    };

    /** Enumerated list of the different suites on playing cards in various
         countries. */
    typedef enum class Suites { 
        kClub = 1, kAcorn = 1, kCoin = 1, kBlack = 1, 
        kDiamond = 2, kRose = 2, kCup = 2, kRed = 2, 
        kHeart = 3, kBell = 3, 
        kSpade = 4, kLief = 4, kShield = 4, kSword = 4 ,
    } Suit;

    /** Enumerated list of the different French playing cards suites. */
    typedef enum class FrenchSuits {
        kClub = 1, 
        kDiamond, 
        kHeart, 
        kSpade
    } FrenchSuit;

    /** Enumerated list of the different German playing cards suites. */
    typedef enum class GermanSuits {
        kAcorn = 1,
        kBell,
        kHeart,
        kLief
    } GermanSuit;

    /** Enumerated list of the different German playing cards suites. */
    typedef enum class SwissGermanSuits {
        kAcorn = 1,
        kBell,
        kRose,
        kShield
    } SwissGermanSuit;

    /** Enumerated list of the different Latin playing cards suites. */
    typedef enum class LatinSuits {
        kClub = 1,
        kCoin,
        kCup,
        kSword,
    } LatinSuit;

    /** Enumerated list of different European-style playing card suites.
         Piacentine, Napoletane, Spagnole, Bergamasche suits are also known as
         Latin suites, because they are from countries that speak Romantic
         languages. */
    typedef enum SuitCultures {
        kFrench = 1,
        kGerman,
        kSwissGerman,
        kPiacentine,
        kNapoletane,
        kSpagnole,
        kBergamasche
    } SuitCulture;

    /** A list of the strings "Clubs", "Diamonds", "Hearts", "Spades" */
    static const char* kFrenchSuit[4];

    /** A list of the strings "Acorns", "Bells", "Hearts", "Levies" */
    static const char* kGermanSuit[4];

    /** A list of the strings "Acorns", "Bells", "Roses", "Shields" */
    static const char* kSwissGermanSuit[4];

    /** A list of the strings "Clubs", "Coins", "Cups", "Swords" */
    static const char* kLatinSuit[4];

    /** An array of strings that represent the 7 different directories for the
         suit icon file */
    static const char* kSuitCulturestrings[];

    /** Default constructor.
        This constructor creates a simple card with no image. The faceValue will
        equal the pointValue. Setting Default Values: When you put the = in the 
        constructor, it sets the default value. This case, we can make a card
        like this: Card example (); This constructor creates a Black Joker card.
    */
    Card (int pip_value = 0, Suit suit = Suit::kBlack);
    
    /** Verbose constructor. */
    Card (int pip_value, Suit suit, int face_value, int point_value, 
          int suit_value = kDefaultSuitValue,
          SuitCulture culture = SuitCulture::kFrench,
          const char* image_directory = nullptr);
    
    /** Copy constructor. */
    Card (const Card& other);
    
    /** Virtual destructor just in case we want to make a sub-class later. */
    virtual ~Card () {}

    /** Operator= overlaoder deeps copies the other Card. */
    Card& operator= (const Card& other);

    /** Compares this Card to the other Card.
        @return Returns 0 if they are identical.
        @return Returns 1 if this Card is greater than the other Card. */
    virtual int Compare (const Card& other);
                                          
    /** Compares this Card to the other Card and returns true if the two are
        identical. */     
    bool Equals (const Card& other);

    /** Returns the pip value of this Card.
        The pip value is a number between 0-13 that represents which card it is: 
        J=0, A=1, 2-10, J=11, Q=12, K=13. */
    int GetPipValue ();
                           
    /** Returns the face value of this Card
         The face value represents the rank of the card. */                         
    int GetFaceValue ();
    
    /** Returns the point value of this Card
        The face value represents how many points this card is worth. */
    int GetPointValue ();
                                     
    /** Sets the point value to the value. */          
    void SetPointValue (int value);
    
    /** Returns the face value of this Card
        The face value represents how many points this card is worth. */
    int GetSuitValue ();

    /** Returns the Suit of this card. */
    Suit GetSuit ();

    /** Returns the Card::Culture of this Deck.
        Function sets the suitCulture to the cardCulture. */
    SuitCulture GetSuitCulture ();
        
    void SetSuitCulture (SuitCulture culture);

    /** Returns a string representation of the suit. */
    const char* GetSuitString ();

    /** Loads and sets the cardImage to the on in the given directory.
        Yes this goes in the library, just no file IO or hardware dependent
        stuff (besides OpenGL, OpenAL, etc).
        @return Returns 0 upon success.
        @return Returns -1 if the directory was actually a file.
        @return Returns 1 if the directory doesn't exist.
        @return Returns 2 if the file doesn't.
        @return Returns 3 if the image didn't load right. */
    //int SetCardImage (const char* directory);
                       
    /** Returns the address of this Card's image. */                            
    //Image& GetImage ();
    
    /** Returns a string representation of this Object. */
    void Print ();

    private:
    
    //< Sets the values of the cards.
    void SetCard (int pip_value, Suit suit, int face_value, int point_value,
                  int suit_value = kDefaultSuitValue,
                  SuitCulture suit_culture = SuitCulture::kFrench,
                  const char* folder_path = nullptr);

    /** Loads the cardImage from the specified directory.
         We can address images in DLL(s) but not files or anything
         hardware specific.
    */
    //Image LoadCardImage (const char* directory);

    int         pip_value_,    //< Value of this Card from 0-13.
                face_value_,   //< Face value of this Card from 1-14.
                point_value_,  //< Number of points this Card is worth from 0-10.
                suit_value_;   //< Value of this suit for changing suit's value.
    Suit        suit_;         //< Suit of this Card.
    SuitCulture suit_culture_; //< Culture of this Card.
    const char* suit_string_;  //< String that stores the suit.
    //Image       card_image_;     //< Image of this Card.
};      //< class Card
}       //< namespace cards
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_CARDS_CARD
