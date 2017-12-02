/** kabuki:cards
    @file    ~/source/project/kabuki/cards_card.cc
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

Card::Card () {
}

void Card::Set (int pip, Suit suit, int face_value, int points_value,
                    int suit_value, SuitCulture culture) {
    // First we have to ensure that the input values were in bounds.

    if (pip < 0) {
        //< We can't have any negative numbers.
        pip_ = pip;
    } else if (pip > 13) {
        //< Pip values range from 0 - 13 J=0, A=1, J=10, Q=11, K=13
        pip_ = 13;
    }
    pip_ = pip;

    if (suit > Suit::kSpade) {
        suit = Suit::kSpade;
    }
    suit_ = suit;

    if (face_value < 0) {
        //< 0 is a Joker
        face_value_ = 0;
    } else if (face_value > 14) {
        //< An ace can have a face value of 14 in some games.
        face_value_ = 14;
    }
    face_value_ = face_value;

    if (face_value < 0) {
        face_value_ = 0;
    } else if (face_value > 10) {
        face_value_ = 10;
    }
    face_value_ = face_value;

    if (suit_value < 1) {
        suit_value_ = 1;
    } else if (suit_value > 4) {
        suit_value_ = 4;
    }
    suit_value_ = suit_value;

    suit_culture_ = culture;
    int suit_index = (int)suit_;
    switch (culture) {
        case SuitCulture::kFrench:
        {
            suit_string_ = kFrenchSuit[suit_index];
            break;
        }
        case SuitCulture::kGerman:
        {
            suit_string_ = kGermanSuit[suit_index];
            break;
        }
        case SuitCulture::kSwissGerman:
        {
            suit_string_ = kSwissGermanSuit[suit_index];
            break;
        }
        default:
        {
            //< kNapoletane, kSpagnole, and kBergamasche share the same suites.
            suit_string_ = kLatinSuit[suit_index];
            break;
        }
    }
    //LoadCardImage (folder_path);
}

int Card::Compare (const Card& other) {
    int pip = pip_,
        other_pip = other.pip_,
        suit_value = suit_value_,
        other_suit_value = other.suit_value_;

    // Function works like strcmp.

    if (pip > other_pip)
        return 1;
    if (pip < other_pip)
        return -1;

    if (suit_value > other_suit_value)
        return 1;
    if (suit_value < other_suit_value)
        return -1;
    return 0;
}

bool Card::Equals (const Card& other) {
    return !Compare (other);
}

int Card::GetPip () {
    return pip_;
}

void Card::SetValue (int newValue) {
    // The user might want to use a negative point value in a game.
    point_value_ = newValue;
}

int Card::GetFace () {
    return face_value_;
}

int Card::GetValue () {
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

    string filename = string (pip_) + "-" + string (suit_) + ".svg";

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
    switch (pip_) {
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
            cout << pip_ << " of ";
            break;
        }
    }
    cout << suit_string_;
}

}   //< namespace cards
}   //< namespace kabuki
