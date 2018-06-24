/* Kabuki Toolkit
@file    ~/source/hmi/hmi_component.cc
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

#include <stdafx.h>
#if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
#if SEAM_MAJOR == 2 && SEAM_MINOR == 2
#define PRINTF(format, ...) printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "component.h"

namespace _ {

const char* GetAllowedSymbols() { return " `!@#$%^&* (){}+=-_\\|/?.>,<"; }

const char* allowed_symbols_ = GetAllowedSymbols();

void SetAllowedSymbols(const char* s) {
  if (s == nullptr) return;
  allowed_symbols_ = s;
}

bool ContainsChar(const char* chars, const char* s) {
  if (chars == nullptr) return false;
  if (s == nullptr) return false;
  const char* s_ptr;
  char c;

  do {
    s_ptr = s;
    c = *chars;
    if (ContainsChar(chars, c)) return true;
    ++chars;
  } while (c != 0);
  return false;
}

bool ContainsChar(const char* chars, char c) {
  if (chars == nullptr) return false;
  const char* s_ptr;
  char d;
  do {
    d = *chars;
    if (c == d) return true;
    ++chars;
  } while (d != 0);
  return false;
}

const char* GetUnnamedLabel() { return "Unnamed"; }

int IsValidLabel(const char* label) {
  int labelLength = StringLength(label);
  printf("Checking string: %s length = %u", label, labelLength);

  if (labelLength == 0)
    return 1;
  else if (labelLength > HmiComponent::MaxLabelLength)
    return 2;

  for (int i = 0; i < labelLength; ++i) {
    char temp = label[i];
    if (!isalnum(temp)) return 3;
    if (!ContainsChar(GetAllowedSymbols(), temp)) return 3;
  }
  return 0;
}

HmiComponent::HmiComponent(const char* label) {
  if (SetLabel(label)) label = GetUnnamedLabel();
}

HmiComponent::HmiComponent(const HmiComponent& s) : Label(s.label_) {
  // Nothing to do here :-)
}

uid_t HmiComponent::GetUid() { return uid_; }

/* Returns the label string. */
const char* HmiComponent::GetLabel() const { return label_; }

int HmiComponent::SetLabel(const char* label) {
  if (label == "") return -1;
  size_t length = strlen(label);
  if (length > MaxLabelLength) return 1;

  if (!IsValidLabel(label)) return 1;

  label = label;
  return 0;
}

int HmiComponent::SetLabel(const HmiComponent& label) {
  return SetLabel(label.label_);
}

int HmiComponent::Compare(const char* s) const {
  cout << "# " << label_ << ".compare (" << s << ") = " << label_.Compare(s);

  return label_.Compare(s);
}

int HmiComponent::Compare(const HmiComponent& s) const {
  return Compare(s.label_);
}

void HmiComponent::Print() const { label_->Print(); }

}       //< namespace _
#endif  //< #if SEAM_MAJOR > 2 || SEAM_MAJOR == 2 && SEAM_MINOR >= 0
