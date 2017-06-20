/** Kabuki Theater
    @file    /.../Source/_Theater/MIDI/ShortMessage.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once


#include <KabukiTheater-Config.hpp>
#include "Message.hpp"

namespace _Theater { namespace MIDI {
    
class _KabukiTheater_ Port;

/** Represents a short MIDI message. */
class _KabukiTheater_ ShortMessage: public Message
{
      public:

    explicit ShortMessage  (uint32_t TimeStamp = 0);
    
    ShortMessage  (uint32_t Msg, uint32_t TimeStamp = 0);
    ShortMessage  (uint8_t Status, uint8_t Data1, uint8_t Data2, uint32_t TimeStamp);
    ShortMessage  (uint8_t Command, uint8_t Channel, uint8_t Data1, uint8_t Data2, uint32_t TimeStamp);

    //void send  (Port &thisDevice); //< Sends this Message out thisDevice.
    //void SendMsgNoStatus  (midi::CMIDIOutDevice &OutDevice); //< Sends message without status uint8_t

    uint32_t getLength () const;

    const uint8_t *getMessage () const;
    uint8_t getStatus () const;
    uint8_t getChannel () const;
    uint8_t getCommand () const;
    uint8_t getDataByte1 () const;
    uint8_t getDataByte2 () const;

    void setMessage  (uint8_t statusBYte, uint8_t byte1, uint8_t byte2);
    void setMessage  (uint8_t commandBYte, uint8_t channelBYte, uint8_t byte1,
        uint8_t byte2);

    /** Packs a short messages without status uint8_t. */
    static uint32_t pack  (uint8_t byte1, uint8_t byte2);
    
    /** Packs a short messages with status uint8_t. */
    static uint32_t pack  (uint8_t statusBYte, uint8_t byte1, uint8_t byte2);
    
    /** Packs a short messages on a given channel. */
    static uint32_t pack  (uint8_t commandBYte, uint8_t channelBYte, uint8_t byte1,
        uint8_t byte2);

    /** Unpacks AMessage. */
    static void unpack  (uint32_t AMessage, uint8_t &statusBYte, uint8_t &byte1,
        uint8_t &byte2);
        
    /** Unpacks AMessage. */
    static void unpack  (uint32_t AMessage, uint8_t &commandBYte, uint8_t &channelBYte,
        uint8_t &byte1, uint8_t &byte2);

    static const uint32_t Length = 3;

    private:

    uint32_t message,
        messageNoStatus;
};

}   //< namespace MIDI

