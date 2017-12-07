/** kabuki::id
    @file    ~/source/kabuki/id/validator.h
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

#ifndef HEADER_FOR_KABUKI_ID_VALIDATOR
#define HEADER_FOR_KABUKI_ID_VALIDATOR

#include "config.h"

namespace kabuki { namespace id {

/** Interface for a class that can validate a string for correctness.
    This interface is useful for making rules for things like Handle(s) and
    Password(s). Classes that implement this interface must define the indexes
    of the types.
*/
struct Validator {
    /** Function validates the input for correctness.
        @param  input The input to validate.
        @param  type  The type of input (i.e. Handle or Password).
        @return Returns null upon success and a pointer to . an error string
                if the input is invalid. */
    virtual const char* IsValid (const char* input, int type) = 0;
};

}       //< namespace id
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_ID_VALIDATOR
