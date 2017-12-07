/** kabuki:cards
    @file    ~/source/project/kabuki/cards_suit.cc
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

#include "suit.h"

namespace kabuki { namespace cards {

// These 4 string arrays allow us to print the names of suits in our text 
// console app. The last 4 suit_ suitFormats are the Latin Suits.
const char* Suit::kFrenchSuit[4] = { "Hearts", "Diamonds", "Clubs",
                                     "Spades" };
const char* Suit::kGermanSuit[4] = { "Hearts", "Bells", "Acorns",
                                     "Leaves" };
const char* Suit::kSwissGermanSuit[4] = { "Roses", "Bells", "Acorns",
                                          "Shields" };
const char* Suit::kLatinSuit[4] = { "Cups", "Coins", "Clubs",
                                    "Swords" };
const char* Suit::kFormatStrings[] = { "French", "German", "Swiss-German",
                                       "Piacentine", "Napoletane",
                                       "Spagnole", "Bergamasche" };

Suit::Suit (int column, int denomination, Format format, Color color) {
    if (column < 0) {
        column_ = 0;
    } else if (column > 3) {
        column_ = 3;
    } else {
        column_ = column;
    }
    Set (denomination, format, color);
}

void Suit::Set (int denomination, Format format, Color color) {
    // There are no rules for denomination as far as I can think of.
    denomination_ = denomination;
    color_ = color;
    SetFormat (format);
    SetColor (color);
}

int Suit::Compare (const Suit* other) {
    if (other == nullptr) {
        return denomination_;
    }
    return other->denomination_ - denomination_;
}

bool Suit::Equals (const Suit* other) {
    return !Compare (other);
}

int Suit::GetDenomination () {
    return denomination_;
}

void Suit::SetDenomination (int value) {
    // The user might want to use a negative point value in a game.
    denomination_ = value;
}

wchar_t Suit::GetChar () {
    return char_;
}

Suit::Color Suit::GetColor () {
    return color_;
}

void Suit::SetColor (Color color) {
    color_ = color;
    switch (column_) {
        case 0: { // Heart
            char_ = (color == Color::kBlack)?kBlackHeart:kWhiteHeart;
            return;
        }
        case 1: { // Diamond
            char_ = (color == Color::kBlack)?kBlackDiamond:kWhiteDiamond;
            return;
        }
        case 2: { // Club
            char_ = (color == Color::kBlack)?kBlackClub:kWhiteClub;
            return;
        }
        case 3: { // Spade
            char_ = (color == Color::kBlack)?kBlackSpade:kWhiteSpade;
            return;
        }
    }
}

const char* Suit::GetLabel () {
    return label_;
}

void Suit::SetFormat (Format format) {
    switch (format) {
        case Format::kFrench: {
            label_ = kFrenchSuit[column_];
            return;
        }
        case Format::kGerman: {
            label_ = kGermanSuit[column_];
            return;
        }
        case Format::kSwissGerman: {
            label_ = kSwissGermanSuit[column_];
            return;
        }
    }
    //< kNapoletane, kSpagnole, and kBergamasche are all Latin formats.
    label_ = kLatinSuit[column_];
}

/*
int Suit::LoadCardImage (const char* directory) {
    // First, check to see if the directory is valid.
    File filePath = File (directory);

    // Next check to see if it isn't a normal file.
    if (filePath.existsAsFile ())
        return -1;

    if (!filePath.isDirectory ())
        return -1;
    // Directory is good, now check to see if the directory contains an image
    // with the correct naming convention.

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

//Image& Suit::GetImage () {
//    return card_image_;
//}

void Suit::Print () {
    cout << label_;
}

Suit* SuitError () {
    static Suit error (Suit::kSpade);
    return &error;
}

}   //< namespace cards
}   //< namespace kabuki
