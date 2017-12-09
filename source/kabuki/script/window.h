/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/window.h
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
    Window (Bin* bin = nullptr, Bout* bout = nullptr);

    /** Script Operations. */
    virtual const Operation* Star (uint index, Expression* expr);
    
    private:

    uint  size_bytes_;  //< Size of the Window in bytes.
    Bin * bin_;         //< Pointer to the B-Input.
    Bout* bout_;        //< Pointer to the B-Output.
};

}       //< namespace _
#endif  //< SCRIPT_WINDOW_H
