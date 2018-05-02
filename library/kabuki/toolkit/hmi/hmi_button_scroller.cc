/** Kabuki Toolkit
    @file    ~/source/hmi/button_scroller.cc
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

#include "button_scroller.h"

namespace kt { namespace hmi {

ButtonScroller::ButtonScroller (string initLabel, int newNumVisibleButtons) 
:   ButtonScroller (initLabel),
    currentIndex (0),
    buttons ()
{
    setNumVisibleButtons (newNumVisibleButtons);
}

ButtonScroller::ButtonScroller (const ButtonScroller& o)
:   HmiComponent (o),
    numVisibleButtons (o.numVisibleButtons),
    currentIndex (o.currentIndex),
    buttons (o.buttons)
{
    // Nothing to do here yet.
}

Button** ButtonScroller::GetButtons ()
{
    return &buttons[0];
}

int ButtonScroller::GetNumButtons () const
{
    return buttons.size ();
}

void ButtonScroller::SetNumVisibleButtons (int value)
{
    numVisibleButtons = (value < 1) ? 1 : value;
}

void ButtonScroller::RemoveButton (int index)
{
    int l_numButtons = getNumButtons ();
    if (index < 0 || index >= l_numButtons)
        return;
    if (buttons.remove (index))
    {
        ;
    }

    if (index  > currentIndex)
        --currentIndex;

    if (currentIndex + numVisibleButtons >= l_numButtons)
    {
        currentIndex = l_numButtons - numVisibleButtons;
    }
}

int ButtonScroller::GetNumVisibleButtons () const
{
    return numVisibleButtons;
}

int ButtonScroller::GetCurrentIndex () const
{
    return currentIndex;
}

int ButtonScroller::press (int index)
{
    if (index < 0)
        return -1;

    if (index >= numVisibleButtons)
        return 1;

    int buttonIndex = currentIndex + index;

    if (buttonIndex > getNumButtons ())
        return 2;

    auto temp = buttons[buttonIndex];
    temp->press ();
    return 0;
}

void ButtonScroller::scrollUp ()
{
    // Note: Up equals getPrevious.

    if (currentIndex == 0) // Special Case: We can't scroll any farther.
        return;

    --currentIndex;
}

void ButtonScroller::scrollDown ()
{
    if (currentIndex >= getNumButtons () + numVisibleButtons) // We can't scroll any farther!
        return;

    ++currentIndex;
}

void ButtonScroller::scrollPageUp ()
{
    scrollUp (numVisibleButtons);
}

void ButtonScroller::scrollPageDown ()
{
    scrollDown (numVisibleButtons);
}

void ButtonScroller::scrollUp (int numTimes)
{
    if (numTimes < 1)
        return;

    int thisNumButtons = getNumButtons ();

    if (currentIndex + numTimes > thisNumButtons)              //< Were at the end and can't scroll any farther!
        currentIndex = thisNumButtons - numVisibleButtons;    //< Show the last numVisibleButtons Buttons.
        
    currentIndex -= numTimes;
}


void ButtonScroller::scrollDown (int numTimes)
{
    int l_numButtons = getNumButtons ();
    if (currentIndex + numTimes >= l_numButtons) // Special Case: We can't scroll any farther.
        numTimes = l_numButtons - currentIndex - numVisibleButtons;

    currentIndex += numTimes;
}

void ButtonScroller::print () const
{
    int i;

    printf ("Controls:::ButtonScroller %s \n_numVisibleButtons: %i, size (): \nVisible Buttons: ", 
        getLabel (), numVisibleButtons, getNumButtons ());

    for (i = 0; i < numVisibleButtons; ++i)
    {
        if (buttons[i] == nullptr)
        {
            printf ("\n");
            break;
        }

        printf (buttons[i]->getLabel ());

        if (i != numVisibleButtons - 1)
            printf (" -> ");
        else
            printf ("\n");
    }

    if (i==0)
        printf ("Empty\n");
}

}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
