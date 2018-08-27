/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/door.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>
#if SEAM >= SEAM_0_0_4
#ifndef INCLUDED_CRABS_DOOR
#define INCLUDED_CRABS_DOOR
// Dependencies:
#include "expr.h"
#include "slot.h"
#include "tstk.h"
#include "tstr.h"
// End dependencies.

namespace _ {

/* An door that connects two Chinese rooms.
    @see https://en.wikipedia.org/wiki/Chinese_room

    A door can lead to one or more rooms. Each room has a Room. A door can
    be locked, in which case all of the Rooms behind this door are locked
    out. When a door is locked, no messages are processed or sent to or
    from the door and its subsidiary agents.

    Rooms can be connected by one or more Portal (string). An example of a
   system with one Door and multiple Portals is a WiFi plus USB/Serial
   connection. This is a commonly used configuration for debugging the system.

    Multiple doors also might share the same Portal. An example of this is a
    connection to multiple systems over a WiFi connection.

    @code

    +=============+
    |    slots    |
    |=============|
    | Door struct |
    +=============+

    @endcode
*/
class Door : public Operand {
 public:
  typedef enum Errors { kErrorInvalidOp = 0, kErrorImplementation } Error;

  enum {
    kMinDoorSize = 128,  //< The min and default size of the door buffer.
  };

  /* A door in a Chinese room. */
  Door(const char* roomName = nullptr, uintptr_t* buffer = nullptr,
       uintptr_t size_bytes = kMinDoorSize);

  /* Destructor. */
  virtual ~Door();

  /* Gets the BOut at the given index. */
  slot_t GetSlot(slot_t index);

  /* Address the given expr to the Door. */
  slot_t AddSlot(slot_t slot);

  /* Attempts to find a Slot or Door with the given address. */
  bool Contains(void* address);

  /* Gets the Slot that contains the given address.
      @return Returns the doors_ stack count if the Door does not
              contain the given address. */
  slot_t FindSlot(void* address);

  /* Executes all of the queued escape sequences. */
  const Op* ExecAll();

  /* Script expressions. */
  virtual const Op* Star(wchar_t index, Expr* expr);

 private:
  uintptr_t* begin;                       //< Pointer to dynamic buffer.
  CArray<slot_t, uint_t, int_t>* slots_;  //< Slots in the door.
};

/* Returns a Static Error Op. */
KABUKI const Op* DoorResult(Door* door, Door::Error error);

KABUKI const char* DoorErrorText(Door::Error error);

/* Returns the main door of the room.
KABUKI Door& Doors (); */

/* Initializes a Door at the beginning of the given buffer.
static Door* DoorInit (int* buffer, uint_t slot_size) {
    if (buffer == nullptr) return nullptr;
    if (slot_size < kMinSlotSize) return nullptr;
    Wall* wall = reinterpret_cast<Door*>(buffer);
    w->is_dynamic = 0;
    w->num_doors = 0;
    w->max_num_doors;
    w->door_one = nullptr;
}*/

}  // namespace _
#endif  //< #if SEAM >= SEAM_0_0_4
#endif  //< INCLUDED_CRABS_DOOR
