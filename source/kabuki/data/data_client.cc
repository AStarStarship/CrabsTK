/** Kabuki Toolkit
    @file    ~/source/kabuki/data/data_card_game.cc
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
#include "client.h"

using namespace _;
using namespace kabuki::id;

namespace kabuki { namespace data {

Client::Client () {
}

Client::~Client () {
}

int Client::GetState () {
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

const Operation* Client::Star (uint index, _::Expression* expr) {
    static const Operation This = { "Client",
        OperationCount (0), OperationFirst ('A'),
        "Datagram security client.", 0 };
    void* args[4];
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
            {
                return expr->result;
            }
            SetState (state);
            return nullptr;
        }
    }
    return nullptr;
}

const char* Client::Sudo (const char* strand, const char* strand_end) {
    return strand;
}

}       //< namespace data
}       //< namespace kabuki
