/** kabuki::cards Tests
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
#include "../../source/kabuki/cards/blackjack_dealer.h"
#include "../../source/kabuki/cards/blackjack_player.h"
#include "../../source/kabuki/cards/blackjack_game.h"
#include "../../source/kabuki/script/global.h"
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
        PrintLine ("|", '-');
    }
};

TEST (CARDS_TESTS, BlackjackTests) {
    
    enum {
        kNameBufferSize = 64,
        kInputBufferSize = 256
    };
    char name[kNameBufferSize];

    KeyboardString ("\n< Please Entire your name: ", name, kNameBufferSize);

    Deck pack;
    CardStack stock (pack);

    BlackjackDealer dealer;
    BlackjackPlayer player (dealer.GetStock (), name);
    /**

    bool play_again = true;
    char input[kInputBufferSize];

    CardGame* game;

    cout << "\n|\n|";
    PrintLine ("|", '-');
    cout << "\n| Starting a game of " << game->GetName ();

    // Play-again loop.
    // A play-again loop goes in a loop where the user is asked if they want to
    // play again. The user can
    // enter "hit", "hold", "exit", or "quit". The code for the game.
    while (play_again && game->PlayGameInConsole ()) {  // Main game loop.
        
        KeyboardString (DefaultPlayAgainString (), input, kInputBufferSize);

        bool input_valid = false;

        while (!input_valid) {
            KeyboardString (DefaultPlayAgainString () , input, kInputBufferSize);

            // If the user wants to continue, they need to type the letter y, or
            // else we need to exit the program loop.
            // We need to convert the input to lower case letters just in case
            // someone capitalizes any of the letters.
            
            if (input == "yes")     //  Keep playing.
            {
                // again is current true.
                input_valid = true;
            } else if (input == "no") //< Then exit the play-again loop.
            {
                play_again = false;
                input_valid = true;
            }
        }
    }*/
}

TEST (CARDS_TESTS, DealerTests) {
    cout << "\n| Testing CardStack class..."
         << "\n| Creating test_deck...";

    Deck stock = Deck ();

    cout << "\n| Creating cards from test_deck...";

    CardStack cards (stock);

    cards.Print ();

    cout << "\n| Testing Dealer::Shuffle()...";

    cards.Shuffle ();

    cards.Print ();
}

TEST (CARDS_TESTS, HandTests) {
    cout << "\n|"
            "\n| Testing BlackjackHand class..."
         << "\n|"
            "\n| Creating deck...";
    Deck pack;

    CardStack stock (pack);

    stock.Shuffle ();

    cout << "\n| Creating player_hand...";

    Hand player_hand (pack.GetNumCards ()),
         dealers_hand (pack.GetNumCards ());

    cout << "\n| Testing Hard::Print ()...";
    player_hand.Print ();
    dealers_hand.Print ();

    cout << "\n| Creating Hands for Player {Ace of Hearts, Ace of Clubs) and"
            "\n| Dealer { King of Hearts, Nine of Diamonds } ...";
    //player_hand.SetCards ();

    cout << "\n|\n| Done testing kabuki::cards::Hand class...";
    system ("PAUSE");
}

TEST (CARDS_TESTS, DeckTests) {
    cout << "\n|\n| Testing kabuki::cards::Deck class..."
         << "\n|\n| Creating test_deck...";

    Deck pack;

    for (int i = 0; i < 7; ++i) {
        PrintLine ("|\n|", '-');
        cout << "\n| Creating " << Suit::kFormatStrings[i] << " Deck...";
        pack.Set (Deck::kNoJokers, Deck::kAcesLow, (Suit::Format) i);
        pack.Print ();
    }
    cout << "\n|\n| Testing kabuki::cards::Deck::Shuffle () class...";

    CardStack stock (pack);

    stock.Shuffle ();
    pack.Print ();

    cout << "\n|\n| Done testing kabuki::cards::Deck class...";
}

//TEST (CARDS_TESTS, CardComboTests) {
//    cout << "\n|\n| Testing kabuki::cards::CardCombo class...";
//    cout << "\n|\n| Done testing kabuki::cards::CardCombo class...";
//}

TEST (CARDS_TESTS, CardTests) {
    cout << "\n|\n| Testing kabuki::cards::Card class... "
         << "\n|\n| Creating test Suit::Print ()...";

    Suit heart    (Suit::kHeart  , 0),
         diamond  (Suit::kDiamond, 1),
         club     (Suit::kClub   , 2),
         spade    (Suit::kSpade  , 3);
    cout << "\n| ";
    heart.Print   ();
    cout << ", ";
    diamond.Print ();
    cout << ", ";
    club.Print    ();
    cout << ", ";
    spade.Print   ();

    Card cards[5];

    cout << "\n|\n| Changing suits... ";

    cards[0].Set (Card::kKing, &heart);
    cards[1].Set (Card::kKing, &heart);
    cards[2].Set (Card::kJack, &diamond);
    cards[3].Set (Card::kJack, &club);
    cards[4].Set (Card::kKing, &club);

    cout << "\n|\n| Testing Card::Print():void... ";

    for (int i = 0; i < 5; ++i) {
        cout << "\n| ";
        cards[i].Print ();
    }

    cout << "\n|\n| Testing Print():void";

    for (int i = 0; i < 5; ++i) {
        cout << "\n| ";
        cards[i].Print ();
    }

    cout << "\n|\n| Testing Compare(const Card&):int... ";
    int result = cards[0].Compare (&cards[1]);
    cout << "\n| cards[0].Compare (&cards[1]) = " << result;
    CHECK (result == 0)
    result = cards[2].Compare (&cards[3]);
    cout << "\n| cards[2].Compare (&cards[3]) = " << result;
    CHECK (result > 0)
    result = cards[3].Compare (&cards[4]);
    cout << "\n| cards[1].Compare (&cards[3]) = " << result;
    CHECK (result > 0)
    result = cards[4].Compare (&cards[3]);
    CHECK (result < 0)

    cout << "\n|\n| Testing kabuki::cards::Card class... ";
}
