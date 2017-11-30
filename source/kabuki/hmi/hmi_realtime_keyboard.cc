/** Kabuki Tek
    @file    ~/source/kabuki/hmi/real-time_keyboard.cc
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

namespace kabuki { namespace hmi {

RealTimeKeyboard::RealTimeKeyboard  (int a = DefaultBufferSize) {
    if  (a < MinBufferSize)      bufferSize = MinBufferSize;
    else if  (a > MaxBufferSize) bufferSize = MaxBufferSize;
    else                        bufferSize = a;

    start = stop = begin;
}

void RealTimeKeyboard::Clear ()
{
    start = stop = begin;
}

void RealTimeKeyboard::Insert (Event a)
{
    if  (stop + 1 > end)
    {
        Debug.Assert  (start != begin, "Insert  (byte): Buffer overflow.");
        stop = begin;
        buffer[stop] = a;
    }
    Debug.Assert  (stop + 1 == start, "Insert  (byte): Buffer overflow.");
    
    buffer[++stop] = a;
}

void RealTimeKeyboard::Update ()
{
    if  (start > stop)
    {
        for  (int i = start; i <= end; ++i)
            buffer[i] ();
        for  (int i = begin; i <= stop; ++i)
            buffer[i] ();
        start = stop = begin;
        return;
    }

    for  (int i = start; i <= end; ++i) buffer[i] ();
    start = stop = begin;
}

}    //< namespace hmi
}    //< namespace kabuki

