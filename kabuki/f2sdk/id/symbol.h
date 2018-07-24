/** Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/grammar.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
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
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_GRAMMAR
#define HEADER_FOR_KT_ID_GRAMMAR
#include "config.h"

namespace _ {

/** Used for settings rules to text grammar.
    Useful for rules for passwords.
*/
class Word {
 public:
  enum {
    ToShortFlag = 1,            //<
    ToLongFlag,                 //<
    ContainsInvalidCharsFlag,   //<
    MissingMandatoryCharsFlag,  //<
    MustStartWithCharFlag       //<
  };

  /** Creates and anarchy grammar with no rules. */
  Word();

  /** Copy Constructor. */
  Word(const Word& initState);

  /** Clones the given object. */
  void Clone(const Word* grammar);

  /** Verifies the given string to see if it has proper grammar. */
  bool IsValid(const char* string);

  /** Gets the min length of a valid symbol. */
  int GetMinLength();

  /** Sets the min length of a valid symbol. */
  bool SetMinLength(int value);

  /** Gets the max length. */
  int GetMaxLength();

  /** Gets the max length. */
  bool SetMaxLength(int value);

  /** Checks to see if thisChar is an illegal char. */
  bool IsIllegal(char aChar);

  /** Adds char index of illegal chars.
      @pre thisChar  must be > 0.
      @param thisChar The illegal char to add.
  */
  bool MakeIllegal(char thisChar, const char* descstring);

  /** Adds char or range of indexes of illegal chars.
      @pre startIndex and stopIndex must be a printable char.
      @param startIndex The starting ASCII char index.
      @param stopIndex  The stopping ASCII char index. */
  bool MakeIllegal(int startIndex, int stopIndex, const char* descstring);

  /** Adds a char or set of range of char indexes that must be included in a
     valid symbol.
      @pre   start_index and stopIndex must be a printable char and numInstances
     must be > 0 and < max_length_.
      @param start_index The starting ASCII char index.
      @param stop_index  The stopping ASCII char index.
      @param num_instances  The number of times the symbol range must occur. */
  bool MakeMandatory(int startIndex, int stopIndex, int numInstances,
                     const char* descstring);

  /** Prints this object to a expression. */
  void Print(Log& log);

 private:
  int num_illegal_char_ranges_,    //< The number of illegal char ranges.
      num_mandatory_char_ranges_,  //< The number of mandatory char ranges.
      min_length_,                 //< The min length of a valid symbol.
      max_length_;                 //< The max length of a valid symbol.
                                   /*
                                   Range<char_t> illegal_chars_,     //< A 2 column table of ints that
                                   represents all of illegal char ranges.
                                   Range<char_t> mandatory_chars_,   //< A 3 column table of ints that
                                   represents all of mandatory char ranges and how many are mandatory.
                                   Range<char_t> mustStartWithChars; //< A 2 column table of ints that
                                   represents the range (string) of chars that the symbol must start with.
                                 
                                   const char* illegal_dhars_desc,     //< An array of strings that describes
                                   what illegal char (string) are.                                  mandatory_chars_desc,   //< An array of
                                   strings that describes what the mandatory char (string) are.
                                               mustStartWithCharsDesc; //< An array of strings that describes
                                   what mandatory first char (string) are.
                                   */
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_GRAMMAR
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
