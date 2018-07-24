/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/midi_long_message.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "long_message.h"

namespace _ {

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

uint32_t LongMessage::GetLength () const
{
    return messageLength;
}



byte* LongMessage::GetMessageData ()
{
    return messageData;
}



void LongMessage::SetMessageData (const char *message, uint32_t length)
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

}       //< namespace _
