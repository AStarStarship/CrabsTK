/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Com/MIDI/ShortMessage.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

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

namespace _Com { namespace MIDI {

#include <-Config.h>

#include <_Com/MIDI/ShortMessage.h>
#include <_Com/MIDI/Port.h>

ShortMessage::ShortMessage  (uint32_t initTimestamp)
:   //theMessage  (0),
    messageNoStatus  (0)
{
    //setTimestamp  (initTimestamp);
}


ShortMessage::ShortMessage  (uint32_t message, uint32_t initTimestamp):
    theMessage  (message)
{
    //byte byte1 = dataByte1 ();
    //byte byte2 = dataByte2 ();

    //messageNoStatus = pack  (byte1, byte2);

    //setTimestamp  (initTimestamp);
}



ShortMessage::ShortMessage  (byte statusByte, byte dataByte1, byte dataByte2, uint32_t initTimestamp)
{
    setMessage  (statusByte, dataByte1, dataByte2);
    messageNoStatus = pack  (dataByte1, dataByte2);

    //setTimestamp  (initTimestamp);
}



ShortMessage::ShortMessage  (byte commandByte, byte channelByte, byte dataByte1, byte dataByte2, uint32_t Timestamp)
{
    setMessage  (commandByte, channelByte, dataByte1, dataByte2);
    messageNoStatus = pack  (dataByte1, dataByte2);

    //setTimestamp  (Timestamp);
}

uint32_t ShortMessage::length () const
{
    return LENGTH;
}

const byte *ShortMessage::message () const
{
    //return reinterpret_cast<const byte *> (&theMessage);
    return 0;
}



byte ShortMessage::status () const
{
    byte statusByte;
    byte dummyVar;

    unpack  (theMessage, statusByte, dummyVar, dummyVar);

    return statusByte;
}



byte ShortMessage::channel () const
{
    byte channelByte;
    byte dummyVar;

    unpack  (theMessage, dummyVar, channelByte, dummyVar, dummyVar);

    return channelByte;
}



byte ShortMessage::command () const
{
    byte commandByte;
    byte dummyVar;

    unpack  (theMessage, commandByte, dummyVar, dummyVar, dummyVar);

    return commandByte;
}



byte ShortMessage::dataByte1 () const
{
    byte dataByte1;
    byte dummyVar;

    unpack  (theMessage, dummyVar, dummyVar, dataByte1, dummyVar);

    return dataByte1;
}



byte ShortMessage::dataByte2 () const
{
    byte dataByte2;
    byte dummyVar;

    unpack  (theMessage, dummyVar, dummyVar, dummyVar, dataByte2);

    return dataByte2;
}



void ShortMessage::setMessage  (byte Status, byte dataByte1, byte dataByte2)
{
    theMessage = pack  (Status, dataByte1, dataByte2);
    messageNoStatus = pack  (dataByte1, dataByte2);
}



void ShortMessage::setMessage  (byte commandByte, byte channelByte, byte dataByte1, byte dataByte2)
{
    theMessage = pack  (commandByte, channelByte, dataByte1, dataByte2);
    messageNoStatus = pack  (dataByte1, dataByte2);
}



uint32_t ShortMessage::pack  (byte dataByte1, byte dataByte2)
{
    /*
    uint32_t thisMessage = dataByte1;
    thisMessage |= dataByte2 << MIDI::SHORT_MSG_SHIFT;

    return thisMessage;
    */ return 0;
}



uint32_t ShortMessage::pack  (byte Status, byte dataByte1, byte dataByte2)
{
    /*
    uint32_t thisMessage = Status;
    thisMessage |= dataByte1 << MIDI::SHORT_MSG_SHIFT;
    thisMessage |= dataByte2 << MIDI::SHORT_MSG_SHIFT * 2;

    return thisMessage;
    */
    return 0;
}



uint32_t ShortMessage::pack  (byte commandByte, byte channelByte, byte dataByte1, byte dataByte2)
{
    /*
    uint32_t thisMessage = commandByte | channelByte;
    thisMessage |= dataByte1 << MIDI::SHORT_MSG_SHIFT;
    thisMessage |= dataByte2 << MIDI::SHORT_MSG_SHIFT * 2;

    return thisMessage;
    */
    return 0;
}



void ShortMessage::unpack  (uint32_t thisMessage, byte &statusByte, byte &dataByte1, byte &dataByte2)
{
    /*
    statusByte = static_cast <byte>  (thisMessage);
    dataByte1  = static_cast <byte>  (thisMessage >> MIDI::SHORT_MSG_SHIFT);
    dataByte2  = static_cast <byte>  (thisMessage >> MIDI::SHORT_MSG_SHIFT * 2);
    */
}



void ShortMessage::unpack  (uint32_t thisMessage, byte &commandByte, byte &channelByte, byte &dataByte1, byte &dataByte2)
{
    /*
    commandByte = static_cast <byte>  (thisMessage & ~MIDI::SHORT_MSG_MASK);
    channelByte = static_cast <byte>  (thisMessage &  MIDI::SHORT_MSG_MASK);
    dataByte1   = static_cast <byte>  (thisMessage >> MIDI::SHORT_MSG_SHIFT);
    dataByte2   = static_cast <byte>  (thisMessage >> MIDI::SHORT_MSG_SHIFT * 2);
    */
}

byte ShortMessage::GetState ()
{
    return 0;
}

const char* ShortMessage::SetState (byte Value)
{
    return 0;
}

const char* ShortMessage::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("ShortMessage", "_Com::MIDI"): InvalidIndex ();
}

}   //< namespace MIDI
}   //< namespace _Com
