/** Kabuki Theater
    @file    ~/Source/_Dev/Keyboard/KeyEventBuffer.h
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


namespace _ {
    
class _HMI_API KeyEventBuffer
/*< A computer keyboard event buffer. 
    Keyboard events are stored as delegates.
*/
{
    enum {
        DefaultBufferSize = 256,    //< The default buffer size.
        MinBufferSize = 64,         //< Min buffer size.
        MaxBufferSize = 1024 * 10   //< Max buffer size.
    };

    KeyEventBuffer  (int a = DefaultBufferSize);
    /*< Default constructor. **/
    
    void clear ();
    /*< Resets the event buffer. */

    void insert (Event a);
    /*< Inserts an event into the buffer. */
    
    void update ();
    /*<  */

    private:

    int begin,
        end,
        start,
        stop,
        bufferSize;

    Event* buffer;
};

}   //< _hmi
}   //< _Theater
