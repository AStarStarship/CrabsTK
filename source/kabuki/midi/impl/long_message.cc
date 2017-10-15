/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/MIDI/LongMessage.cpp
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

#include <_Theater/MIDI/LongMessage.h>

namespace _Theater { namespace MIDI {

LongMessage::LongMessage  (const char *message, uint32_t length)
:    messageData  (0),
    messageLength  (0)
{
    //setMessage  (message, length);
}

LongMessage::~LongMessage ()
{
    if  (messageData != 0) // Release resources for this object if they exist
        delete [] messageData;
}

LongMessage::LongMessage  (const LongMessage &message)
{
    messageData = 0;
    messageLength = 0;

    *this = message;
}

LongMessage &LongMessage::operator =  (const LongMessage &copyObject)
{
   //if  (this != &copyObject)  // Test for self assignment
    //    setMessage  (copyObject.messageData, copyObject.messageLength);

    return *this;

}

uint32_t LongMessage::getLength () const
{
    return messageLength;
}



byte* LongMessage::getMessageData ()
{
    return messageData;
}



void LongMessage::setMessageData (const char *message, uint32_t length)
{

    if  (messageData != 0) // Release old LongMessage if it exists
        delete [] messageData;

 // Allocate and initialize new LongMessage

    //messageData = new char[length];
    messageLength = length;

    //for  (uint i = 0; i < messageLength; ++i)
    //    messageData[i] = message[i];
}


/*
char& LongMessage::operator []  (int index)
{
    // Warning!!! No bounds checking is being performed!
    return messageData[index];
}
*/

}   //< namespace MIDI
}   //< namespace _Theater
