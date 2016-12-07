/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Card.cpp
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

#include "Card.hpp"
#include "Deck.hpp"

namespace KabukiCardGames {

// These 4 string arrays allow us to print the names of suits in our text console app. The last 4 suit suitCultures are the Latin Suits.
const string Card::suitCulturestrings[] = { "French", "German", "Swiss-German", "Piacentine", "Napoletane", "Spagnole", "Bergamasche" };
const string Card::frenchSuit[4]      = { "Clubs", "Diamonds", "Hearts", "Spades" };
const string Card::germanSuit[4]      = { "Acorns", "Bells", "Hearts", "Lieves" };
const string Card::swissGermanSuit[4] = { "Acorns", "Bells", "Roses", "Shields" };
const string Card::latinSuit[4]       = { "Clubs", "Coins", "Cups", "Swords" };

Card::Card (int thisPipValue, Suit thisSuit)
{
    setVCard (thisPipValue, thisFaceValue, thisPointValue, thisSuitValue);
}

Card::Card (int thisPipValue, Suit thisSuit, int thisFaceValue, int thisPointValue, int thisSuitValue, SuitCulture newSuitCulture, string& imageDirectory) :
    suit (thisSuit)     //< This is called an initialization list. It sets the values of the object.
{
    setVCard (thisPipValue, thisFaceValue, thisPointValue, thisSuitValue, newSuitCulture);
    // Nothing to do here. ({:-)
}

Card::Card (const Card& other) :
    pipValue (other.pipValue),      //< Make sure you intialize the values in the same order they are in the header file!
    faceValue (other.faceValue),
    pointValue (other.pointValue),
    suitValue (other.suitValue),
    suit (other.suit),
    suitCulture (other.suitCulture),
    suitstring (other.suitstring),
    cardImage (other.cardImage)
{
    // All the copy constructor does is copy the other object. Nothing hard about this.
}

Card& Card::operator= (const Card& other)
{
    // This funciton sets all of the private values to the other object's values... its really easy.
    pipValue = other.pipValue;
    faceValue = other.faceValue;
    pointValue = other.pointValue;
    suitValue = other.suitValue;
    suit = other.suit;
    suitCulture = other.suitCulture;
    suitstring = other.suitstring;
    cardImage = other.cardImage;
}

