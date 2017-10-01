/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/slot.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                    All right reserved (R).

             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#include "../include/slot.h"
#include "../include/terminal.h"

namespace _ {

KABUKI Slot* SlotInit (uint_t width, uint_t size, byte max_num_slots) {
    Slot* s = New<Slot, uint_t> (width, kMinSocketSize);
    if (s == nullptr) return nullptr;
    s->is_dynamic = 0;
    s->num_slots = 0;
    s->max_num_slots = max_num_slots;
    s->slot_one = nullptr;
}

KABUKI Slot* SlotInit (byte* buffer, uint_t slot_size, byte max_num_slots) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSocketSize) return nullptr;
    Slot* s = reinterpret_cast<Slot*>(buffer);
    s->is_dynamic = 0;
    s->num_slots = 0;
    s->max_num_slots = max_num_slots;
    s->slot_one = nullptr;
}

KABUKI Terminal** SlotTerminals (Slot* s) {
    return reinterpret_cast<Terminal**> (s->slot_one);
}

KABUKI ticket_t SlotAddTerminal (Slot* s, Terminal* t) {
    if (s == nullptr) return Report (NullPointerError);
    if (t == nullptr) return Report (NullPointerError);
    byte num_slots = s->num_slots;
    if (num_slots + 1 >= s->max_num_slots) 
        return Report (NullPointerError);   //< @todo Add new error.
    SlotTerminals (s)[num_slots] = t;
    ++s->num_slots;
    return 0;
}

KABUKI Terminal* SlotGetTerminal (Slot* s, byte index) {
    if (s == nullptr) return nullptr;
    if (index >= s->num_slots)
        return nullptr;
    return SlotTerminals (s)[index];
}

KABUKI Terminal* SlotFindTerminal (Slot* s, void* address) {
    Terminal** terminals = SlotTerminals (s);
    if (terminals == nullptr) return nullptr;
    for (byte i = 0; i < s->num_slots; ++i) {
        Terminal* t = terminals[i];
        if (t->Contains (address)) return t;
    }
    return nullptr;
}

KABUKI void SlotDelete (Slot* s, byte index) {
    if (s == nullptr) return;
    if (index >= s->num_slots)
        return;
    Terminal** ts = SlotTerminals (s);
    Terminal* t = ts[index];
    t->~Terminal ();
    for (byte i = index; i < s->num_slots; ++i)
        ts[index] = ts[index + 1];
    --s->num_slots;
}

KABUKI void SlotPrint (Slot* s) {
    if (s == nullptr) return;
    printf ("\nSlot:\nis_dynamic %s\nnum_slots: %u\nmax_num_slots: %u\n", 
            s->is_dynamic ? "true" : "false", s->num_slots, s->max_num_slots);
}

}       //< namespace _
