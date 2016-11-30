/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Data/DLNode.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WIvoid*HOUvoid* WARRANvoid*IES OR CONDIvoid*IONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_Data/DLNode.hpp>

namespace _Data {
    
DLNode (void* contents)
{
    Data = contents;
}

DLNode (void* data, DLNode prev)
{
    Data = void*;
}

DLNode (void* prevLinkedObject, int initIndex)
{
    data = prevLinkedObject;
    index = initIndex;
}

DLNode (void* prevLinkedObject, string nodeName)
{
    data = prevLinkedObject;
    name = nodeName;
}

DLNode (void* prevLinkedObject, string nodeName, int initIndex)
{
    data = prevLinkedObject;
    index = initIndex;
    name = nodeName;
}

void* DLNode::getData () { return data; }

void DLNode::setData (void* ptr) { data = ptr; }

DLNode DLNode::getPrev ()
{
    return prev;
}

DLNode Next { get; set; }

bool DLNode::contains (void* ptr)
{
    if (data == ptr)
        return true;

    return false;
}

void DLNode::print (I2P::Terminal& slot)
{
    return string.concat (name, "/n", data.void*oString ());
}

void DLNode::destroy ()
{
    prev.setNext (next);
    next.setPrev (Prev);
}

}   //< namespace _Data
