/** kabuki:cards
    @file    ~/source/kabuki/cards/deck_.cc
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

#include "deck.h"
#include "card_stack.h"

namespace kabuki { namespace cards {

const int Deck::kDefaultSuitValues[4] = { 1, 2, 3, 4 };
/** The default deck_ art directory. */
const char Deck::kDefaultDeckArtDirectory[] = "./\0";

/** The default rear deck_ image. */
const char Deck::kDefaultRearDeckImage[] = "card.png\0";

Deck::Deck (bool has_jokers, int aces_high, Card::SuitCulture culture,
            const char* deck_name, const char* directory_path) :
    has_jokers_    (has_jokers),
    culture_       (culture),
    aces_high_     (aces_high),
    num_cards_     (has_jokers ? kFullDeckSize : kDefaultNumCards),
    lowest_value_  (aces_high == 0 ? 1 : 2),
    highest_value_ (aces_high == 0 ? 13 : 14) {

    // Depending on if aces are high, the value of an Ace will be either 1
    // followed by the 2 card, or 14.
    int start_value = aces_high_?1:2;
    const char* culture_string = Card::kSuitCulturestrings[culture_];

    // First we want to start by creating the Aces because their pipValue is 1,
    // but might have a faceValue of 14.
    int aceValue = aces_high_ ? 14 : 1;

    // This is a nested for loop. It makes it so that our deck_ will be sorted
    // by suit and value.
    for (int suit = 0; suit <= 3; ++suit) { // If we had switched the order of
        // the for loops, it would sort it by value then suit.
        for (int pip = 1; pip <= 13; ++pip) {
            // There are 13 different face values { A,2,3,4,5,6,7,8,9,10,J,Q,K }
            pack_[(pip - 1) * 13 + suit].Set (pip, (Card::Suit)suit, pip, pip, suit, culture);
            // The (Card::Suit) "casts" the suitValue to a Card::Suit.
        }
    }

    //rearImage = ImageCache::getFromFile (deck_name);

    //if (rearImage.isNull ())    //< null means that there was an error so try to load the default deck_.
    //    ImageCache::getFromFile (defaultRearDeckImage);
}

bool Deck::Set (bool has_jokers, int aces_high, Card::SuitCulture culture,
                const char* deck_name, const char* directory_path) {
    has_jokers_    = has_jokers;
    culture_       = culture;
    aces_high_     = aces_high;
    num_cards_     = has_jokers ? kFullDeckSize : kDefaultNumCards;
    lowest_value_  = aces_high == 0 ? 1 : 2;
    highest_value_ = aces_high == 0 ? 13 : 14;

    // Depending on if aces are high, the value of an Ace will be either 1
    // followed by the 2 card, or 14.
    int start_value = aces_high_?1:2;
    const char* culture_string = Card::kSuitCulturestrings[culture_];

    // First we want to start by creating the Aces because their pipValue is 1,
    // but might have a faceValue of 14.
    int aceValue = aces_high_ ? 14 : 1;

    // This is a nested for loop. It makes it so that our deck_ will be sorted
    // by suit and value.
    for (int suit = 0; suit <= 3; ++suit) { // If we had switched the order of
        // the for loops, it would sort it by value then suit.
        for (int pip = 1; pip <= 13; ++pip) {
            // There are 13 different face values { A,2,3,4,5,6,7,8,9,10,J,Q,K }
            pack_[(pip - 1) * 13 + suit].Set (pip, (Card::Suit)suit, pip, pip, suit, culture);
            // The (Card::Suit) "casts" the suitValue to a Card::Suit.
        }
    }

    //rearImage = ImageCache::getFromFile (deck_name);

    //if (rearImage.isNull ())    //< null means that there was an error so try to load the default deck_.
    //    ImageCache::getFromFile (defaultRearDeckImage);
    return true;
}

void Deck::SetSuitCulture (Card::SuitCulture culture) {
    if (culture > Card::kBergamasche) {
        return;
    }
    culture_ = culture;
}

void Deck::Reshuffle () {
    
}

bool Deck::SetSuitValues (int value1, int value2, int value3, int value4) {
    if (value1 < 1)
        return false;
    if (value1 > 4)
        return false;

    if (value2 < 1)
        return false;
    if (value2 > 4)
        return false;

    if (value3 < 1)
        return false;
    if (value3 > 4)
        return false;

    if (value4 < 1)
        return false;
    if (value4 > 4)
        return false;

    suit_values_[1] = value1;
    suit_values_[2] = value2;
    suit_values_[3] = value3;
    suit_values_[4] = value4;
    return true;
}

