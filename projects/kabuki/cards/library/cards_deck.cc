/** Kabuki Arcade
    @file    ~/source/kabuki/arcade/cards/deck_.cc
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

#include "deck.h"
#include "card_stack.h"

using namespace std;

namespace kabuki { namespace cards {

const SI4 Deck::kDefaultSuitValues[4] = { 1, 2, 3, 4 };
/** The default deck_ art directory. */
const CH1 Deck::kDefaultDeckArtDirectory[] = "./\0";

/** The default rear deck_ image. */
const CH1 Deck::kDefaultRearDeckImage[] = "card.png\0";

Deck::Deck (SI4 num_decks, BOL has_jokers, SI4 aces_high, Suit::Format format,
            const CH1* deck_name, const CH1* directory_path,
            Suit::Color color) :
    has_jokers_    (has_jokers),
    num_decks_     ((num_decks < 1) ? 1 : num_decks),
    num_cards_     (num_decks_ * (has_jokers ? kFullDeckSize : kDefaultDeckSize)),
    aces_high_     (aces_high),
    lowest_value_  (aces_high == 0 ? 1 : 2),
    highest_value_ (aces_high == 0 ? 13 : 14),
    heart_         (Suit::kHeart  , 0),
    diamond_       (Suit::kDiamond, 1),
    club_          (Suit::kClub   , 2),
    spade_         (Suit::kSpade  , 3),
    pack_          () {
    pack_.reserve (num_cards_);
    suits_[0] = &heart_;
    suits_[1] = &diamond_;
    suits_[2] = &club_;
    suits_[3] = &spade_;
    if (num_decks < 1) {
        num_decks = 1;
    }

    Set (has_jokers, aces_high, format, deck_name, directory_path, color);
}

Deck::~Deck () {
    for (size_t i = 0; i < pack_.size (); ++i) {
        delete pack_[i];
    }
}

void Deck::Set (BOL has_jokers, SI4 aces_high, Suit::Format format,
                const CH1* deck_name, const CH1* directory_path,
                Suit::Color color) {
    has_jokers_    = has_jokers;
    aces_high_     = aces_high;
    lowest_value_  = aces_high == 0 ? 1 : 2;
    highest_value_ = aces_high == 0 ? 13 : 14;

    // Depending on if aces are high, the value of an Ace will be either 1
    // followed by the 2 card, or 14.
    SI4 start_value = aces_high_?1:2;
    const CH1* format_string = Suit::kFormatTexts[format];

    // First we want to start by creating the Aces because their pipValue is 1,
    // but might have a faceValue of 14.
    SI4 aceValue = aces_high_ ? 14 : 1;

    pack_.clear ();
    pack_.reserve (num_cards_);
    // This is a nested for loop. It makes it so that our deck_ will be sorted
    // by suit and value.
    SI4 i = 0;
    for (SI4 i = 0; i < num_decks_; ++i) {
        for (SI4 suit = 0; suit <= 3; ++suit) { // If we had switched the order of
            // the for loops, it would sort it by value then suit.
            Suit* suit_ptr = suits_[suit];
            //suit_ptr->Set (suit, format);
            for (SI4 pip = 1; pip <= 13; ++pip) {
                // There are 13 different face values { A,2,3,4,5,6,7,8,9,10,J,Q,K }
                pack_.emplace_back (new Card (pip, suit_ptr));
                // The (Card::Suit) "casts" the suitValue to a Card::Suit.
            }
        }
        if (has_jokers_) {
            pack_.emplace_back (new Card (Card::kJoker, suits_[0]));
            pack_.emplace_back (new Card (Card::kJoker, suits_[2]));
        }
    }

    //rearImage = ImageCache::getFromFile (deck_name);

    //if (rearImage.isNull ()) {
    //    // null means that there was an error so try to load the default deck.
    //    ImageCache::getFromFile (defaultRearDeckImage);
    //}
}

void Deck::SetFormat (Suit::Format format) {
    heart_.SetFormat   (format);
    diamond_.SetFormat (format);
    club_.SetFormat    (format);
    spade_.SetFormat   (format);
}

BOL Deck::HasJokers () {
    return has_jokers_;
}

SI4 Deck::GetSize ( ) {
    return num_cards_;
}

BOL Deck::AcesHigh () {
    return aces_high_;
}

Card* Deck::GetCard (SI4 suit, SI4 pip) {
    if (suit < 0) {
        return pack_[0];
    }
    SI4 index;
    if (pip == Card::kJoker) {
        if (!has_jokers_) { // Error!
            return nullptr;
        }
        if (suit <= 1) { // It's a black joker.
            return pack_[52];
        }
        // It's a red joker.
        // @todo $CaleMcCollough Make this work with multiple decks.
        return pack_[53];
    } 
    index = 13 * suit + pip;
    return pack_[index];
}

Card* Deck::GetCard (SI4 index) {
    if (index < 0) {
        return pack_[0];
    }
    if (index >= num_cards_) {
        return pack_[num_cards_ - 1];;
    }
    return pack_[index];
}

/*
SI4 Deck::SetRearImage (const File& thisFile) {
    Image tempImage = ImageCache::getFromFile (thisFile);

    if (tempImage.isNull)
        return 0;

    rearImage = tempImage;

    return 1;
}

SI4 Deck::checkDeckArtFolder (const CH1* directory_path) {
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
    for (SI4 suit = 1; suit <= 4; ++suit) {
        for (SI4 pipValue = 1; pipValue <= 13; ++pipValue) {
            SI4 index = suit*pipValue;

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

SI4 Deck::SetDeckArt (const CH1* directory_path) {
    SI4 returnValue = Deck::checkDeckArtFolder ();

    // Now we know that all of the files are in the directory and are named
    // correctly. Now lets actually change the images.
    for (SI4 suit = 1; suit <= 4; ++suit) {
        for (SI4 pipValue = 1; pipValue <= 13; ++pipValue) {
            SI4 index = suit*pipValue;
            string filename = string (pipValue) + "-" + string (suit) + ".svg";

            SI4 returnValue = deck_[index]->setCardImage (ImageCache::getFromFile (filename));

            if (returnValue != 0)
                return returnValue;
        }
    }
    return 0;
}*/

Suit* Deck::Heart () {
    return &heart_;
}

Suit* Deck::Diamond () {
    return &diamond_;
}

Suit* Deck::Club () {
    return &club_;
}

Suit* Deck::Spade () {
    return &spade_;
}
Suit** Deck::Suits () {
    return suits_;
}

void Deck::SetSuitDenominations (SI4 column_0, SI4 column_1, SI4 column_2,
                                 SI4 column_4) {
    heart_.SetDenomination   (column_0);
    diamond_.SetDenomination (column_1);
    club_.SetDenomination    (column_2);
    spade_.SetDenomination   (column_4);
}

void Deck::Print (_::Text& txt) {
    cout << "\n| Deck: num_cards_: " << pack_.size () << ", "
         << (aces_high_ ? "Aces high, " : "Aces low, ") 
         << (aces_high_ ? "Has Jokers" : "No Jokers");

    for (size_t i = 0; i < pack_.size (); ++i) {
        cout << "\n| " << i << ": ";
        Card* card = pack_[i];
        card->Print (txt);
    }
}

}   //< namespace cards
}   //< namespace kabuki
