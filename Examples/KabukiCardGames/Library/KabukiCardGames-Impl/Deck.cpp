/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Deck.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include "Deck.hpp"
#include "CardStack.hpp"

namespace KabukiCardGames {

const int Deck::defaultSuitValues[4] = { 1, 2, 3, 4 };

const string Deck::defaultDeckArtDirectory = "../../../Images/FrontDeckArt/vectorized-playing-cards/";
const string Deck::defaultRearDeckImage = "../../../Images/RearDeckArt/CoderDojoDeckRearImage.svg";

const Deck Deck::standard = Deck ();
const Deck Deck::standardWithJokers = Deck (false);

Deck::Deck (bool deckContainsJokers, int acesAreHigh, Card::SuitCulture thisSuitCulture, string& rearDeckImageFilename, const string& deckArtDirectory) :
    hasJokers (deckContainsJokers),
    acesHigh (acesAreHigh),
    suitCulture (thisSuitCulture),
    numCards (deckContainsJokers ? defaultNumCardsWithJokers : defaultNumCards),
    lowestCardValue (acesAreHigh == 0 ? 1 : 2),
    highestCardValue (acesAreHigh == 0 ? 13 : 14)
{
    initialise (rearDeckImageFilename, deckArtDirectory);
}

void Deck::initialise (string& rearDeckImageFilename, const string& deckArtDirectory)
{
    if (deck.size () == 0)
        deck.clear ();
    // Depending on if aces are high, the value of an Ace will be either 1 followed by the 2 card, or 14.
    int startValue = acesHigh ? 1 : 2;
    const string& suitCulturestringAddress = Card::suitCulturestrings[suitCulture];

    // First we want to start by creating the Aces because their pipValue is 1, but might have a faceValue of 14.
    int aceValue = acesHigh ? 14 : 1;

    for (int suit = 1; suit <= 4; ++suit)
        deck.add (new Card (1, 1, aceValue, suitValues[suit], (Card::Suit) suit, Card::suitCulturestrings[suit], deckArtDirectory);

    // This is a nested for loop. It makes it so that our deck will be sorted by suit and value.
    for (int suit = 1; suit <= 4; ++suit) // If we had switched the order of the for loops, it would sort it by value then suit.
        for (int cardValue = 2; cardValue <= 13; ++cardValue) // There are 13 different face values { 2,3,4,5,6,7,8,9,10,J,Q,K,A }
            deck.add (new Card (cardValue, (Card::Suit)suit, this)); // The (Card::Suit) "casts" the suitValue to a Card::Suit.

    rearImage = ImageCache::getFromFile (rearDeckImageFilename);

    if (rearImage.isNull ())    //< null means that there was an error so try to load the default deck.
        ImageCache::getFromFile (defaultRearDeckImage);
}



Deck& Deck::operator= (const Deck& other)
{
}

int Deck::setSuitValues (int value1, int value2, int value3, int value4)
{
    if (value1 < 1)
        return -1;
    if (value1 > 4)
        return 1;

    if (value2 < 1)
        return -2;
    if (value2 > 4)
        return 2;

    if (value3 < 1)
        return -3;
    if (value3 > 4)
        return 3;

    if (value4 < 1)
        return -4;
    if (value4 > 4)
        return 4;

    suitValues[1] = value1;
    suitValues[2] = value2;
    suitValues[3] = value3;
    suitValues[4] = value4;
}

int Deck::getSuitValue (int suit)
{
    if (suit < 1 || suit > 4)
        return -1;
    return suitValues[suit];
}

bool Deck::deckHasJokers ()
{
    return hasJokers;
}

int Deck::getNumCards ()
{
    return numCards;
}

Card* Deck::getCard (int index)
{
    if (index < 0 || index >= deck.size ())
        return nullptr;
    return deck[index];
}

int Deck::setRearImage (const File& thisFile)
{
    Image tempImage = ImageCache::getFromFile (thisFile);

    if (tempImage.isNull)
        return 0;

    rearImage = tempImage;

    return 1;
}

int Deck::checkDeckArtFolder (string& deckArtFolder)
{
    File artFolder = File (deckArtFolder); //< Make a file fromt he deckArtFolder string.

    // Before we go trying to change all of the images, lets first check to see if the deckArtFolder is a valid folder.
    if (!artFolder.isDirectory ())
        return 1;

    // Next, we need to check to see if the folder contains 54 .svg images.
    Array<File> cardImageFiles;
    if (artFolder.findChildFiles (cardImageFiles, File::TypesOfFileToFind::findFiles, false, ".svg") != 54)
        return 54;
    // Note: I'm assuming that the deckArtFolder is sorted by name.
    // Then check to see if all of the images were nammed correctly.
    for (int suit = 1; suit <= 4; ++suit)
    {
        for (int pipValue = 1; pipValue <= 13; ++pipValue)
        {
            int index = suit*pipValue;

            string expectedFileName = string (pipValue) + "-" + string (suit) + ".svg"; // Examples "1-2.svg" = Ace of Diamons "13-4" = King of Spades

            if (cardImageFiles[index].getFileName () != expectedFileName)
                return -1 * index;
        }
    }
    if (cardImageFiles[52].getFileName () != "0-1.svg")
        return -53;
    if (cardImageFiles[53].getFileName () != "0-2.svg")
        return -54;
}

int Deck::setDeckArt (string& deckArtFolder)
{
    int returnValue = Deck::checkDeckArtFolder ();

    // Now we know that all of the files are in the directory and are nammed correctly. Now lets actually change the images.
    for (int suit = 1; suit <= 4; ++suit)
    {
        for (int pipValue = 1; pipValue <= 13; ++pipValue)
        {
            int index = suit*pipValue;
            string filename = string (pipValue) + "-" + string (suit) + ".svg";

            int returnValue = deck[index]->setCardImage (ImageCache::getFromFile (filename));

            if (returnValue != 0)
                return returnValue;
        }
    }
    return 0;
}

string Deck::getSuitstring (Card::Suit suit)
{
    switch (suitCulture)
    {
        case Card::french:
        {
            switch (suit)
            {
            case 1: return "Clubs";
            case 2: return "Diamonds";
            case 3: return "Hearts";
            case 4: return "Spades";
            }
            break;
        }
        case Card::german:
        {
            switch (suit)
            {
            case 1: return "Acorns";
            case 2: return "Bells";
            case 3: return "Hearts";
            case 4: return "Lieves";
            }
            break;
        }
        case Card::swissGerman:
        {
            switch (suit)
            {
            case 1: return "Acrons";
            case 2: return "Bells";
            case 3: return "Roses";
            case 4: return "Shields";
            }
            break;
        }
        default: // Then its a Latin suit.
        {
            switch (suit)
            {
            case 1: return "Clubs";
            case 2: return "Coins";
            case 3: return "Cups";
            case 4: return "Swords";
            }
        }
    }
}

string Deck::tostring ()
{
    string returnstring = "Deck: numCards: ";

    returnstring += deck.size (); +", ";
    returnstring += acesHigh ? "Aces high, " : "Aces low, ";
    returnstring += acesHigh ? "Has Jokers\n" : "No Jokers\n";
    
    for (int i = 0; i < deck.size (); ++i)
        returnstring += deck[i]->tostring () + "\n";
    return returnstring;
}

#if DEBUG_MODE

class DeckUnitTest : public UnitTest
{
public:
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    DeckUnitTest () : UnitTest ("Testing KabukiCardGames::Deck class...") {}

    void runTest ()
    {
        logMessage ("Creating testDeck...");

        Deck testDefaultDeck = Deck ();
        
        logMessage (testDefaultDeck.tostring ());

        for (int i = 0; i < 7; ++i)
        {
            logMessage ("Creating " + Deck::suitCulturestrings[i] + " Deck...");
            Deck testDeck ((Deck::SuitCulture) i, Deck::acesLow, Deck::doesNotContainJokers);
            logMessage (testDeck.tostring ());
        }
        //logMessage (testDeck.tostring ());
    }
};
static DeckUnitTest deckUnitTest;
#endif // DEBUG_MODE

}   //< KabukiCardGames
