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

using namespace std;

namespace kabuki { namespace cards {

Card::Card (int pip, Suit* suit, int denomination) {
    Set (pip, suit, denomination);
}

Card::Card (const Card& other) :
    pip_          (other.pip_),
    denomination_ (other.denomination_),
    suit_         (other.suit_) {
    // Nothing to do here. :-)
}

void Card::Set (int pip, Suit* suit, int denomination) {
    // First we have to ensure that the input values were in bounds.
    //< Pip values range from 0 - 13 J=0, A=1, J=10, Q=11, K=13
    if (pip < 0) {
        pip = 0;
    } else if (pip > kKing) {
        pip = 13;
    }
    pip_ = pip;
    suit = (suit == nullptr) ? SuitError () : suit;
    suit_ = suit;
    int offset = 13 * suit->GetDenomination ();
    denomination_ = offset + ((denomination == ~0) ? pip : denomination);
    //LoadCardImage (folder_path);
}

int Card::Compare (Card* other) {
    return other->denomination_ - denomination_;
}

bool Card::Equals (Card* other) {
    return !Compare (other);
}

int Card::GetPip () {
    return pip_;
}

int Card::GetDenomination () {
    return denomination_;
}

void Card::SetDenomination (int value) {
    // The user might want to use a negative point value in a game.
    denomination_ = value;
}

Suit* Card::GetSuit () {
    return suit_;
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

//Image& Card::GetImage () {
//    return card_image_;
//}

Card& Card::operator= (const Card& other) {
    if (this == &other) { // self-assignment check expected
        return *this;
    }
    pip_          = other.pip_;
    denomination_ = other.denomination_;
    suit_         = other.suit_;
    return *this;
}

void Card::Print () {
    switch (pip_) {
        case 0:
        {
            cout << ((suit_->GetColor () == Suit::Color::kBlack) ? "Black Joker" 
                                                                 : "White Joker");
            break;
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
    cout << suit_->GetLabel ();
}

}   //< namespace cards
}   //< namespace kabuki