 int Card::compare(const Card& other)
{
    int otherValue = other.pipValue,
        otherSuitValue = other.suitValue;

    if (pipValue > otherValue)
        return 1;
    if (pipValue < otherValue)
        return -1;

    // Else then the two pipValues are equal so compare the suits.
    if (suitValue > otherSuitValue)
        return 1;
    if (suitValue < otherSuitValue)
        return - 1;
    // Else then the two cards are the same suit and pipValue.
    return 0;
}
   
bool Card::equals (const Card& other)
{
    return compare (other) == 0;
}

int Card::getPipValue ()
{
    return pipValue;
}

void Card::setPointValue (int newValue)
{
    // The user might want to use a negative point value in a game.
    pointValue = newValue;
}

void Card::setVCard (int newPipValue, int newFaceValue, int newPointValue, int newSuitValue, SuitCulture newSuitCulture, string& imageDirectory)
{
    // First we have to ensure that the input values were in bounds.

    if (newPipValue < 0)            //< We can't have any negative numbers.
        pipValue = newPipValue;
    else if (newPipValue > 13)      //< Pip values range from 0 - 13 J=0, A=1, J=10, Q=11, K=13
        pipValue = 13;
    pipValue = newPipValue;

    if (newFaceValue < 0)           //< 0 is a Joker
        faceValue = 0;
    else if (newFaceValue > 14)      //< An ace can have a face value of 14 in some games.
        faceValue = 14;
    faceValue = newFaceValue;

    if (newFaceValue < 0)
        faceValue = 0;
    else if (newFaceValue > 10)
        faceValue = 10;
    faceValue = newFaceValue;

    if (newSuitValue < 1)
        suitValue = 1;
    else if (newSuitValue > 4)
        suitValue = 4;
    suitValue = newSuitValue;

    suitCulture = newSuitCulture;

    switch (newSuitCulture)
    {
        case 1:
        {
            suitstring = frenchSuit[suit];
            break;
        }
        case 2:
        {
            suitstring = germanSuit[suit];
            break;
        }
        case 3:
        {
            suitstring = swissGermanSuit[suit];
            break;
        }
        default:
        {
            suitstring = latinSuit[suit];
            break;
        }
    }
    loadCardImage (imageDirectory);
}

int Card::getFaceValue ()
{
    return faceValue;
}

int Card::getSuitValue ()
{
    return suitValue;
}

Card::Suit Card::getSuit ()
{
    return suit;
}

Card::SuitCulture Card::getSuitCulture ()
{
    return suitCulture;
}

void Card::setSuitCulture (Card::SuitCulture newSuitCulture)
{
    suitCulture = newSuitCulture;
}

string& Card::getSuitstring ()
{
    return suitstring;
}

int Card::loadCardImage (string& directory)
{
    // First, check to see if the directory is valid.
    File filePath = File (directory);

    // Next check to see if it isn't a normal file.
    if (filePath.existsAsFile ())
        return -1;

    if (!filePath.isDirectory ())
        return -1;
    // The directory is good, now check to see if the the directory contains an image with the correct naming convention.

    string cardImageFilename = string (pipValue) + "-" + string (suit) + ".svg";

    File filename = directory + cardImageFilename;

    if (!filename.existsAsFile ())
        return 2;

    Image tempImage = ImageCache::getFromFile (filename);

    if (tempImage.isNull ())
        return 3;

    cardImage = tempImage;

    return 0;
}

Image& Card::getImage ()
{
    return cardImage;
}

string Card::tostring()
{ 
    string cardstring;

    switch (pipValue)
    {
        case 0:
        {
            return suitstring;
        }
        case 1:
        {
            cardstring = "Ace of ";
            break;
        }
        case 11:
        {
            cardstring = "Jack of ";
            break;
        }
        case 12:
        {
            cardstring = "Queen of ";
            break;
        }
        case 13:
        {
            cardstring = "King of ";
            break;
        }
        default:
        {
            cardstring = string (pipValue) + " of ";
            break;
        }
    }
    cardstring += suitstring;

    return cardstring;
}




#if DEBUG_MODE

class CardUnitTest : public UnitTest
{
public:

    CardUnitTest () : UnitTest ("Testing KabukiCardGames::Card class...") {}

    void runTest ()
    {
        logMessage ("Creating test objects...");

        string imagesDirectory = "../../../Images/DeckArt/French/vectorized-playing-cards/";

        Card testCard1 (12, 12, 1, (Card::Suit)1, "Clubs", imagesDirectory),
            testCard2 (13, 14, 1, (Card::Suit)1, "Clubs", imagesDirectory),
            testCard3 (12, 12, 2, (Card::Suit)2, "Diamonds", imagesDirectory),
            testCard4 (13, 13, 1, (Card::Suit)1, "Hearts", imagesDirectory);

        beginTest ("Testing string tostring()");
        logMessage (testCard1.tostring ());
        logMessage (testCard2.tostring ());
        logMessage (testCard3.tostring ());
        logMessage (testCard4.tostring ());

        beginTest ("Testing int compare(const Card&)...");

        expect (testCard1.compare (testCard2) < 0, "Error in int compare (const Card&) function");
        expect (testCard2.compare (testCard3) < 0, "Error in int compare (const Card&) function");
        expect (testCard1.compare (testCard3) < 0, "Error in int compare (const Card&) function");

        expect (testCard1.equals (testCard4) < 0, "Error in bool equals (const Card&) function");
    }
};

}   //< KabukiCardGames
