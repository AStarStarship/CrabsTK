/** Kabuki Starship
    @file    ~/Source-Impl/_hmi/Components/XYControl.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

namespace _hmi { namespace Components {

#include <_hmi/Components/XYControl.h>

XYControl::XYControl (int initX, int initY, int initLeftBounds, int initRightBounds, int initTopBounds, int initBottomBounds)
  : x (initX),
    y (initY),
    leftBounds (initLeftBounds),
    rightBounds (initRightBounds), 
    topBounds (initTopBounds), 
    bottomBounds (initBottomBounds)
{
    // Nothing to do here yet.
}

XYControl::XYControl (const XYControl& that)
  : x (that.x),
    y (that.y),
    leftBounds (that.leftBounds),
    rightBounds (that.rightBounds), 
    topBounds (that.topBounds), 
    bottomBounds (that.bottomBounds)
{
    // Nothing to do here yet.
}


int XYControl::getX ()
{
    return x;
}
int XYControl::getY ()
{
    return y;
}

void XYControl::setX (int value)
{
    if (value <= leftBounds)
        x = leftBounds;
    else if (value >= rightBounds)
        x = rightBounds;
    else
        x = value;
}
void XYControl::setY (int value)
{
    if (value <= bottomBounds)
        y = bottomBounds;
    else if (value >= topBounds)
        y = topBounds;
    else
        y = value;
}


int XYControl::getLeftBounds ()
{
    return leftBounds;
}

int XYControl::getRightBounds ()
{
    return rightBounds;
}

int XYControl::topBounds ()
{
    return topBounds;
}

int XYControl::getBottomBound ()
{
    return bottomBounds;
}
    
bool XYControl::setLeftBounds (int newBounds)
{
    if (newBounds >= rightBounds)
        return false;

    leftBounds = newBounds;

    return true;
}

bool XYControl::setRightBounds (int newBounds)
{
    if (newBounds <= rightBounds)
        return false;

    rightBounds = newBounds;

    return true;
}

bool XYControl::setTopBounds (int newBounds)
{
    if (newBounds <= bottomBounds)
        return false;

    topBounds = newBounds;

    return true;
}

bool XYControl::setBottomBound (int newBounds)
{
    if (newBounds >= topBounds)
        return false;

    bottomBounds = newBounds;

    return true;
}


void XYControl::print ()
{
    string temp = "XYControl:\nx: " + x + "  y: " + y + "\n";
    slot.prints (temp);
}

}   //  Components
}   //  _hmi
