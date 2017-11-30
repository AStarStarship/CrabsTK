/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/macro_button.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#include "MacroButton.h>

namespace kabuki { namespace hmi {

MacroButton::MacroButton (const string& label)
    : Parameter<int> (Parameter<int>::MacroButton, label, 0, 0, 0, 0, 0),
    num_controls_ (0),
    controls (nullptr) {
    // Nothing to do here! :-)
}


MacroButton::~MacroButton () {
    ClearControls ();
}

int MacroButton::GetNumControls () const {
    return num_controls_;
}

void MacroButton::ClearControls () {
    ClearList (controls_);
}

int MacroButton::Add (Parameter<int>* control) {
    if (control == nullptr)
        return -1;

    controls = new LinearNode<Parameter<int>*> (control, controls);
    ++num_controls_;

    return Success;
}

int MacroButton::add (vector<Parameter<int>*>& events) {
    int numFails = 0;

    for (int i = 0; i < events.size (); ++i) {
        if (add (events[i])) {
            ++numFails;
        }
    }
    return numFails;
}

int MacroButton::Set (int index, Parameter<int>* control) {
    if (index < 0)
        return -1;
    if (index >= num_controls_)
        return 1;

    if (num_controls_ == 0) {
        controls = new LinearNode<Parameter<int>*> (control);
        return Success;
    }
    auto currentNode = controls;
    while (index-- != 0)
        currentNode = currentNode->next;
    currentNode->data = control;
    return Success;
}

Parameter<int>* MacroButton::remove () {
    if (controls == nullptr)
        return nullptr;
    --num_controls_;
    auto temp = controls->next;
    delete controls;
    controls = temp;
    return temp->data;
}

int MacroButton::maxWordValue () const {
    return Parameter<int>::maxWordValue ();
}

Parameter<int>* MacroButton::remove (int index) {
    if (index < 0 || index >= num_controls_ || num_controls_ == 0)
        return nullptr;

    if (num_controls_ == 1) {
        delete controls;
        num_controls_ = 1;
        return nullptr;
    }

    auto currentNode = controls;
    while (index-- != 1)
        currentNode = currentNode->next;

    return removeAfter (currentNode);
}

Parameter<int>* MacroButton::remove (Parameter<int>* oldControl) {
    auto c = controls;

    while (c != nullptr) {
        if (c->next->data == oldControl)
            return removeAfter (c);

        c = c->next;
    }
    return nullptr;
}

Parameter<int>* MacroButton::GetControl (int index) {
    if (num_controls_ == 0)
        return nullptr;

    if (index < 0 || index >= num_controls_)
        return nullptr;

    auto currentNode = controls;
    while (index != 0) {
        currentNode = currentNode->next;
        --index;
    }
    return currentNode->data;
}

Parameter<int>* MacroButton::GetFirstControl () {
    if (controls == nullptr)
        return nullptr;

    return controls->data;
}

void MacroButton::toggle () {
    Button::toggle ();
}

string MacroButton::label () const {
    return HmiComponent::print ();
}

int MacroButton::GetLabel (string label) {
    return HmiComponent::GetLabel (label);
}

string MacroButton::listString () const {
    string returnString = CharLine ('#', Parameter<int>::MacroHeaderLength);

    auto currentNode = controls;
    while (currentNode != nullptr) {
        returnString += currentNode->data->toStringRow () + '\n';
        currentNode = currentNode->next;
    }
    return returnString + CharLine ('#', Parameter<int>::MacroHeaderLength);
}

void MacroButton::trigger () {
    auto currentNode = controls;
    while (currentNode != nullptr) {
        currentNode->data->trigger ();
        currentNode = currentNode->next;
    }
}

void MacroButton::printHeaderString () const {
    printf ("|    Parameter<int>    |Action|Step | CC  | Ch  |Value| min | max |Init |# Bits|\n");
}

void MacroButton::printRow () const {
    Parameter<int>::printRow ();
    printf ("   | %10s | %5 |\n%s\n", actionString (), stepSize (), listString ());
}

void MacroButton::print () const {
    printLine ('~', Parameter<int>::MacroHeaderLength);
    printf ("Macro Control:\n ");
    printLine ('~', Parameter<int>::MacroHeaderLength);
    printf (Parameter<int>::MacroHeader);
    printNL ();
    printLine ('~', Parameter<int>::MacroHeaderLength) + toStringRow () + listString ();
}



void MacroButton::clearList (LinearNode<Parameter<int>*>* thisNode) {
    if (thisNode == nullptr)
        return;
    clearList (thisNode->next);
    delete thisNode;
}

Parameter<int>* MacroButton::removeAfter (LinearNode<Parameter<int>*>* thisNode) {
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

}   //< namespace hmi
}   //< namespace kabuki
