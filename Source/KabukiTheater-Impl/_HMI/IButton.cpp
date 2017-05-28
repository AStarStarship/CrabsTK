/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_HMI/IButton.cpp
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

#include <_HMI/IButton.hpp>
#include <_System/Time.hpp>

namespace _HMI {

static int minDoublePressTime = 100,    //< The min int click time period in seconds.
    maxDoublePressTime = 2000,          //< The max int click time period in seconds.
    defaultDoublePressTime = 0;         //< The default int press time period in seconds.

const char* getButtonActionString (ButtonAction type)
{
    static const char* buttonActionStrings[] = { "Momentary", "Latching", "Presure Sensitive" },
        * invalidState = "Invalid state";
    if (type < 0) return invalidState;
    if (type > 3) return invalidState;
    return buttonActionStrings[type];
}

int getMinDoublePressTime ()        { return minDoublePressTime; }
int getMaxDoublePressTime ()        { return maxDoublePressTime; }
int getDefaultDoublePressTime ()    { return defaultDoublePressTime; }

IButton::IButton (ButtonAction action, int stepSize, int doublePressTime)
:   action (action),
    pressed (false),
    stepSize (stepSize),
    doublePressTime (doublePressTime),
    lastTimePressed (0)
{
    setButtonAction (action);
}

IButton::IButton (const IButton &o)
:   action (o.action),
    stepSize (o.stepSize),
    pressed (o.pressed),
    doublePressTime (o.doublePressTime),
    lastTimePressed (o.lastTimePressed)
{
    // Nothing to do here :-)
}

void IButton::press ()
{
    pressed = true;

    int currentTime = _System::getTimestamp ();
    if (currentTime - lastTimePressed <= doublePressTime)
    {
        doublePress ();
        return;
    }
    press ();
}

void IButton::depress ()
{
    pressed = false;
    depress ();

}

bool IButton::isPressed () const
{
    return pressed;
}

void IButton::setState (bool state)
{
    pressed = state;
}

int IButton::getLastTimePressed () const
{
    return lastTimePressed;
}

int IButton::getStepSize () const
{
    return stepSize;
}

void IButton::setStepSize (int value)
{
    if (value < 0)
        stepSize = 0;
    else if (value >= getMaxWordValue ())
        stepSize = getMaxWordValue ();
    else
        stepSize = value;
}

ButtonAction IButton::getButtonAction () const
{
    return action;
}

void IButton::setButtonAction (ButtonAction newAction)
{
    action = newAction;
}

const char* IButton::getActionString () const
{
    return getButtonActionString (action);
}

void IButton::toggle ()
{
    if (pressed)
        pressed = false;
    else
        pressed = true;
}

void IButton::print () const
{

}

}   //< _HMI
