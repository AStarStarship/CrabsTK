/** Kabuki Toolkit
    @file    ~/source/hmi/hmi_button.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#if MAJOR_SEAM == 2 && MINOR_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "button.h"
#include "../app/time.h"

namespace kt { namespace hmi {

enum {
    kMinDoublePressTime     = 100,  //< Min int click time period in ms.
    kMaxDoublePressTime     = 2000, //< Max int click time period in ms.
    kDefaultDoublePressTime = 0;    //< Default int press time period in ms.

const char* getButtonActionString (ButtonAction type)
{
    static const char* buttonActionStrings[] = { "Momentary", "Latching", "Pressure Sensitive" },
        * invalidState = "Invalid state";
    if (type < 0) return invalidState;
    if (type > 3) return invalidState;
    return buttonActionStrings[type];
}

int getMinDoublePressTime ()        { return minDoublePressTime; }
int getMaxDoublePressTime ()        { return maxDoublePressTime; }
int getDefaultDoublePressTime ()    { return defaultDoublePressTime; }

Button::Button (ButtonAction action, int stepSize, int doublePressTime)
:   action (action),
    pressed (false),
    stepSize (stepSize),
    doublePressTime (doublePressTime),
    lastTimePressed (0)
{
    setButtonAction (action);
}

Button::Button (const Button &o)
:   action (o.action),
    stepSize (o.stepSize),
    pressed (o.pressed),
    doublePressTime (o.doublePressTime),
    lastTimePressed (o.lastTimePressed)
{
    // Nothing to do here :-)
}

void Button::Press () {
    pressed = true;

    int currentTime = _System::GetTimestamp ();
    if (currentTime - lastTimePressed <= doublePressTime) {
        doublePress ();
        return;
    }
    press ();
}

void Button::Depress ()
{
    pressed = false;
    depress ();

}

bool Button::IsPressed () const
{
    return pressed;
}

void Button::SetState (bool state)
{
    pressed = state;
}

int Button::GetLastTimePressed () const
{
    return lastTimePressed;
}

int Button::GetStepSize () const
{
    return stepSize;
}

void Button::SetStepSize (int value)
{
    if (value < 0)
        stepSize = 0;
    else if (value >= getMaxWordValue ())
        stepSize = getMaxWordValue ();
    else
        stepSize = value;
}

ButtonAction Button::GetButtonAction () const
{
    return action;
}

void Button::SetButtonAction (ButtonAction newAction)
{
    action = newAction;
}

const char* Button::GetActionString () const
{
    return getButtonActionString (action);
}

void Button::Toggle ()
{
    if (pressed)
        pressed = false;
    else
        pressed = true;
}

Printer& Button::Print (Printer& printer) const
{

}

}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
