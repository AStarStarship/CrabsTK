/** Kabuki Theater
    @file    /.../Source-Impl/_HMI/MacroButton.cpp
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

#include <_HMI/MacroButton.hpp>

namespace _HMI {

MacroButton::MacroButton (const string& newLabel)
:   Parameter<int> (Parameter<int>::MacroButton, newLabel, 0, 0, 0, 0, 0),
    numControls (0),
    controls (nullptr)
{
    // Nothing to do here! :-)
}


MacroButton::~MacroButton ()
{
    clearControls ();
}

int MacroButton::getNumControls () const
{
    return numControls;
}

void MacroButton::clearControls ()
{
    clearList (controls);
}

int MacroButton::add (Parameter<int>* newControl)
{
    if (newControl == nullptr)
        return -1;

    controls = new LinearNode<Parameter<int>*> (newControl, controls);
    ++numControls;

    return Success;
}

int MacroButton::add (vector<Parameter<int>*>& events)
{
    int numFails = 0;

    for (int i = 0; i < events.size (); ++i)
    {
        if (add (events[i]))
        {
            ++numFails;
        }
    }
    return numFails;
}

int MacroButton::set (int index, Parameter<int>* newControl)
{
    if (index < 0)
        return -1;
    if (index >= numControls)
        return 1;

    if (numControls == 0)
    {
        controls = new LinearNode<Parameter<int>*> (newControl);
        return Success;
    }
    auto currentNode = controls;
    while (index-- != 0)
        currentNode = currentNode->next;
    currentNode->data = newControl;
    return Success;
}

Parameter<int>* MacroButton::remove ()
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
    return Parameter<int>::maxWordValue ();
}

Parameter<int>* MacroButton::remove (int index)
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
    
    return removeAfter (currentNode);
}

Parameter<int>* MacroButton::remove (Parameter<int>* oldControl)
{
    auto c = controls;

    while (c != nullptr)
    {
        if (c->next->data == oldControl)
            return removeAfter (c);
        
        c = c->next;
    }
    return nullptr;
}

Parameter<int>* MacroButton::getControl (int index)
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

Parameter<int>* MacroButton::getFirstControl ()
{
    if (controls == nullptr)
        return nullptr;

    return controls->data;
}

void MacroButton::toggle ()
{
    IButton::toggle ();
}

string MacroButton::label () const
{
    return HMIComponent::print ();
}

int MacroButton::getLabel (string newLabel)
{
    return HMIComponent::getLabel (newLabel);
}

string MacroButton::listString () const
{
    string returnString = CharLine ('#', Parameter<int>::MacroHeaderLength);

    auto currentNode = controls;
    while (currentNode != nullptr)
    {
        returnString += currentNode->data->toStringRow () + '\n';
        currentNode = currentNode->next;
    }
    return returnString + CharLine ('#', Parameter<int>::MacroHeaderLength);
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

void MacroButton::printHeaderString () const
{
    printf ("|    Parameter<int>    |Action|Step | CC  | Ch  |Value| min | max |Init |# Bits|\n");
}

void MacroButton::printRow () const
{
    Parameter<int>::printRow ();
    printf ("   | %10s | %5 |\n%s\n", actionString (), stepSize (), listString ());
}

void MacroButton::print () const
{
    printLine ('~', Parameter<int>::MacroHeaderLength);
    printf ("Macro Control:\n ");
    printLine ('~', Parameter<int>::MacroHeaderLength);
    printf (Parameter<int>::MacroHeader);
    printNL ();
    printLine ('~', Parameter<int>::MacroHeaderLength) + toStringRow () + listString ();
}



void MacroButton::clearList (LinearNode<Parameter<int>*>* thisNode)
{
    if (thisNode == nullptr)
        return;
    clearList (thisNode->next);
    delete thisNode;
}

Parameter<int>* MacroButton::removeAfter (LinearNode<Parameter<int>*>* thisNode)
{
    if (thisNode == nullptr)
        return nullptr;

    auto nodeToDelete = thisNode->next;

    if (nodeToDelete == nullptr)
        return nullptr;

    // else we are safe to delete the node.
    Parameter<int>* removedNodesControl = nodeToDelete->data;

    thisNode->next = nodeToDelete->next;

    delete nodeToDelete;

    return removedNodesControl;
}

}   //< _HMI
