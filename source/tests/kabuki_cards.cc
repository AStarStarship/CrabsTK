/** Kabuki Toolkit
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

#include "../../../../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../../../../cpputest/include/CppUTest/TestHarness.h"

#include "../../source/kabuki/cards/global.h"

using namespace std;
using namespace _;
using namespace kabuki::id;
using namespace kabuki::cards;

TEST_GROUP (CARDS_TESTS) {
    void setup () {
        cout << Text ();
    }

    void teardown () {
        Print ("\n|\n| Test completed.");
        Print (Text().Line ());
    }
};
/*
TEST (CARDS_TESTS, CardTests) {
    cout << "\n|\n| Testing kabuki::cards::Card class... "
        << "\n|\n| Creating test Suit::Print (_::Text& txt)...";

    Suit heart (Suit::kHeart, 0),
        diamond (Suit::kDiamond, 1),
        club (Suit::kClub, 2),
        spade (Suit::kSpade, 3);
    cout << "\n| ";
    heart.Print (txt);
    cout << ", ";
    diamond.Print (txt);
    cout << ", ";
    club.Print (txt);
    cout << ", ";
    spade.Print (txt);

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
        cards[i].Print (txt);
    }

    cout << "\n|\n| Testing Print():void";

    for (int i = 0; i < 5; ++i) {
        cards[i].Print (txt);
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

    Deck pack;

    for (int i = 0; i < 7; ++i) {
        PrintLine ("|\n|", '-');
        cout << "\n| Creating " << Suit::kFormatTexts[i] << " Deck...";
        pack.Set (Deck::kNoJokers, Deck::kAcesLow, (Suit::Format) i);
        pack.Print (txt);
    }
    cout << "\n|\n| Testing kabuki::cards::Deck::Shuffle ()...\n|";

    CardStack stock (pack);

    stock.Print (txt);

    cout << "\n|\n| Done testing kabuki::cards::Card class... ";
}*/

TEST (CARDS_TESTS, ServerTests) {
    // # How to use this tester:
    //
    // 1. Setup the game state using function calls.
    // 2. Run in a constant loop inputing in keyboard 
    //    commands to change the state of the server.
    // 3. Have fun.

    //char input[Handle::kDefaultMaxLength + 1],
    //    password[Password::kDefaultMaxLength + 1];
    char input[141];
    //int32_t session;
    //uid_t public_key;

    I () << Text ().Line ()
         << "\n| Testing kabuki.cards.Server..."
         << Text ().Line ();

    //StrandWrite (handle, handle + Handle::kDefaultMaxLength + 1, "Me");
    //StrandWrite (password, password + Handle::kDefaultMaxLength + 1, "Too");

    Server server;

    I () << server.Star ('?', nullptr)->description
         << "\n|\n|";

    server.Users ().Add ("Me", "Too");
    Print (server.Users ().Print ());
    server.Users ().Add ("Marcus");
    server.Users ().Add ("Duy");
    server.Users ().Add ("Jo");

    Print ("\n|\n|");

    server.AddBlackjackGame ();
    server.Users ().Add ("Player4");
    server.AddBlackjackGame ();
    I () << server.Print ();
    //KeyboardText ("\n< Enter your handle: ", handle,
    //              Handle::kDefaultMaxLength + 1);
    //KeyboardText ("\n< Enter your password: ", password,
    //              Password::kDefaultMaxLength + 1);
    //txt << "\n| Registering @" << handle << " with password:\n\""
    //    << password << '\"';
    //server.Users () = server.Users ().LogIn (handle, password);
    I () << "\n|\n|";
    server.SetState (Server::kStateServingClients);
    Print (server.Print ());
    // Play-again loop.
    // A play-again loop goes in a loop where the user is asked if they want to
    // play again. The user can
    // enter "hit", "hold", "exit", or "quit". The code for the game.
    while (server.GetState ()) {
        // Server is asynchronous so all we do is loop and wait for the state
        // to get set to 0.
        KeyboardText (server.GetDirections (), input, input + 141);
        I () << server.Sudo (input, input + 141);
    }

    I () << "\n|\n| Done testing kabuki::cards::Server class... ";
    system ("PAUSE");
}

//#define USING_KABUKI_CARDS_PROJECT 1
#if USING_KABUKI_CARDS_PROJECT
#include "../pro/global.h"

using namespace kabuki::_;
using namespace kabuki::pro;

class KabukiCards : public Project {
    public:

    ChineseRoom () :
        Project ("kabuki::cards") {
        StartProject ("2017-11-30@12:30PM");
        Timestamp ("2017-11-30@12:30PM");
        Log      ("This project actually got started in 2015 but I'm just now writing the first log. Scope of project has expanded into TCP-based client-server app with IRC-style chat using Script.");
    }
};
#endif  //< USING_KABUKI_CARDS_PROJECT
