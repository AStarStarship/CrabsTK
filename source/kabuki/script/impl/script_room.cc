/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/script_room.h
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

#include <stdafx.h>
#include "../include/room.h"
#include "../include/door.h"
#include "../include/interrupts.h"
#include "../include/args.h"
#include "../include/operation.h"

namespace _ {

const char** RoomStateStrings () {
    static const char* states[] = {
        "Initializing",
        "Waking up",
        "Running",
        "Going to sleep",
        "Exiting"
    };
    return states;
}

const char** RequestStrings () {
    static const char* RequestStrings[] = {
        "Open door",
        "Close door",
        "Invalid request"
    };

    return RequestStrings;
}

const char* RequestString (Request r) {
    if (r < 0 || r >= InvalidRequest) return RequestStrings ()[InvalidRequest];
    return RequestStrings ()[r];
}

Room::Room (uint_t size) :
    door_         (nullptr),
    xoff_         (nullptr),
    device_       (nullptr),
    devices_      (nullptr)
{

}

Room::~Room () {
}

Request Room::HandleNextRequest (Request r) {
    return InvalidRequest;
}

void Room::ClearLog () {

}

void Room::ProcessLog () {

}

void Room::PrintErrors (Bout* bout) {
    //uint_t errorHeader[] = { 0 };
    //return a.prints (errorHeader);
}

void Room::DiagnoseProblems () {
    /// Check for remote crash request.
    throw RoomCrashException ();
}

const Operation* Room::Init () {
    return 0;
}

void Room::ShutDown () {}

void Room::Sleep () {}

void Room::Wake () {}

void Room::Crash () {}

const Operation* Room::Loop () { return 0; }

bool Room::IsOn () {
    return true;
}

int Room::Main (const char** args, int args_count) {
    const Operation* result;

    std::cout << "Initializing Chinese Room with " << args_count << " args:\n";
    for (int i = 0; i < args_count; ++i) {
        std::cout << i << ": " << args[i] << '\n';
    }
    std::cout << '\n';

    while (IsOn ()) {
        try {
            Init ();
            do {
                door_->ExecAll ();
                result = Loop ();
            } while (!result);
            ShutDown ();
        } catch (RoomCrashException e) {
            std::cout << "\n| Room crashed!\n";
            return 3;
        }
    }
    return 1;
}

const Operation* Room::Star (char_t index, Expression* expr) {
    static const Operation star = { "Room",
        NumOperations (0), FirstOperation (' '),
        "A Chinese Room.", 0
    };

    switch (index) {
        case '?': {
            return &star;
        }

    }
    return 0;
}

Room* ChineseRoom (Room* room) {
    static Room* chinese_room = nullptr;
    if (room != nullptr)
        chinese_room = room;
    return room;
}

}       //< namespace _
