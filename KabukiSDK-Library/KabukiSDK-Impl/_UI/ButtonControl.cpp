/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/AVControl.cpp
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

#include <_UI/ButtonControl.hpp>

namespace _UI {

const char* ButtonControl::ActionStrings[] = { "Momentary", "Latching", "Presure Sensitive" };

const double ButtonControl::MinDoubleClickTime = 0.01;
const double ButtonControl::MaxDoubleClickTime = 0.02;

const double ButtonControl::DefaultDoublePressTime = 
    (ButtonControl::MinDoubleClickTime + ButtonControl::MaxDoubleClickTime) /2.0;

ButtonControl::ButtonControl (int newButtonControlAction, int newStepSize, double newDoublePressTime) :
    isPressed (false),
    buttonAction (newButtonControlAction),
    stepSize (newStepSize),
    doublePressTime (newDoublePressTime),
    lastTimePressed (0.0)
{
    setButtonAction (newButtonControlAction);
}

ButtonControl::ButtonControl (const ButtonControl &other) :
    buttonAction (other.buttonAction),
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

const char*& ButtonControl::ActionString () const
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

byte ButtonControl::getState ()
{
    return 0;
}

const char* ButtonControl::getState (byte Value)
{
    return 0;
}

const char* ButtonControl::op (I2P::Terminal& slot, int index, int Enq)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("ButtonControl", "_UI"): InvalidIndex ();
}

}   //< namespace _UI
