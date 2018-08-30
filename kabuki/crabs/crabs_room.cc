/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit
@file    ~/kabuki/crabs/crabs_room.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>
#if SEAM >= SEAM_0_0_4
// Dependencies:
#include "bsq.h"
#include "debug.h"
#include "door.h"
#include "room.h"
#include "stack.h"
#include "text.h"
// End dependencies.
#if SEAM_MAJOR == 0 && SEAM_MINOR == 4
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PUTS(string) putsr(string);
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PUTS(string)
#endif

namespace _ {

const char** RoomStateTexts() {
  static const char* states[] = {"Initializing", "Waking up", "Running",
                                 "Going to sleep", "Exiting"};
  return states;
}

const char** RequestTexts() {
  static const char* RequestTexts[] = {"Open door", "Close door",
                                       "Invalid request"};

  return RequestTexts;
}

const char* RequestText(Request r) {
  // if (r < 0 || r >= InvalidRequest)
  if (r >= InvalidRequest) return RequestTexts()[InvalidRequest];
  return RequestTexts()[r];
}

Room::Room(const char* room_name, int state_count)
    : state_(1),
      state_count_(state_count < 1 ? 1 : state_count),
      name_(!room_name ? "Unnamed" : room_name),
      this_(nullptr),
      xoff_(nullptr),
      device_(nullptr),
      devices_(nullptr) {}

Room::~Room() {}

int Room::GetState() { return state_; }

int Room::GetStateCount() { return state_count_; }

bool Room::SetState(int new_state) {
  if (new_state < 0) {
    return false;
  }
  if (new_state >= state_count_) {
    return false;
  }
  state_ = new_state;
  return true;
}

const char* Room::GetRoomName() { return name_; }

bool Room::SetRoomName(const char* name) {
  if (!name) {
    return false;
  }
  delete name_;
  name_ = TextClone(name);
  return true;
}

Request Room::HandleNextRequest(Request r) { return InvalidRequest; }

void Room::ClearLog() {}

void Room::ProcessLog() {}

void Room::PrintErrors(BOut* bout) {
  // uint_t errorHeader[] = { 0 };
  // return a.prints (errorHeader);
}

void Room::DiagnoseProblems() {
  // Check for remote crash request.
  // throw RoomCrashException ();
}

const Op* Room::Init(Expr* expr) {
  if (expr != nullptr) {
    // @todo We need to load a stored Room state.
    return nullptr;
  }
  return nullptr;
}

void Room::ShutDown() {
#if CRABS_DEBUG
    PRINTF ("\nShutting down...";
#endif  //< CRABS_DEBUG
}

void Room::Sleep() {
#if CRABS_DEBUG
    PRINTF ("\nGoing to sleep...";
#endif  //< CRABS_DEBUG
}

void Room::Wake() {
#if CRABS_DEBUG
    PRINTF ("\nWaking up...";
#endif  //< CRABS_DEBUG
}

void Room::Crash() {
#if CRABS_DEBUG
    PRINTF ("\nRoom crash!";
#endif  //< CRABS_DEBUG
}

const Op* Room::Loop() { return 0; }

bool Room::IsOn() { return true; }

int Room::Main(const char** args, int args_count) {
  const Op* result = nullptr;
  PRINTF("\nInitializing Chinese Room with %i args:", args_count)
  for (int i = 0; i < args_count; ++i) {
    PRINTF("\n%i:\"%s\"", i, args[i])
  }
  PRINT('\n')
  while (IsOn()) {
    try {
      result = Init(nullptr);
      if (result) return 1;
      do {
        this_->ExecAll();
        result = Loop();
      } while (!result);
      ShutDown();
    } catch (...) {
      PRINTF("\nRoom crashed!")
      return 3;
    }
  }
  return 1;
}

char Room::CommandNext() { return 0; }

const Op* Room::Star(wchar_t index, Expr* expr) {
  static const Op kThis = {
      "Room", OpFirst('A'), OpLast('A'), "A Chinese Room.", ';', '}', 0};

  switch (index) {
    case '?': {
      return ExprQuery(expr, kThis);
    }
  }
  return 0;
}

int_t Room::WallCount() { return walls_->count; }

Wall* Room::GetWall(int_t wall_number) {
  if (wall_number < 0) return nullptr;
  if (wall_number >= walls_->count) return nullptr;
  return StackGet<Wall*, uint_t, int_t>(walls_, wall_number);
}

Wall* Room::AddWall(Wall* new_wall) {
  if (new_wall == nullptr) return nullptr;
  if (walls_->count >= walls_->count_max) return nullptr;
  StackPush<Wall*>(walls_, new_wall);
  return new_wall;
}

bool Room::RemoveWall(int_t wall_number) {
  return StackRemove<Wall*, uint_t, int_t>(walls_, wall_number);
}

uintptr_t Room::GetSizeBytes() {
  uintptr_t count = kRoomFloorSize;
  for (int_t i = 0; i < walls_->count; ++i) {
    count += StackGet<Wall*, uint_t, int_t>(walls_, i)->GetSizeBytes();
  }
  // @todo Add all memory we used in bytes here.
  return count;
}

#if CRABS_TEXT
Utf8& Room::Print(Utf8& print) { return print << "\nRoom: "; }
#endif

}  // namespace _
#undef PRINTF
#undef PRINT
#endif  //> #if SEAM >= SEAM_0_0_4
