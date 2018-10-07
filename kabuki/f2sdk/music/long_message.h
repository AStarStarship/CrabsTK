/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/midi/long_message.h
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

#pragma once
#include <pch.h>

#include "message.h"

namespace _ {

/** Class that represents a MIDI SYsEx SYsExMessage.
    A SysExMessage extends a standard MIDI Message. */
class API LongMessage
{
    public:
	
    /** Simple default constructor. */
    LongMessage  (const char* Msg = nullptr, uint32_t Length = 0);
	
    /** Simple copy constructor. */
    LongMessage  (const LongMessage& O);

    virtual ~LongMessage ();

    /** Gets the length of this message. */
    virtual uint32_t GetLength () const;
    
    /** Gets the message. */
	byte* GetMessageData ();
	
    /** Sets thisMessage to the newMessage. */
    void SetMessageData (const char* new_message, uint32_t Length);

    /** Operator (= ) overload. */
    LongMessage &operator=  (const LongMessage &O);

    protected:

    //char& operator []  (int i);
    /** This is for derived classes to use in order to access the individual bytes within a SYsExMessage object.
        @warning No bounds checking is being performed! */
	
    private:

    byte* messageData;
    uint32_t messageLength;
};

}       //< namespace _
