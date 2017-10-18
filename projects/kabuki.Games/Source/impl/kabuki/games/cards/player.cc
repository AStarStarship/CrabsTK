/** Kabuki Card Games
    @file    /.../KabukiTheater-Examples/KabukiCardGames/KabukiCardGames/Player.cpp
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
#include "Player.hpp"

namespace KabukiCardGames {

Player::Player (string thisPlayerName, int startingPoints) :
    name (thisPlayerName),
    numWins (0),
    numPoints (startingPoints < 1 ? 1 : startingPoints),
    hand (Hand ())
{
    // Nothing to do here!
}

string& Player::getName ()
{
    return name;
}

void Player::setName (const string& newName)
{
    name = newName;
}

Hand& Player::getHand ()
{
	return hand;
}

void Player::setHand (Hand& newHand)
{
    hand = newHand;
}

int Player::getNumPoints ()
{
    return numPoints;
}

int Player::addPoints (int num_points)
{
    if (num_points < 0)
        return -1;

    numPoints += num_points;

    return numPoints;
}

int Player::removePoints (int num_points)
{
    if (num_points < 0)
        return -1;

    if (num_points > numPoints)
        return numPoints - num_points;

    numPoints -= num_points;

    return 0;
}

int Player::getNumWins ()
{
    return numWins;
}

void Player::addWin ()
{
    ++numWins;
}

void Player::resetWins ()
{
    numWins = 0;    //< Reset the numWins.
}

void Player::dealHand (Hand& newHand)
{
    hand = newHand; //< And deal a new hand.
}

string Player::tostring ()
{
    return "Player: " + name + "\nnumPoints: " + string (numPoints) + "\nnumWindows: " + string (numWins) + "\n" + dashedLnBreak + hand.tostring ();;
}

}   //< KabukiCardGames
