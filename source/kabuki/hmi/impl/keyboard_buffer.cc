/** Kabuki Theater
    @file    /.../Source/_HMI/Keyboard/KeyboardBuffer.h
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


namespace _HMI {

KeyboardBuffer::KeyboardBuffer  (int a)
{
    if  (a < MinBufferSize)      bufferSize = MinBufferSize;
    else if  (a > MaxBufferSize) bufferSize = MaxBufferSize;
    else                         bufferSize = a;

    start = stop = begin;
}

void KeyboardBuffer::clear ()
{
    start = stop = begin;
}

void KeyboardBuffer::insert (Event a)
{
    if  (stop + 1 > end)
    {
        assert  (start != begin, "Insert  (byte): Buffer overflow.");
        stop = begin;
        buffer[stop] = a;
    }
    assert  (stop + 1 == start, "Insert  (byte): Buffer overflow.");
    
    buffer[++stop] = a;
}

void KeyboardBuffer::update ()
{
    if  (start > stop)
    {
        for  (int i = start; i <= end; ++i) buffer[i] ();
        for  (int i = begin; i <= stop; ++i) buffer[i] ();
        start = stop = begin;
        return;
    }

    for  (int i = start; i <= end; ++i) buffer[i] ();
    start = stop = begin;
}

}   //< namespace HMI
}   //< _Theater
