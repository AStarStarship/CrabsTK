/** Dojo Card Games
    @file       ~/Console/main.cpp
    @author     Cale McCollough
    @copyright  © CoderDojo 2015 ®
    @license    This program is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        This program is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../Library/global.h"

#include <iostream>
#include <string>
#include <sstream>

#include "../Library/DojoCardGames/Deck.h"

#include "../Library/DojoCardGames/Blackjack/BlackjackDealer.h"
#include "../Library/DojoCardGames/Blackjack/BlackjackPlayer.h"

using namespace std;

using namespace DojoCardGames;
using namespace DojoCardGames::Blackjack;

void runUnitTests ()
{
    Array<UnitTest> unitTests ();
    unitTests += deckUnitTest;
    unitTests += cardStackUnitTest,
}


void playBlackjack ()
{
    auto pack = Deck ();

    auto player = BlackjackPlayer ("Player", 100);
    auto dealer = BlackjackDealer ();

    // 
    bool agian = true;
    string input = "";
    

    do {
        dealer.startNewGame ();
        
        do {
            cout << dealer.toString ();                 //< Print the dealer to the console.
            
            cout << tildiLnBreak;                       //< This prints a "~~~~~~~~~~~~~~~~~~~~~~~~~~~~" line to the console

            cout << dealer.getPlayer (1).toString ();   //< Print player 1 to the console (we only have 2 players right now.

            cout << tildiLnBreak;

            bool inputValid = false;

            do {
                cout << "Hit or hold?";
                getline (cin, input);

                if (input == "hit")
                {
                    inputValid = true;

                    if (stock.isEmpty ())
                        stock.shuffle ();

                    player.getHand ().addToHand (stock.nextCard ());
                    inputValid = true;
                }
                else if (input == "hold")
                {
                    agian = false;
                }
                else if (input == "quit") //< First we need to check for the exit condition
                {
                    inputValid = true;
                    agian = false;
                }
                else
                {
                    cout << "Error: invalid input. Please type 'hit', 'hold', 'quit'";
                }
            }

        } while (agian);

        dealer.playHand ();

        int handComparison = player.getHand ().compare (dealer);

        // calculates totals
        if ()
        {
            cout << "      Players wins!!!";
        }
        else
        {
            if (dealer.beats (player))
            {
                cout << "      Dealer wins:(";
            }
        }
        cout << "Your Score: " + player.handTotal ()[0] + " with " + player.handTotal ()[1] + " aces ** Computers Score: " + dealer.handTotal ()[0] + " with " + dealer.handTotal ()[1] + " ace(s).";
        cout << "Do you want to play agian?";
        getline(cin, input);
        if (input == "y" || input == "Y")
            agian = true;
    } while (agian);
}


int main (int argc, char* argv[])
{
    runUnitTests ();
    // playBlackjack ();
}