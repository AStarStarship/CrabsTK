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

TEST (CARDS_TESTS, DealerTests) {
    cout << "\n| Testing CardStack class..."
         << "\n| Creating testDeck...";

    Deck testDeck = Deck ();

    cout << "\n| Creating cards from testDeck...");

    CardStack cards = CardStack (testDeck);

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
    cout << "\n| Testing kabuki::cards::Dealer class..."
         << "Creating testDeck...";

    Deck deck ();
    deck.Print ();

    for (int i = 0; i < 7; ++i) {
        cout << "\n| Creating " << Card::kSuitCulturestrings[i]
             << " Deck...";
        Deck testDeck ((Card::SuitCulture) i, Deck::kAcesLow,
                       Deck::kNoJokers);
        testDeck.Print ();
    }
    //logMessage (testDeck.tostring ());
    cout << "\n| Done testing kabuki::cards::Dealer class...";
}

TEST (CARDS_TESTS, CardComboTests) {
    cout << "\n| Testing kabuki::cards::CardCombo class...";
    cout << "\n| Done testing kabuki::cards::CardCombo class...";
}

TEST (CARDS_TESTS, CardTests) {
    cout << "\n| Testing kabuki::cards::Card class..."
         << "Creating test objects...";

    const char* directory_path = "../../../resources/images/deck_art/french/vectorized-playing-cards/";

    Card test_card_1 (12, (Card::Suit)1, 12, 1, "Clubs"   , directory_path),
         test_card_2 (13, (Card::Suit)1, 14, 1, "Clubs"   , directory_path),
         test_card_3 (12, (Card::Suit)2, 12, 2, "Diamonds", directory_path),
         test_card_4 (13, (Card::Suit)1, 13, 1, "Hearts"  , directory_path);

    cout << "\n| Testing string ToString()";
    test_card_1.Print ();
    test_card_2.Print ();
    test_card_3.Print ();
    test_card_4.Print ();

    cout << "\n| Testing int Compare(const Card&)...";

    CHECK (test_card_1.Compare (test_card_2) < 0)
    //< "Error in int compare (const Card&) function");
    CHECK (test_card_2.Compare (test_card_3) < 0)
    //< "Error in int compare (const Card&) function");
    CHECK (test_card_1.Compare (test_card_3) < 0)
    //< "Error in int compare (const Card&) function");
    CHECK (test_card_1.Equals (test_card_4) < 0)
    //< "Error in bool equals (const Card&) function");

    cout << "\n| Testing kabuki::cards::Card class..."
}

void PlayBlackjack () {
    Deck pack ();
    BlackjackPlayer player = BlackjackPlayer ("Player", 100);
    BlackjackDealer dealer = BlackjackDealer ();

    // 
    bool agian = true;
    string input = "";


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
                cout << "\n| Hit or hold?";
                getline (cin, input);

                if (input == "hit") {
                    input_valid = true;

                    if (stock.IsEmpty ())
                        stock.Shuffle ();

                    player.GetHand ().AddToHand (stock.NextCard ());
                    input_valid = true;
                } else if (input == "hold") {
                    agian = false;
                } else if (input == "quit") //< First we need to check for the exit condition
                {
                    input_valid = true;
                    agian = false;
                } else {
                    cout << "\n| Error: invalid input. Please type 'hit', 'hold', 'quit'";
                }
            }

        } while (agian);

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
             << dealer.HandTotal ()[1] << " ace(s)."
             << "\n| Do you want to play again?";
        getline (cin, input);
        if (input == "y" || input == "Y")
            agian = true;
    } while (agian);
}
