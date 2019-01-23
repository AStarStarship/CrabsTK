/* kabuki::pro
    @file    /library/kt/id/impl/grammar.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
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

#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "grammar.h"

namespace _ {

const char* GetDefaultPassword() { "password"; }

const char* GetDefaultPasswordFormat() { return "^[a-zA-Z0-9]*$"; }

Word::Word() {
  num_illegal_char_ranges_ = 0;
  num_mandatory_char_ranges_ = 0;

  illegal_chars_ = nullptr;
  mandatory_chars_ = nullptr;
}

Word::Word(const Word& initState) {
  num_illegal_char_ranges_ = initState.num_illegal_char_ranges_;
  num_mandatory_char_ranges_ = initState.num_mandatory_char_ranges_;

  /*
  int i;
  if (num_illegal_char_ranges_)
  {
  illegal_chars_ = new char[num_illegal_char_ranges_][2];
  for (i = 0; i < num_illegal_char_ranges_; ++i)
  {
  illegal_chars_[i][1] = initState.illegal_chars_[i][1];
  illegal_chars_[i][2] = initState.illegal_chars_[i][2];
  }
  }
  else
  illegal_chars_ = nullptr;

  if (num_mandatory_char_ranges_)
  {
  illegaChars = new char[num_illegal_char_ranges_][2];
  for (i = 0; i < num_illegal_char_ranges_; ++i)
  {
  illegal_chars_[i][1] = initState.illegal_chars_[i][1];
  illegal_chars_[i][2] = initState.illegal_chars_[i][2];
  }
  }
  else
  mandatory_chars_ = nullptr;
  */
}

void Word::SetFormat(const char* format) {
  if (format == nullptr) return;
  if (format_ == nullptr) return;
  delete format_;
  format_ = StringClone(format);
}

bool Word::IsValid(const char* string) {
  /*
  int stringLength = strlen (thisstring);

  if (stringLength  < min_length_ || stringLength > max_length_)
  return false;

  int i, j; // Looping variables
  for (i = 0; i < ; ++i)
  {
  for (j = 0; j < num_illegal_char_ranges_; ++j)
  if (thisstring[i])
  }
  */
  return true;
}

int Word::GetMinLength() { return min_length_; }

bool Word::setMinLength(int value) {
  if (value < 0 || value >= max_length_) return false;
  min_length_ = value;
  return true;
}

int Word::GetMaxLength() { return max_length_; }

bool Word::setMinLength(int value) {
  if (value < min_length_) return false;
  max_length_ = value;
  return true;
}

bool Word::IsIllegal(char aChar) { return true; }

bool Word::MakeIllegal(char thisChar, char descstring) {
  // if (!isprint (thisChar))
  //   return false;
  return true;
}

bool Word::MakeIllegal(int start_index, int stop_index, char description) {
  return true;
}

bool Word::MakeMandatory(int start_index, int stop_index, int num_instances,
                         char descstring) {
  return true;
}

void Word::Print(Log& log) { log << "Word: "; }

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
