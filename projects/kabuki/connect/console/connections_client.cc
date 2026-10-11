/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/store/store_client.cc
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

#include <pch.h>
#include "client.h"

namespace _ {

Client::Client () {

}

Client::~Client () {
}

const char* Client::Reset () {
}

uint Client::GetState () {
    return state_;
}

bool Client::SetState (int state) {
    if (state < 0) {
        return false;
    }
    state_ = state;
    return true;
}

_::Text& Client::Print (_::Text& txt) {
    return txt;
}

const Operation* Client::Star (uint index, _::Expr* expr) {
    static const Operation This = { "Client",
        OperationCount (0), OperationFirst ('A'),
        "kabuki::cards Script client.", 0
    };

    void* args[4];
    PlayerProxy* player;

    switch (index) {
        case '?': return &This;
        case 'A': {
            static const Operation OpA = { "SetState",
                Params<1, UI1> (), Params<0> (),
                "Sets the client state.", 0
            };
            if (!expr) return &OpA;
            byte state;
            if (!ExpressionArgs (expr, Params<1, UI1> (), Args (args, &state)))
                return expr->result;
            SetState (state);
            return nullptr;
        }
        case 'B': {
            static const Operation OpB = { "Print",
                Params<1, STR, kMaxMessageLength + 1> (), Params<0> (),
                "Sets the client state.", 0
            };
            if (!expr) return &OpB;
            char buffer[kMaxMessageLength + 1];
            if (!ExpressionArgs (expr, Params<1, STR, kMaxMessageLength + 1> (),
                           Args (args, buffer)))
                return expr->result;
            std::cout << buffer;
            return nullptr;
        }
        case 'C': {
            static const uint_t* kRxHeaderC = Params<1, SI4,
                STR, User::kMaxStatusLength + 1,
                STR, Handle::kMaxLength + 1> ();
            static const Operation OpC = { "SetPlayer",
                kRxHeaderC, Params<0> (),
                "Sets the player at the given #index to the given "
                "#dislpay_name.", 0
            };
            if (!expr) return &OpC;
            int32_t player_number;
            char status[User::kMaxStatusLength + 1],
                 handle[Handle::kMaxLength];
            if (!ExpressionArgs (expr, kRxHeaderC, Args (args, &player_number,
                                                   status, handle)))
                return expr->result;
            
            
            return nullptr;
        }
    }
    return nullptr;
}

}       //< namespace _
