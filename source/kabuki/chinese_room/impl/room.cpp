/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/Source/ChineseRoom/room.h
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

#include "../include/room.h"
#include "../include/door.h"
#include "../include/exceptions.h"

namespace _ {

KABUKI const char ** RoomStateStrings () {
    static const char * states[] = {
        "Initializing",
        "Waking up",
        "Running",
        "Going to sleep",
        "Exiting"
    };
    return states;
}

KABUKI const char ** RequestStrings () {
    static const char * RequestStrings[] = {
        "Open door",
        "Close door",
        "Invalid request"
    };

    return RequestStrings;
}

KABUKI const char * RequestString (Request r) {
    if (r < 0 || r >= InvalidRequest) return RequestStrings ()[InvalidRequest];
    return RequestStrings ()[r];
}

Room::Room (uint_t size) :
    address_      (0),
    address_msb_  (0),
    size_         (size),
    stack_height_ (0),
    stack_size_   (0),
    door_         (nullptr),
    xoff_         (nullptr),
    device_       (nullptr),
    devices_      (nullptr)
{

}

virtual Room::~Room () {
}

Request Room::HandleNextRequest (Request r) {
    return InvalidRequest;
}

voidRoom::ClearLog () {

}

void Room::ProcessLog () {

}

void Room::PrintErrors (MonoidTx* tx) {
    //uint_t errorHeader[] = { 0 };
    //return io.prints (errorHeader);
}

virtual void Room::DiagnoseProblems () {
    /// Check for remote crash request.
    throw RoomCrashException ();
}

virtual ticket_t Room::Init () {
    return 0;
}

virtual void Room::ShutDown () {}

virtual void Room::Sleep () {}

virtual void Room::Wake () {}

virtual void Room::Crash () {}

virtual ticket_t Room::Loop () { return 0; }

virtual bool Room::IsOn () {
    return true;
}

virtual int Room::Main (const char ** args, int args_count) {
    ticket_t result;

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

virtual const Set* Room::Star (char_t index, Automata* io) {
    static const Set this_member = { "Room", NumMembers (0), 
                                        FirstMember (' '), "A Chinese Room." };
    switch (index) {
        case ' ': {
            static const Set m_space = { "Space", Esc <0> (), Esc <0> (), "" };
        }

    }
    return 0;
}

}       //< namespace _
