/** Kabuki Card Games
    @file    /.../KabukiSDK-Examples/KabukiCardGames/KabukiCardGames/Deck.hpp
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

#pragma once

#include "Card.hpp"

namespace KabukiCardGames {
    
/** Class that reprents a Deck of Cards.
    A Deck of cards usually has 52 cards, but can also have 2 jokers, a black one, and a red one. Not all card games have this many cards, so
    this class is setup for you to create decks of cards for just about any card game you can think of.
*/
class Deck
{
public:

    static const int defaultNumCards = 52,              //< The default number of playing cards in a deck excluding Jokers.
        defaultNumCardsWithJokers = 54,                 //< The default number of playing cards in a deck including Jokers.
        acesHigh = 14,                                  //< Flag represents if Aces are high (i.e. has a face value of 14).
        acesLow = 0,                                    //< Flag represents if Aces are low (i.e. has a face value of 1).
        acesHighOrLow = 1,                              //< Flag represents if Aces can be either high or low (i.e. has a point value of 1 or 14).
        defaultSuitValues[4];                           //< The default suit value order, Clubs=1, Diamonds=2, Hearts=3, and Spades=4.

    static const bool hasJokers = true,                 //< Flag for if this deck contains Jokers.
        hasNoJokers = false;                            //<- Flag for if this deck doesn't contains Jokers.

    static const Deck standard,                         //< A standard deck of playing cards without Jokers.
        standardWithJokers;                             //< A standard deck of playing cards with Jokers.

    static const string defaultDeckArtDirectory,        //< The default deck art directory.
        defaultRearDeckImage;                           //< The default rear deck image.
    
    Deck (bool deckContainsJokers = hasNoJokers, int acesAreHigh = acesHigh,  
        Card::SuitCulture thisCardSuitCulture = Card::french, 
        string& rearDeckImageFilename = string (defaultRearDeckImage), 
        const string& deckArtDirectory = defaultDeckArtDirectory);
    /*< Constructor.
        In most card games, jokers are no used, and aces are high, so we use those are the default values.
        @param  deckContainsJokers True if the deck continas jokers, and false otherwise.
        @param  acesAreHigh 0 means that aces are low,
                    a positive number means that aces are high,
                    a negative number means that aces are low.
        @param  thisCardSuitCulture This paramter is used to determine what suites we are using.
    */
        
    virtual ~Deck () {}
    //< Destructor.

    Deck& operator= (const Deck& other);
    //< Operator= overlaoder deep copies the state of the other object.
    
    int setSuitValues (int value1, int value2, int value3, int value4);
    /*< Function sets the suit values to the new values.
        @pre    valueN must be between 1-4.
        @return Returns 0 upon success, and a number 1-4 if the valueN is not between 1-4. The number will be negative if the inputed number was, and vice-versa. */
    
    int getSuitValue (int suit);
    /*< Gets the suit value of the given suit.
        @pre    suit must be between 1-4.
        @return Returns a number between 1-4 upon sucess, and -1 if the index was out of bounds. */
    bool deckHasJokers ();
    //< Returns true if this deck has Jokers.

    int getNumCards ();
    //< Returns the number of cards in the deck.
    int getAcesHigh ();
    //< Returns the if aces are high or low.
    Card* getCard (int index);
    /*< Returns a pointer to the Card at the spcified index.
        @return Returns nullptr if the index is out of bounds. */

    int setRearImage (const File& thisFile);
    /*< Sets the frontImage to a new Image from thisFile.
        @pre    thisFile must be a valid existing Image file.
        @return returns -1 if thisFile is not a valid Image. */
        
    int setDeckArt (string& deckArtFolder);
    /*< Loads the art from the deckArtFolder.
        @pre    deckArtFolder must contain all 54 card Images in [1-13]-[1-4].svg/J-1.svg/J-2.svg format format.
        @return Returns 0 upon success, 54 if the directory doesn't contain 54 .svg files, and -x if images are not named correctly, where x is the first failed filename. */
        
    string getSuitstring (Card::Suit suit);
    //< Returns a string representation of the suit.
    
    string tostring ();
    //< Returns a string representation of this Object.

private:

    /** Function initializes the deck with a traditional set of playing cards. */
    void initialise (string& rearDeckImageFilename, const string& deckArtDirectory);

    int checkDeckArtFolder (string& deckArtFolder);     //< Function that checks the artFolder to see if the it has 54 Images in [1-13]-[1-4].svg/J-1.svg/J-2.svg format.

    bool hasJokers;                                     //< Variable represents if this deck has Jokers.

    Card::SuitCulture suitCulture;                      //< The culture of the suits.

    int acesHigh,                                       /*< Variable represeents if this deck is aces high, low, or eighter.
                                                            Aces high (acesHigh > 0) means that an ace is worth 14 points. Aces low (acesHigh == 0) means that it is worth 1.
                                                            In some games, like blackjack, aces can be either high or low (acesHigh < 0), meaning that they can be worth either a 1, or a 14. */
        numCards,                                       //< The number of cards in the deck.
        lowestCardValue,                                //< The lowest allowed card value.
        highestCardValue,                               //< The highest allowed card value.
        suitValues[4];                                  /*< An array of 4 numbers that represent the values of each suit.
                                                            Used for changing the order of the suits */
    Image rearImage;                                    //< The rear Image of the Deck.

    OwnedArray<Card> deck;                              //< The deck of Card objects.
};

}   //< KabukiCardGames
