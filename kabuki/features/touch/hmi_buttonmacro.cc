/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_macro_button.cc
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "button_macro.h"

namespace _ {

ButtonMacro::ButtonMacro(const string& label)
    : Parameter<int>(Parameter<int>::ButtonMacro, label, 0, 0, 0, 0, 0),
      num_controls_(0),
      controls(nullptr) {
  // Nothing to do here! :-)
}

ButtonMacro::~ButtonMacro() { ClearControls(); }

int ButtonMacro::GetNumControls() const { return num_controls_; }

void ButtonMacro::ClearControls() { ClearList(controls_); }

int ButtonMacro::Add(Parameter<int>* control) {
  if (control == nullptr) return -1;

  controls = new LinearNode<Parameter<int>*>(control, controls);
  ++num_controls_;

  return Success;
}

int ButtonMacro::add(vector<Parameter<int>*>& events) {
  int numFails = 0;

  for (int i = 0; i < events.size(); ++i) {
    if (add(events[i])) {
      ++numFails;
    }
  }
  return numFails;
}

int ButtonMacro::Set(int index, Parameter<int>* control) {
  if (index < 0) return -1;
  if (index >= num_controls_) return 1;

  if (num_controls_ == 0) {
    controls = new LinearNode<Parameter<int>*>(control);
    return Success;
  }
  auto currentNode = controls;
  while (index-- != 0) currentNode = currentNode->next;
  currentNode->data = control;
  return Success;
}

Parameter<int>* ButtonMacro::remove() {
  if (controls == nullptr) return nullptr;
  --num_controls_;
  auto temp = controls->next;
  delete controls;
  controls = temp;
  return temp->data;
}

int ButtonMacro::maxWordValue() const { return Parameter<int>::maxWordValue(); }

Parameter<int>* ButtonMacro::remove(int index) {
  if (index < 0 || index >= num_controls_ || num_controls_ == 0) return nullptr;

  if (num_controls_ == 1) {
    delete controls;
    num_controls_ = 1;
    return nullptr;
  }

  auto currentNode = controls;
  while (index-- != 1) currentNode = currentNode->next;

  return removeAfter(currentNode);
}

Parameter<int>* ButtonMacro::remove(Parameter<int>* oldControl) {
  auto c = controls;

  while (c != nullptr) {
    if (c->next->data == oldControl) return removeAfter(c);

    c = c->next;
  }
  return nullptr;
}

Parameter<int>* ButtonMacro::GetControl(int index) {
  if (num_controls_ == 0) return nullptr;

  if (index < 0 || index >= num_controls_) return nullptr;

  auto currentNode = controls;
  while (index != 0) {
    currentNode = currentNode->next;
    --index;
  }
  return currentNode->data;
}

Parameter<int>* ButtonMacro::GetFirstControl() {
  if (controls == nullptr) return nullptr;

  return controls->data;
}

void ButtonMacro::toggle() { Button::toggle(); }

string ButtonMacro::label() const { return HmiComponent::print(); }

int ButtonMacro::GetLabel(string label) {
  return HmiComponent::GetLabel(label);
}

string ButtonMacro::listString() const {
  string returnString = CharLine('#', Parameter<int>::MacroHeaderLength);

  auto currentNode = controls;
  while (currentNode != nullptr) {
    returnString += currentNode->data->toStringRow() + '\n';
    currentNode = currentNode->next;
  }
  return returnString + CharLine('#', Parameter<int>::MacroHeaderLength);
}

void ButtonMacro::trigger() {
  auto currentNode = controls;
  while (currentNode != nullptr) {
    currentNode->data->trigger();
    currentNode = currentNode->next;
  }
}

void ButtonMacro::printHeaderString() const {
  Printf(
      "|    Parameter<int>    |Action|Step | CC  | Ch  |Value| min | max |Init "
      "|# Bits|\n");
}

void ButtonMacro::printRow() const {
  Parameter<int>::printRow();
  Printf("   | %10s | %5 |\n%s\n", actionString(), stepSize(), listString());
}

void ButtonMacro::print() const {
  printLine('~', Parameter<int>::MacroHeaderLength);
  Printf("Macro Control:\n ");
  printLine('~', Parameter<int>::MacroHeaderLength);
  Printf(Parameter<int>::MacroHeader);
  printNL();
  printLine('~', Parameter<int>::MacroHeaderLength) + toStringRow() +
      listString();
}

void ButtonMacro::clearList(LinearNode<Parameter<int>*>* thisNode) {
  if (thisNode == nullptr) return;
  clearList(thisNode->next);
  delete thisNode;
}

Parameter<int>* ButtonMacro::removeAfter(
    LinearNode<Parameter<int>*>* thisNode) {
  if (thisNode == nullptr) return nullptr;

  auto nodeToDelete = thisNode->next;

  if (nodeToDelete == nullptr) return nullptr;

  // else we are safe to delete the node.
  Parameter<int>* removedNodesControl = nodeToDelete->data;

  thisNode->next = nodeToDelete->next;

  delete nodeToDelete;

  return removedNodesControl;
}

}  // namespace _
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
