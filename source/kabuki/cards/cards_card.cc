/** kabuki:cards
    @file    ~/source/project/kabuki_cards_servard.cc
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

#include "card.h"
#include "deck.h"

namespace kabuki { namespace cards {

// These 4 string arrays allow us to print the names of suits in our text 
// console app. The last 4 suit_ suitCultures are the Latin Suits.
const char* Card::kFrenchSuit[4] = { "Clubs", "Diamonds", "Hearts",
                                     "Spades" };
const char* Card::kGermanSuit[4] = { "Acorns", "Bells", "Hearts",
                                     "Leaves" };
const char* Card::kSwissGermanSuit[4] = { "Acorns", "Bells", "Roses",
                                          "Shields" };
const char* Card::kLatinSuit[4] = { "Clubs", "Coins", "Cups",
                                    "Swords" };
const char* Card::kSuitCulturestrings[] = { "French", "German", "Swiss-German",
                                            "Piacentine", "Napoletane",
                                            "Spagnole", "Bergamasche" };

Card::Card (int pip_value, Suit suit) {
    SetCard (pip_value, suit, pip_value, pip_value);
}

Card::Card (int pip_value, Suit suit, int face_value, int point_value,
            int suit_value, SuitCulture culture,
            const char* folder_path) :  //< This is called an
    suit_ (suit)      //< initialization list. It sets the values of the object.
{
    SetCard (pip_value, suit, face_value, point_value, suit_value, culture,
             folder_path);
    // Nothing to do here. ({:-)
}

Card::Card (const Card& other) :
    pip_value_    (other.pip_value_),   //< Make sure you initialize the values
    face_value_   (other.face_value_),  //< in the same order they are in the 
    point_value_  (other.point_value_), //< header file!
    suit_value_   (other.suit_value_),
    suit_         (other.suit_),
    suit_culture_ (other.suit_culture_),
    suit_string_  (other.suit_string_) {
    //card_image_ (other.card_image_)
    // All the copy constructor does is copy the other object. Nothing hard about this.
}

Card& Card::operator= (const Card& other) {
    // Function sets all of the private values to the other object's values... its really easy.
    pip_value_    = other.pip_value_;
    face_value_   = other.face_value_;
    point_value_  = other.point_value_;
    suit_value_   = other.suit_value_;
    suit_         = other.suit_;
    suit_culture_ = other.suit_culture_;
    suit_string_  = other.suit_string_;
    //card_image_   = other.card_image_;
}

int Card::Compare (const Card& other) {
    int otherValue = other.pip_value_,
        otherSuitValue = other.suit_value_;

    if (pip_value_ > otherValue)
        return 1;
    if (pip_value_ < otherValue)
        return -1;

    // Else then the two pipValues are equal so compare the suits.
    if (suit_value_ > otherSuitValue)
        return 1;
    if (suit_value_ < otherSuitValue)
        return -1;
    // Else then the two cards are the same suit_ and pip_value_.
    return 0;
}

bool Card::Equals (const Card& other) {
    return !Compare (other);
}

int Card::GetPipValue () {
    return pip_value_;
}

void Card::SetPointValue (int newValue) {
    // The user might want to use a negative point value in a game.
    point_value_ = newValue;
}

int Card::GetFaceValue () {
    return face_value_;
}

int Card::GetPointValue () {
    return face_value_;
}

int Card::GetSuitValue () {
    return suit_value_;
}

Card::Suit Card::GetSuit () {
    return suit_;
}

Card::SuitCulture Card::GetSuitCulture () {
    return suit_culture_;
}

void Card::SetSuitCulture (Card::SuitCulture culture) {
    suit_culture_ = culture;
}

const char* Card::GetSuitString () {
    return suit_string_;
}

/*
int Card::LoadCardImage (const char* directory) {
    // First, check to see if the directory is valid.
    File filePath = File (directory);

    // Next check to see if it isn't a normal file.
    if (filePath.existsAsFile ())
        return -1;

    if (!filePath.isDirectory ())
        return -1;
    // Directory is good, now check to see if the directory contains an image with the correct naming convention.

    string filename = string (pip_value_) + "-" + string (suit_) + ".svg";

    File filename = directory + filename;

    if (!filename.existsAsFile ())
        return 2;

    Image tempImage = ImageCache::GetFromFile (filename);

    if (tempImage.isNull ())
        return 3;

    card_image_ = tempImage;

    return 0;
}*/

//Image& Card::GetImage () {
//    return card_image_;
//}

void Card::Print () {
    switch (pip_value_) {
        case 0:   {
            cout << suit_string_;
        }
        case 1: {
            cout << "Ace of ";
            break;
        }
        case 11: {
            cout << "Jack of ";
            break;
        }
        case 12: {
            cout << "Queen of ";
            break;
        }
        case 13: {
            cout << "King of ";
            break;
        }
        default: {
            cout << pip_value_ << " of ";
            break;
        }
    }
    cout << suit_string_;
}

void Card::SetCard (int pip_value, Suit suit, int face_value, int points_value,
                    int suit_value, SuitCulture culture,
                    const char* folder_path) {
    // First we have to ensure that the input values were in bounds.

    if (pip_value < 0) {
        //< We can't have any negative numbers.
        pip_value_ = pip_value;
    }
    else if (pip_value > 13) {
        //< Pip values range from 0 - 13 J=0, A=1, J=10, Q=11, K=13
        pip_value_ = 13;
    }
    pip_value_ = pip_value;

    if (suit > Suit::kSpade) {
        suit = Suit::kSpade;
    }
    suit_ = suit;

    if (face_value < 0) {
        //< 0 is a Joker
        face_value_ = 0;
    }
    else if (face_value > 14) {
        //< An ace can have a face value of 14 in some games.
        face_value_ = 14;
    }
    face_value_ = face_value;

    if (face_value < 0) {
        face_value_ = 0;
    }
    else if (face_value > 10) {
        face_value_ = 10;
    }
    face_value_ = face_value;

    if (suit_value < 1) {
        suit_value_ = 1;
    }
    else if (suit_value > 4) {
        suit_value_ = 4;
    }
    suit_value_ = suit_value;

    suit_culture_ = culture;
    int suit_index = (int) suit_;
    switch (culture) {
        case SuitCulture::kFrench: {
            suit_string_ = kFrenchSuit[suit_index];
            break;
        }
        case SuitCulture::kGerman: {
            suit_string_ = kGermanSuit[suit_index];
            break;
        }
        case SuitCulture::kSwissGerman: {
            suit_string_ = kSwissGermanSuit[suit_index];
            break;
        }
        default: { 
            //< kNapoletane, kSpagnole, and kBergamasche share the same suites.
            suit_string_ = kLatinSuit[suit_index];
            break;
        }
    }
    //LoadCardImage (folder_path);
}

}   //< namespace cards
}   //< namespace kabuki
