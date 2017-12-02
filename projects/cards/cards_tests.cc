/** Kabuki Card Games
    @file    ~/projects/cards/cards_tests.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include <stdafx.h>
#include "../../source/kabuki/cards/deck.h"
#include "../../source/kabuki/cards/blackjack_dealer.h"
#include "../../source/kabuki/cards/blackjack_player.h"
#include "../../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../../cpputest/include/CppUTest/TestHarness.h"

using namespace std;
using namespace _;
using namespace kabuki::cards;

TEST_GROUP (CARDS_TESTS) {
    void setup () {
        PrintLine (VerticalBar ());
    }

    void teardown () {
        std::cout << "\n| Test completed.\n";
        PrintLine ("+", '-');
    }
};

/*
TEST (CARDS_TESTS, BlackjackTests) {
    Deck pack ();
    BlackjackPlayer player ("Player", 100);
    BlackjackDealer dealer ();

    // 
    bool again = true;
    char input[256];


    do {
        dealer.StartNewGame ();

        do {
            dealer.Print ();

            PrintLine ('~');

            dealer.GetPlayer (1).Print ();
            //< Print player 1 to the console (we only have 2 players right now.

            PrintLine ('~');

            bool input_valid = false;

            do {
                KeyboardString ("\n| Hit or hold?", input);

                if (input == "hit") {
                    input_valid = true;

                    if (stock.IsEmpty ())
                        stock.Shuffle ();

                    player.GetHand ().Add (stock.NextCard ());
                    input_valid = true;
                } else if (input == "hold") {
                    again = false;
                } else if (input == "quit") {
                    // First we need to check for the exit condition
                {
                    input_valid = true;
                    again = false;
                } else {
                    cout << "\n| Error: invalid input. Please type 'hit', 'hold', 'quit'";
                }
            }

        } while (again);

        dealer.PlayHand ();

        int hand_comparison = player.GetHand ().Compare (dealer);

        // calculates totals
        if (player.Beats (dealer)) {
            cout << "\n|       Players wins!!!";
        } else {
            if (dealer.Beats (player)) {
                cout << "\n|       Dealer wins:(";
            }
        }
        cout << "\n| Your Score: " << player.HandTotal ()[0] << " with "
            << player.HandTotal ()[1] << " aces"
            << "\n| Computers Score: " << dealer.HandTotal ()[0] << " with "
            << dealer.HandTotal ()[1] << " ace(s).";
        KeyboardString (\n| Do you want to play again?, input);
        if (input == "y" || input == "Y")
            again = true;
    } while (again);
}

TEST (CARDS_TESTS, BlackjackHandTests) {
    cout << "\n| Testing Blackjack::BlackjackHand class..."
         << "\n| Creating deck...";

    Deck deck (Deck::Culture Deck::kFrench, Deck::kAcesHigh,
                      Deck::kNoJokers);

    CardStack deck (deck);

    deck.Reshuffle ();

    cout << "\n| Creating player_hand";

    BlackjackHand player_hand (deck.TakeNextCard (), deck.TakeNextCard ()),
        dealer_hand (deck.TakeNextCard (), deck.TakeNextCard ());

    cout << "\n| Testing String Hard::toString ()...";
    player_hand.Print ();
    dealer_hand.Print ();

    cout << "\n| Creating BlackjackHands Player{Ace of Hearts, Ace of Clubs) "
            "Dealer{ King of Hearts, Nine of Diamonds } ...";
    player_hand.SetCards ();
}

TEST (CARDS_TESTS, DealerTests) {
    cout << "\n| Testing CardStack class..."
         << "\n| Creating test_deck...";

    Deck test_deck = Deck ();

    cout << "\n| Creating cards from test_deck...";

    CardStack cards = CardStack (test_deck);

    cards.Print ();

    cout << "\n| Testing CardStack::shuffle()...";

    cards.Shuffle ();

    cards.Print ();
}

TEST (CARDS_TESTS, DealerTests) {
    cout << "\n| Testing kabuki::cards::Dealer class...";
    cout << "\n| Done testing kabuki::cards::Dealer class...";
}

TEST (CARDS_TESTS, DeckTests) {
    cout << "\n| Testing Blackjack::Hand class..." {}

void runTest () {
    cout << "\n| Creating deck...");

    Deck pack (Deck::Culture Deck::kFrench, Deck::kAcesLow, Deck::doesNotContainJokers);

    CardStack deck = CardStack (pack);

    deck.shuffle ();

    cout << "\n| Creating playerHand");

    Hand playerHand (deck.takeNextCard (), deck.takeNextCard ()),
        dealersHand (deck.takeNextCard (), deck.takeNextCard ());

    cout << "\n| Testing Hard::Print ()...");
    logMessage (playerHand.tostring);
    logMessage (dealersHand.tostring);

    cout << "\n| Creating Hands Player {Ace of Hearts, Ace of Clubs) "
            " Dealer { King of Hearts, Nine of Diamonds } ...");
    playersHand.setCards ();
}*/

TEST (CARDS_TESTS, DeckTests) {
    cout << "\n|\n| Testing kabuki::cards::Dealer class..."
         << "\n|\n| Creating test_deck...";

    Deck deck;

    for (int i = 0; i < 7; ++i) {
        cout << "\n| Creating " << Card::kSuitCulturestrings[i]
             << " Deck...";
        deck.Set (Deck::kNoJokers), Deck::kAcesLow,
            (Card::SuitCulture) i;
        deck.Print ();
    }
    cout << "\n|\n| Done testing kabuki::cards::Dealer class...";
}

TEST (CARDS_TESTS, CardComboTests) {
    cout << "\n|\n| Testing kabuki::cards::CardCombo class...";
    cout << "\n|\n| Done testing kabuki::cards::CardCombo class...";
}

TEST (CARDS_TESTS, CardTests) {
    cout << "\n|\n| Testing kabuki::cards::Card class..."
         << "\n|\n| Creating test Cards...";

    Card cards[4];

    cards[1].Set (12, Card::Suit::kClub, 12, 1);
    cards[2].Set (13, Card::Suit::kClub, 14, 1);
    cards[3].Set (12, Card::Suit::kDiamond, 12, 2);
    cards[4].Set (13, Card::Suit::kHeart, 13, 1);

    cout << "\n| Testing Print():void";
    for (int i = 0; i < 4; ++i) {
        cards[i].Print ();
    }

    cout << "\n| Testing int Compare(const Card&)...";

    CHECK (cards[1].Compare (cards[2]) < 0)
    //< "Error in int compare (const Card&) function");
    CHECK (cards[2].Compare (cards[3]) < 0)
    //< "Error in int compare (const Card&) function");
    CHECK (cards[1].Compare (cards[3]) < 0)
    //< "Error in int compare (const Card&) function");
    CHECK (cards[1].Equals (cards[4]))
    //< "Error in bool equals (const Card&) function");

    cout << "\n|\n| Testing kabuki::cards::Card class...";
}
