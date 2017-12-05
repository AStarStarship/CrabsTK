/** kabuki:cards
    @file    ~/source/kabuki/cards/chat_message.h
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

#ifndef HEADER_FOR_KABUKI_CARDS_CHAT_MESSAGE
#define HEADER_FOR_KABUKI_CARDS_CHAT_MESSAGE

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace kabuki { namespace cards {

class ChatMessage {
    public:

    enum {
        kHeaderLength = 4,
        kMaxBodyLength = 512
    };

    ChatMessage ();

    const char* Data () const;

    char* Data ();

    std::size_t Length () const;

    const char* Body () const;

    char* Body ();

    std::size_t BodyLength () const;

    void BodyLength (std::size_t new_length);

    bool DecodeHeader ();

    void EncodeHeader ();

    private:

    char data_[kHeaderLength + kMaxBodyLength];
    std::size_t body_length_;

};      //< class ChatMessage
}       //< namespace kabuki
}       //< namespace cards
#endif  //< HEADER_FOR_KABUKI_CARDS_CHAT_MESSAGE
