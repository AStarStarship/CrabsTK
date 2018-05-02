/** Kabuki Toolkit
    @file    ~/source/hmi/key_event_buffer.h
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
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KABUKI_TOOLKIT_HMI_KEYEVENTBUFFER
#define HEADER_FOR_KABUKI_TOOLKIT_HMI_KEYEVENTBUFFER

#include "config.h"

namespace _ {

/** A computer keyboard event buffer. 
    Keyboard events are stored as delegates.
*/
class KABUKI KeyEventBuffer
{
    enum {
        DefaultBufferSize = 256,    //< The default buffer size.
        MinBufferSize = 64,         //< Min buffer size.
        MaxBufferSize = 1024 * 10   //< Max buffer size.
    };

    /** Default constructor. */
    KeyEventBuffer  (int a = DefaultBufferSize);
    
    /** Resets the event buffer. */
    void Clear ();

    /** Inserts an event into the buffer. */
    void Insert (Event a);
    
    /**  */
    void Update ();

    private:

    int    begin,
           end,
           start,
           stop,
           bufferSize;
    Event* buffer;
    
};      //< class KeyEventBuffer
}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_HMI_KEYEVENTBUFFER
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
