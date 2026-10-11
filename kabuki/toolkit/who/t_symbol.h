/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_symbol.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_WHO_1
#ifndef KABUKI_TOOLKIT_WHO_GRAMMAR
#define KABUKI_TOOLKIT_WHO_GRAMMAR


namespace _ {

/* Used for settings rules to text grammar.
Useful for rules for passwords.
*/
class TSymbol {
 public:
  enum {
    ToShortFlag = 1,            //<
    ToLongFlag,                 //<
    ContainsInvalidCharsFlag,   //<
    MissingMandatoryCharsFlag,  //<
    MustStartWithCharFlag       //<
  };

  /* Creates and anarchy grammar with no rules. */
  TSymbol ()
  : num_illegal_char_ranges_ (0),
  num_mandatory_char_ranges_ (0),
  illegal_chars_ (nullptr),
  mandatory_chars_ (nullptr) {
  }

  /* Copy Constructor. */
  TSymbol (const TSymbol& initState)
    : num_illegal_char_ranges_ (initState.num_illegal_char_ranges_),
      num_mandatory_char_ranges_ = initState.num_mandatory_char_ranges_) {
    /*
    SI4 i;
    if (num_illegal_char_ranges_) {
      illegal_chars_ = new CH1[num_illegal_char_ranges_][2];
      for (i = 0; i < num_illegal_char_ranges_; ++i) {
        illegal_chars_[i][1] = initState.illegal_chars_[i][1];
        illegal_chars_[i][2] = initState.illegal_chars_[i][2];
      }
    }
    else {
      illegal_chars_ = nullptr;
    }

    if (num_mandatory_char_ranges_) {
      illegaChars = new CH1[num_illegal_char_ranges_][2];
      for (i = 0; i < num_illegal_char_ranges_; ++i {
        illegal_chars_[i][1] = initState.illegal_chars_[i][1];
        illegal_chars_[i][2] = initState.illegal_chars_[i][2];
      }
    } else {
      mandatory_chars_ = nullptr;
    }
    */
  }

  const TStrand<>& GetDefaultPassword () { "password"; }

  const TStrand<>& GetDefaultPasswordFormat () { return "^[a-zA-Z0-9]*$"; }

  /* Clones the given object. */
  void Clone(const Word* grammar);

  /* Verifies the given string to see if it has proper grammar. */
  BOL IsValid(const TStrand<>& string) {
    /*
    SI4 stringLength = StrandLength (thisstring);

    if (stringLength  < min_length_ || stringLength > max_length_)
    return false;

    SI4 i, j; // Looping variables
    for (i = 0; i < ; ++i)
    {
    for (j = 0; j < num_illegal_char_ranges_; ++j)
    if (thisstring[i])
    }
    */
    return true;
  }

  /* Gets the min length of a valid symbol. */
  SI4 GetMinLength() { return min_length_; }

  /* Sets the min length of a valid symbol. */
  BOL SetMinLength(SI4 value) {
    if (value < min_length_) return false;
    max_length_ = value;
    return true;
  }

  /* Gets the max length. */
  SI4 GetMaxLength() { return max_length_; }

  /* Gets the max length. */
  BOL SetMaxLength (SI4 value) {

  }

  void SetFormat (const TStrand<>& format) {
    /*if (!format == nullptr) return;
    if (!format_ == nullptr) return;
    delete format_;
    format_ = StrandClone (format);*/
  }

  /* Checks to see if thisChar is an illegal CH1. */
  BOL IsIllegal(CH1 aChar) { return true; }

  /* Adds CH1 index of illegal chars.
  @pre thisChar  must be > 0.
  @param thisChar The illegal CH1 to add.
  */
  BOL MakeIllegal(CH1 thisChar, const TStrand<>& descstring) {
    // if (!isprint (thisChar))
    //   return false;
    return true;
  }

  /* Adds CH1 or range of indexes of illegal chars.
  @pre startIndex and stopIndex must be a printable CH1.
  @param startIndex The starting ASCII CH1 index.
  @param stopIndex  The stopping ASCII CH1 index. */
  BOL MakeIllegal(SI4 startIndex, SI4 stopIndex, 
                   const TStrand<>& descstring) {
    return true;
  }

  /* Adds a CH1 or set of range of CH1 indexes that must be included in a
  valid symbol.
  @pre   start_index and stopIndex must be a printable CH1 and numInstances
  must be > 0 and < max_length_.
  @param start_index The starting ASCII CH1 index.
  @param stop_index  The stopping ASCII CH1 index.
  @param num_instances  The number of times the symbol range must occur. */
  BOL MakeMandatory(SI4 startIndex, SI4 stopIndex, SI4 numInstances,
                     const TStrand<>& descstring) {
    return true;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "\nSymbol rules:" << " MinLength:" << min_length_ << " MaxLength:"
      << max_length_;
  }

 private:
  SI4 num_illegal_char_ranges_,    //< The number of illegal CH1 ranges.
      num_mandatory_char_ranges_,  //< The number of mandatory CH1 ranges.
      min_length_,                 //< The min length of a valid symbol.
      max_length_;                 //< The max length of a valid symbol.
    /*
    Range<char_t> illegal_chars_,     //< A 2 column table of ints that
    represents all of illegal CH1 ranges.
    Range<char_t> mandatory_chars_,   //< A 3 column table of ints that
    represents all of mandatory CH1 ranges and how many are mandatory.
    Range<char_t> mustStartWithChars; //< A 2 column table of ints that
    represents the range (string) of chars that the symbol must start with.
                                 
    const TStrand<>& illegal_chars_desc,     
    //< An array of strings that describes what illegal CH1 (string) are.                               
    mandatory_chars_desc,   //< An array of
    strings that describes what the mandatory CH1 (string) are.
                mustStartWithCharsDesc; //< An array of strings that describes
    what mandatory first CH1 (string) are.
    */
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_GRAMMAR
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