int Deck::GetSuitValue (int suit) {
    if (suit < 1 || suit > 4)
        return -1;
    return suit_values_[suit];
}

bool Deck::HasJokers () {
    return has_jokers_;
}

int Deck::GetCount () {
    return num_cards_;
}

bool Deck::AcesHigh () {
    return aces_high_;
}

Card* Deck::GetCard (int index) {
    if (index < 0) {
        return nullptr;
    }
    if (index >= num_cards_) {
        return nullptr;
    }
    return &pack_[index];
}
/*
int Deck::SetRearImage (const File& thisFile) {
    Image tempImage = ImageCache::getFromFile (thisFile);

    if (tempImage.isNull)
        return 0;

    rearImage = tempImage;

    return 1;
}

int Deck::checkDeckArtFolder (const char* directory_path) {
    // Make a file from the directory_path string.
    File artFolder = File (directory_path);

    // Before we go trying to change all of the images, lets first check to
    // see if the directory_path is a valid folder.
    if (!artFolder.isDirectory ())
        return 1;

    // Next, we need to check to see if the folder contains 54 .svg images.
    Array<File> cardImageFiles;
    if (54 != artFolder.FindChildFiles (cardImageFiles,
                                 File::TypesOfFileToFind::FndFiles, false, ".svg"))
        return 54;
    // Note: I'm assuming that the directory_path is sorted by name.
    // Then check to see if all of the images were named correctly.
    for (int suit = 1; suit <= 4; ++suit) {
        for (int pipValue = 1; pipValue <= 13; ++pipValue) {
            int index = suit*pipValue;

            string expectedFileName = string (pipValue) + "-" + string (suit) + ".svg"; // Examples "1-2.svg" = Ace of Diamond "13-4" = King of Spades

            if (cardImageFiles[index].getFileName () != expectedFileName)
                return -1 * index;
        }
    }
    if (cardImageFiles[52].getFileName () != "0-1.svg")
        return -53;
    if (cardImageFiles[53].getFileName () != "0-2.svg")
        return -54;
}

int Deck::setDeckArt (const char* directory_path) {
    int returnValue = Deck::checkDeckArtFolder ();

    // Now we know that all of the files are in the directory and are named correctly. Now lets actually change the images.
    for (int suit = 1; suit <= 4; ++suit) {
        for (int pipValue = 1; pipValue <= 13; ++pipValue) {
            int index = suit*pipValue;
            string filename = string (pipValue) + "-" + string (suit) + ".svg";

            int returnValue = deck_[index]->setCardImage (ImageCache::getFromFile (filename));

            if (returnValue != 0)
                return returnValue;
        }
    }
    return 0;
}*/

const char* Deck::GetSuitstring (Card::Suit suit) {
    switch (culture_) {
        case Card::kFrench:
        {
            switch (suit) {
                case Card::Suit::kHeart  : return "Hearts";
                case Card::Suit::kDiamond: return "Diamonds";
                case Card::Suit::kClub   : return "Clubs";
                case Card::Suit::kSpade  : return "Spades";
            }
            break;
        }
        case Card::kGerman:
        {
            switch (suit) {
                case Card::Suit::kHeart: return "Hearts";
                case Card::Suit::kBell : return "Bells";
                case Card::Suit::kAcorn: return "Acorns";
                case Card::Suit::kLief : return "Leaves";
            }
            break;
        }
        case Card::kSwissGerman:
        {
            switch (suit) {
                case Card::Suit::kRose  : return "Roses";
                case Card::Suit::kBell  : return "Bells";
                case Card::Suit::kAcorn : return "Acorns";
                case Card::Suit::kShield: return "Shields";
            }
            break;
        }
        default: // Then its a Latin suit.
        {
            switch (suit) {
                case Card::Suit::kCup  : return "Cups";
                case Card::Suit::kCoin : return "Coins";
                case Card::Suit::kClub : return "Clubs";
                case Card::Suit::kSword: return "Swords";
            }
        }
    }
    return nullptr; //< This should never happen.
}

void Deck::Print () {
    cout << "\n| Deck: num_cards_: " << num_cards_ << ", " 
         << (aces_high_ ? "Aces high, " : "Aces low, ") 
         << (aces_high_ ? "Has Jokers" : "No Jokers");

    for (int i = 0; i < num_cards_; ++i)
        pack_[i].Print ();
}

}   //< namespace cards
}   //< namespace kabuki
