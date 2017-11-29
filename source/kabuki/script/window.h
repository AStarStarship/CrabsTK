/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/window.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef SCRIPT_WINDOW_H
#define SCRIPT_WINDOW_H

#include "operation.h"
#include "bin.h"
#include "bout.h"

namespace _ {

/** A word-aligned B-Stream.
    Windows can be written to like B-Outputs or Scanned as Expressions and
    unpacked while scanning. */
class KABUKI Window : public Operation {

    public:

    /** Constructs a Window. */
    Window (Bin* bin, Bout* bout);

    /** Sets the Portal up for a batch of bytes transfer.
        Implementation of this function is not required to do anything, but
        regardless it will be called by the Set. */
    virtual void Prime ();

    /** Gets the length of current portal.
        @warning Length might not be the actual length, but rather the length
        of the data that is read to be pulled. */
    virtual uint_t Length ();

    /** Feeds tx messages through the a without scanning them. */
    virtual void Feed (byte b);

    /** Pulls rx messages through the a and runs them through the scanner. */
    virtual byte Pull ();

    /** Script Operations. */
    virtual const Operation* Star (char_t index, Expression* expr);
    
    private:

    uint  size_bytes_;  //< Size of the Window in bytes.
    Bin * bin_;         //< Pointer to the B-Input.
    Bout* bout_;        //< Pointer to the B-Output.
};

}       //< namespace _
#endif  //< SCRIPT_WINDOW_H
