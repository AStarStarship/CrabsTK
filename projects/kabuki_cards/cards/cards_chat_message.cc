/** kabuki:cards
    @file    ~/source/kabuki/arcade/cards/chat_message.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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
#include "chat_message.h"

namespace kabuki { namespace cards {

ChatMessage::ChatMessage () :
    body_length_ (0) {
}

const char* ChatMessage::Data () const {
    return data_;
}

char* ChatMessage::Data () {
    return data_;
}

std::size_t ChatMessage::Length () const {
    return kHeaderLength + body_length_;
}

const char* ChatMessage::Body () const {
    return data_ + kHeaderLength;
}

char* ChatMessage::Body () {
    return data_ + kHeaderLength;
}

std::size_t ChatMessage::BodyLength () const {
    return body_length_;
}

void ChatMessage::BodyLength (std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > kMaxBodyLength)
        body_length_ = kMaxBodyLength;
}

bool ChatMessage::DecodeHeader () {
    char header[kHeaderLength + 1] = "";
    //std::strncat (header, data_, kHeaderLength);
    body_length_ = std::atoi (header);
    if (body_length_ > kMaxBodyLength) {
        body_length_ = 0;
        return false;
    }
    return true;
}

void ChatMessage::EncodeHeader () {
    char header[kHeaderLength + 1] = "";
    //std::sprintf (header, "%4d", static_cast<int>(body_length_));
    //std::memcpy (data_, header, kHeaderLength);
}

}   //< namespace cards
}   //< namespace kabuki
