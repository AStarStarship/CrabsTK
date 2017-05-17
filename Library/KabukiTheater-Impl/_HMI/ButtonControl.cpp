/** Kabuki Theater
    @file    /.../KabukiTheater-Impl/_HMI/AVControl.cpp
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

#include <_HMI/ButtonControl.hpp>

namespace _HMI {

static float minDoubleClickTime = 0.0f,                 //< The min double click time period in seconds.
    maxDoubleClickTime = 0.0f,                          //< The max double click time period in seconds.
    DefaultDoublePressTime = 0.0f;                      //< The default double press time period in seconds.

static const float defaultMinDoublePressTime = 0.1f,   //< The min double click time period in seconds.
    MaxDoublePressTime = 0.2f;                  //< The max double click time period in seconds.
    DefaultDoublePressTime;                     //< The default double press time period in seconds.

const char** buttonActionStrings ()
{
    static const char* buttonActionStrings[] = { "Momentary", "Latching", "Presure Sensitive" };
}

float getMinDoubleClickTime () { return minDoublePressTime; }

float getMaxDoubleClickTime () { return maxDoublePressTime; }

const double ButtonControl::DefaultDoublePressTime = 
   (ButtonControl::MinDoubleClickTime + ButtonControl::MaxDoubleClickTime) /2.0;

ButtonControl::ButtonControl (int newButtonControlAction, int newStepSize, double newDoublePressTime)
:   isPressed (false),
    buttonAction (newButtonControlAction),
    stepSize (newStepSize),
    doublePressTime (newDoublePressTime),
    lastTimePressed (0.0)
{
    setButtonAction (newButtonControlAction);
}

ButtonControl::ButtonControl (const ButtonControl &other)
:   buttonAction (other.buttonAction),
    stepSize (other.stepSize),
    isPressed (other.isPressed),
    doublePressTime (other.doublePressTime),
    lastTimePressed (other.lastTimePressed)
{
    // Nothing to do here :-)
}

void ButtonControl::press ()
{
    isPressed = true;

    double currentTime = Time::getMillisecondCounterHiRes ();
    if (currentTime - lastTimePressed <= doublePressTime)
    {
        DoublePress ();
        return;
    }
    press ();
}

void ButtonControl::depress ()
{
    isPressed = false;
    depress ();

}

bool ButtonControl::isPressed () const
{
    return isPressed;
}

void ButtonControl::getState (bool isPressedState)
{
    isPressed = isPressedState;
}

double ButtonControl::getLastTimePressed () const
{
    return lastTimePressed;
}

int ButtonControl::getStepSize () const
{
    return stepSize;
}

void ButtonControl::getStepSize (int value)
{
    if (value < 0)
        stepSize = 0;
    else if (value >= MaxWordValue ())
        stepSize = MaxWordValue ();
    else
        stepSize = value;
}

int ButtonControl::getButtonAction () const
{
    return buttonAction;
}

void ButtonControl::getgetButtonAction (int newAction)
{
    buttonAction = newAction;
}

string ButtonControl::ActionString () const
{
    return ActionStrings[buttonAction];
}

void ButtonControl::toggle ()
{
    if (isPressed)
        isPressed = false;
    else
        isPressed = true;
}

}   //  _HMI
