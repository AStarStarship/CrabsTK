/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/touch/touch_component.cc
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
#if SEAM >= KABUKI_FEATURES_TOUCH_1
#if SEAM == KABUKI_FEATURES_TOUCH_1
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PAUSE(message)   \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "component.h"

namespace _ {

const CH1* GetAllowedSymbols() { return " `!@#$%^&* (){}+=-_\\|/?.>,<"; }

const CH1* allowed_symbols_ = GetAllowedSymbols();

void SetAllowedSymbols(const CH1* s) {
  if (s == nullptr) return;
  allowed_symbols_ = s;
}

BOL ContainsChar(const CH1* chars, const CH1* s) {
  if (chars == nullptr) return false;
  if (s == nullptr) return false;
  const CH1* s_ptr;
  CH1 c;

  do {
    s_ptr = s;
    c = *chars;
    if (ContainsChar(chars, c)) return true;
    ++chars;
  } while (c != 0);
  return false;
}

BOL ContainsChar(const CH1* chars, CH1 c) {
  if (chars == nullptr) return false;
  const CH1* s_ptr;
  CH1 d;
  do {
    d = *chars;
    if (c == d) return true;
    ++chars;
  } while (d != 0);
  return false;
}

const CH1* GetUnnamedLabel() { return "Unnamed"; }

SI4 IsValidLabel(const CH1* label) {
  SI4 labelLength = StringLength(label);
  Printf("Checking string: %s length = %u", label, labelLength);

  if (labelLength == 0)
    return 1;
  else if (labelLength > HmiComponent::MaxLabelLength)
    return 2;

  for (SI4 i = 0; i < labelLength; ++i) {
    CH1 temp = label[i];
    if (!isalnum(temp)) return 3;
    if (!ContainsChar(GetAllowedSymbols(), temp)) return 3;
  }
  return 0;
}

HmiComponent::HmiComponent(const CH1* label) {
  if (SetLabel(label)) label = GetUnnamedLabel();
}

HmiComponent::HmiComponent(const HmiComponent& s) : Label(s.label_) {
  // Nothing to do here :-)
}

UID HmiComponent::GetUid() { return uid_; }

/* Returns the label string. */
const CH1* HmiComponent::GetLabel() const { return label_; }

SI4 HmiComponent::SetLabel(const CH1* label) {
  if (label == "") return -1;
  size_t length = strlen(label);
  if (length > MaxLabelLength) return 1;

  if (!IsValidLabel(label)) return 1;

  label = label;
  return 0;
}

SI4 HmiComponent::SetLabel(const HmiComponent& label) {
  return SetLabel(label.label_);
}

SI4 HmiComponent::Compare(const CH1* s) const {
  cout << "# " << label_ << ".compare (" << s << ") = " << label_.Compare(s);

  return label_.Compare(s);
}

SI4 HmiComponent::Compare(const HmiComponent& s) const {
  return Compare(s.label_);
}

void HmiComponent::Print() const { label_->Print(); }

}  // namespace _
#endif  //< #if SEAM >= KABUKI_FEATURES_TOUCH_1
