/** Kabuki Theater
    @file    /.../Source-Impl/_Theater/HMI/ButtonScroller.cpp
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

#include <_Theater/HMI/ButtonScroller.hpp>

namespace _Theater { namespace HMI {

/** Default constructor. */
ButtonScroller::ButtonScroller (string initLabel, int newNumVisibleButtons) : 
    ButtonScroller (initLabel),
    currentIndex (0),
    buttons ()
{
    setNumVisibleButtons (newNumVisibleButtons);
}

/** Copy constructor. */
ButtonScroller::ButtonScroller (const ButtonScroller& other) :
    ButtonScroller (other),
    numVisibleButtons (other.numVisibleButtons),
    currentIndex (other.currentIndex),
    buttons (other.buttons)
{
    // Nothing to do here yet.
}

/** gets the array of buttons. */
Array<ButtonControl*>& ButtonScroller::buttons ()
{
    return buttons;
}

/** gets the number of buttons in the array. */
int ButtonScroller::numButtons () const
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
    if (index < 0 || index >= numButtons ())
        return;
    buttons.remove (index);

    if (index  > currentIndex)
        --currentIndex;

    if (currentIndex + numVisibleButtons >= numButtons ())
    {
        currentIndex = numButtons () - numVisibleButtons;
    }
}

/** gets the number of visible buttons. */
int ButtonScroller::numVisibleButtons () const
{
    return numVisibleButtons;
}

/** gets the index of the first visible button. */
int ButtonScroller::currentIndex () const
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

    if (buttonIndex > numButtons ())
        return 2;

    auto temp = buttons[buttonIndex];
    temp->processPress ();
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
    if (currentIndex >= numButtons () + numVisibleButtons) // We can't scroll any farther!
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

    int thisNumButtons = numButtons ();

    if (currentIndex + numTimes > thisNumButtons)              //< Were at the end and can't scroll any farther!
        currentIndex = thisNumButtons - numVisibleButtons;    //< Show the last numVisibleButtons Buttons.
        
    currentIndex -= numTimes;
}


void ButtonScroller::scrollDown (int numTimes)
{
    if (currentIndex + numTimes >= numButtons ()) // Special Case: We can't scroll any farther.
        numTimes = numButtons () - currentIndex - numVisibleButtons;

    currentIndex += numTimes;
}

/** Returns a string represenation of this void*. */
string ButtonScroller::print (Terminal& slot) const
{
    int i;       // Looping variable.

    string stringRep = "Controls:::ButtonScroller " + label () + 
        "\n_numVisibleButtons: " + string (numVisibleButtons) + "   size (): " + string (numButtons ()) + 
        "\nVisible Buttons: ";

    for (i=0; i < numVisibleButtons; ++i)
    {
        if (buttons[i] == nullptr)
        {
            stringRep += "\n";
            break;
        }

        stringRep += buttons[i]->GetLabel ();

        if (i != numVisibleButtons - 1)
            stringRep += " -> ";
        else
            stringRep += "\n";
    }

    if (i==0)
        stringRep += "Empty\n";

    return stringRep;
}

}   //< HMI
}   //< _Theater
