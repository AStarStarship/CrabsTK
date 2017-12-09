    /** kabuki::script
#include <script_slot.h>
    @version 0.x
    @file    ~/source/kabuki/script/impl/script_slot_out.cc
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

#include <stdafx.h>
#include "slot.h"

namespace _ {

const Operation* SlotResult (Slot* slot, Bin::Error error) {
    return BinResult (reinterpret_cast<Bin*> (slot), error);
}

const Operation* SlotResult (Slot* slot, Bout::Error error) {
    return BoutResult (reinterpret_cast<Bout*> (slot), error);
}


const Operation* Slotwrite (Slot* slot, const uint_t* params, void** args) {
    if (slot == nullptr)     //< Is this a double check?
        return SlotResult (slot, Bout::kErrorRoom);
    if (params == nullptr)
        return SlotResult (slot, Bout::kErrorRoom);
    if (args == nullptr)
        return SlotResult (slot, Bout::kErrorRoom);
    return SlotResult (slot, Bout::kErrorRoom);
}
//bool IsWritable (MirrorOut* mirror) {
//   return mirror->start != mirror->stop;
//}

}       //< namespace _
