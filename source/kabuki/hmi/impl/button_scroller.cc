/** Kabuki Starship
    @file    /.../Source/_HMI-Impl/_HMI/ButtonScroller.cpp
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

#include <_HMI/ButtonScroller.h>

namespace _HMI {

/** Default constructor. */
ButtonScroller::ButtonScroller (string initLabel, int newNumVisibleButtons) 
:   ButtonScroller (initLabel),
    currentIndex (0),
    buttons ()
{
    setNumVisibleButtons (newNumVisibleButtons);
}

/** Copy constructor. */
ButtonScroller::ButtonScroller (const ButtonScroller& o)
:   HMIComponent (o),
    numVisibleButtons (o.numVisibleButtons),
    currentIndex (o.currentIndex),
    buttons (o.buttons)
{
    // Nothing to do here yet.
}

/** gets the array of buttons. */
IButton** ButtonScroller::getButtons ()
{
    return &buttons[0];
}

/** gets the number of buttons in the array. */
int ButtonScroller::getNumButtons () const
{
    return buttons.size ();
}

/** sets the number of visible buttons to the new value. */
void ButtonScroller::setNumVisibleButtons (int value)
{
    numVisibleButtons = (value < 1) ? 1 : value;
}

/** Removes the button at the given index from the array. */
void ButtonScroller::removeButton (int index)
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

/** gets the number of visible buttons. */
int ButtonScroller::getNumVisibleButtons () const
{
    return numVisibleButtons;
}

/** gets the index of the first visible button. */
int ButtonScroller::getCurrentIndex () const
{
    return currentIndex;
}

/** Presses the AButton at the specified index. */
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

/** Scrolls up the list. */
void ButtonScroller::scrollUp ()
{
    // Note: Up equals getPrevious.

    if (currentIndex == 0) // Special Case: We can't scroll any farther.
        return;

    --currentIndex;
}

/** Scrolls down the list. */
void ButtonScroller::scrollDown ()
{
    if (currentIndex >= getNumButtons () + numVisibleButtons) // We can't scroll any farther!
        return;

    ++currentIndex;
}

/** Scrolls up by the number of visible pages. */
void ButtonScroller::scrollPageUp ()
{
    scrollUp (numVisibleButtons);
}

/** Scrolls down by the number of visible pages. */
void ButtonScroller::scrollPageDown ()
{
    scrollDown (numVisibleButtons);
}

/** Scrolls up the list. */
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

/** Returns a string represenation of this void*. */
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

}   //< _HMI
