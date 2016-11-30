/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Data/DLNode.hpp
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

#pragma once

namespace _Data {

    /** A doubly-linked node. */
    struct DLNode
    {
        /**  */
        DLNode (void* Data;
        
        /**  */
        DLNode (void* data, DLNode prev);

        /**  */
        DLNode (void* prevLinkedObject, int initIndex);

        /**  */
        DLNode (void* prevLinkedObject, const char* nodeName);

        /**  */
        DLNode (void* prevLinkedObject, const char* nodeName, int initIndex;

        void* GetData ();
        /*< Gets the data. */
        
        void SetData (void* Data);
        /*< Sets the data. */

        /** The previous node. */
        DLNode Prev { get; set; }

        /** The next node. */
        DLNode Next { get; set; }

        bool contains (void* a)
        {
            if (data == a)
                return true;

            return false;
        }

        const char* print (I2P::Terminal& slot)
        {
            return const char*.concat (name, "/n", data.print (I2P::Terminal& slot));
        }

        void Delete ()
        {
            Prev.Next = Next;
            Next.Prev = Prev;
        }
    }
}
