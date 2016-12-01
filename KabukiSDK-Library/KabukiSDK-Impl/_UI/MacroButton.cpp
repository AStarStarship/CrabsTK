/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Dev/MacroButton.cpp
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

#include <_UI/MacroButton.hpp>

namespace _UI {

MacroButton::MacroButton (const char* newLabel) 
:   AVControl (AVControl::MacroButton, newLabel, 0, 0, 0, 0, 0),
    numControls (0),
    controls (nullptr)
{
    // Nothing to do here! :-)
}


MacroButton::~MacroButton ()
{
    clearControls ();
}

int MacroButton::numControls () const
{
    return numControls;
}

void MacroButton::clearControls ()
{
    clearList (controls);
}

int MacroButton::add (AVControl* newControl)
{
    if (newControl == nullptr)
        return -1;

    controls = new LinearNode<AVControl*> (newControl, controls);
    ++numControls;

    return Success;
}

int MacroButton::add (Array<AVControl*> events)
{
    int numFails = 0;

    for (int i = 0; i < events.size (); ++i)
    {
        if (add (events[i]) != Success)
            ++numFails;
    }
    return numFails;
}

int MacroButton::set (int index, AVControl* newControl)
{
    if (index < 0)
        return -1;
    if (index >= numControls)
        return 1;

    if (numControls == 0)
    {
        controls = new LinearNode<AVControl*> (newControl);
        return Success;
    }
    auto currentNode = controls;
    while (index-- != 0)
        currentNode = currentNode->next;
    currentNode->data = newControl;
    return Success;
}

AVControl* MacroButton::remove ()
{
    if (controls == nullptr)
        return nullptr;
    --numControls;
    auto temp = controls->next;
    delete controls;
    controls = temp;
    return temp->data;
}

int MacroButton::maxWordValue () const
{
    return AVControl::maxWordValue ();
}

AVControl* MacroButton::remove (int index)
{
    if (index < 0 || index >= numControls || numControls == 0)
        return nullptr;

    if (numControls == 1)
    {
        delete controls;
        numControls = 1;
        return nullptr;
    }

    auto currentNode = controls;
    while (index-- != 1)
        currentNode = currentNode->next;
    
    return RemoveAfter (currentNode);
}

AVControl* MacroButton::remove (AVControl* oldControl)
{
    auto currentControl = controls;

    while (currentControl != nullptr)
    {
        if (currentControl->next->data == oldControl)
            return RemoveAfter (currentControl);
        
        currentControl = currentControl->next;
    }
    return nullptr;
}

AVControl* MacroButton::getControl (int index)
{
    if (numControls == 0)
        return nullptr;

    if (index < 0 || index >= numControls)
        return nullptr;

    auto currentNode = controls;
    while (index != 0)
    {
        currentNode = currentNode->next;
        --index;
    }
    return currentNode->data;
}

AVControl* MacroButton::firstControl ()
{
    if (controls == nullptr)
        return nullptr;

    return controls->data;
}

void MacroButton::trigger ()
{
    auto currentNode = controls;
    while (currentNode != nullptr)
    {
        currentNode->data->trigger ();
        currentNode = currentNode->next;
    }
}

void MacroButton::toggle ()
{
    ButtonControl::toggle ();
}

const char* MacroButton::label () const
{
    return Label::print (I2P::Terminal& slot);
}

int MacroButton::getLabel (const char* newLabel)
{
    return Label::getLabel (newLabel);
}

const char* MacroButton::listString () const
{
    const char* returnString = CharLine ('#', AVControl::MacroHeaderLength);

    auto currentNode = controls;
    while (currentNode != nullptr)
    {
        returnString += currentNode->data->toStringRow () + '\n';
        currentNode = currentNode->next;
    }
    return returnString + CharLine ('#', AVControl::MacroHeaderLength);
}

const char* MacroButton::getState (byte Value)
{
    return 0;
}

const char* MacroButton::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("MacroButton", "_UI"): InvalidIndex ();
}

const char* MacroButton::headerString () const
{
    return "|    AVControl    |Action|Step | CC  | Ch  |Value| min | max |Init |# Bits|\n";
}

const char* MacroButton::toStringRow () const
{
    return AVControl::toStringRow () + "   |" + PrintCentered (actionString (), 10) + "|" +
        PrintCentered (const char* (stepSize ()), 5) + "|\n" + listString ();
}

const char* MacroButton::print (I2P::Terminal& slot) const
{
    return CharLine ('~', AVControl::MacroHeaderLength) + "Macro Control:\n " + 
        CharLine ('~', AVControl::MacroHeaderLength) + AVControl::MacroHeader + "\n" + 
        CharLine ('~', AVControl::MacroHeaderLength) + toStringRow () + listString ();
}



void MacroButton::clearList (LinearNode<AVControl*>* thisNode)
{
    if (thisNode == nullptr)
        return;
    clearList (thisNode->next);
    delete thisNode;
}

AVControl* MacroButton::RemoveAfter (LinearNode<AVControl*>* thisNode)
{
    if (thisNode == nullptr)
        return nullptr;

    auto nodeToDelete = thisNode->next;

    if (nodeToDelete == nullptr)
        return nullptr;

    // else we are safe to delete the node.
    AVControl* removedNodesControl = nodeToDelete->data;

    thisNode->next = nodeToDelete->next;

    delete nodeToDelete;

    return removedNodesControl;
}

byte MacroButton::getState ()
{
    return 0;
}

const char* MacroButton::getState (byte Value)
{
    return 0;
}

const char* MacroButton::op (I2P::Terminal* slot, int index)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return enquery ("MacroButton", "_UI"): InvalidIndex ();
}

}   //< namespace _UI
