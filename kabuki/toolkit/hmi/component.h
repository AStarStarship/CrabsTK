/* Kabuki Toolkit
@file    ~/source/hmi/hmi_component.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#ifndef HEADER_FOR_KT_HMI_HMICOMPONENT
#define HEADER_FOR_KT_HMI_HMICOMPONENT

#include "config.h"

namespace _ {

typedef enum {
  HMITypeMIDI = 0,
  HMITypeDMX,

} HMIComponentType;

//< The non-alpha-numeric characters allowed in a label.
const char* GetAllowedSymbols();

/* Sets the allowed symbols to the given string. */
void SetAllowedSymbols(const char* s);

/* Checks the given char c to see if any of the chars contain it. */
inline bool ContainsChar(const char* chars, char c);

/* Checks the given string s to see if any of the chars are in it. */
inline bool ContainsChar(const char* chars, const char* s);

/* Verifies if thisLabel is a valid label.
    @return Returns 0 if the label is valid.
    @return Returns 1 if the label is too uint32_t.
    @return Returns -1 thisLabel is "".
*/
int IsValidLabel(const char* label);

/* All unnamed labels share the same char* pointer. */
const char* GetUnnamedLabel();

/* A list of the most common Parameter types. */
typedef enum ParameterTypes {
  ControlDmx = 0,
  MidiControl,
  OSCControl,
  DMXButton,
  MIDIButton,
  OSCButton,
  ButtonMacro
} ParameterType;

/* A component in a Human-Machine Interface that may be referenced by name or
   UID. A HmiComponent has the following rules: 1.) Labels must be 255
   characters or less for a normal label, and 16 characters or less for a short
   label. 2.) Labels must contain printable ASCII characters that are HUI
   compatible.
*/
class KABUKI HmiComponent {
 public:
  enum {
    MaxLabelLength = 20,  //< The max label length of a label.
  };

  /* Default constructor. */
  HmiComponent(const char* newLabel = "");

  /* Copy constructor. */
  HmiComponent(const HmiComponent& other);

  /* Virtual destructor. */
  virtual ~HmiComponent() {}

  /* Gets the unique ID for the object. */
  uid_t GetUid();

  /* Gets the unique string of the component type. */
  const char* GetHMIComponentType();

  /* Gets the label. */
  const char* GetLabel() const;

  /* Sets the label to the given string.
      @return Returns 0 upon success and 1 if the newLabel is too uint32_t.
      @see    HmiComponent::isValid (char). */
  virtual int SetLabel(const char* s);

  /* Compares this label to the given string.
      @return Returns 0 if they are identical and1 if the other char* is too
     uint32_t. */
  int Compare(const char* s) const;

  /* Prints this object to a string. */
  virtual _::UtfN& Print(_::Utf& print) const;

 private:
  uid_t uid_,          //< Unique id.
      type_;           //< Type of HMI component.
  const char* label_;  //< This object's text label.
};                     //< class HmiComponent
}   //< namespace _
#endif  //< HEADER_FOR_KT_HMI_HMICOMPONENT
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
