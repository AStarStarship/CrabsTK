/** Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/crabs/language.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_CRABS_LANGUAGE
#define HEADER_FOR_CRABS_LANGUAGE

#include "config.h"

#if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5

namespace _ {

namespace langauge {

enum {
    kEnUppercaseBegin = 'A',
    kEnUppercaseEnd   = 'Z',
    kEnLowercaseBegin = 'a',
    kEnLowercaseEnd   = 'z',
};
}       //< namespace language

/** A spoken language Unicode character range.

*/
struct Language {
    wchar_t uppercase_begin, //< First uppercase Unicode char index.
            uppercase_end,   //< Last uppercase Unicode char index.
            lowercase_begin, //< First lowercase Unicode char index.
            lowercase_end;   //< Last lowercase Unicode char index.
};

}       //< namespace _
#endif  //< #if MAJOR_SEAM > 1 || MAJOR_SEAM == 1 && MINOR_SEAM >= 5
#endif  //< HEADER_FOR_CRABS_LANGUAGE