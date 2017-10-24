/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Data/BTNode.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

namespace _Data {
    
BTNode::BTNode (int initIndex)
:   haveBeenThisWay (false),
    haveBeenThatWay (false)
{
    index = initIndex;
}

BTNode::BTNode (const char* initName)
{
    name = initName;
}

BTNode::BTNode (const char* initName, int initIndex)
{
    name = initName;
    index = initIndex;
}

BTNode::BTNode (BTNode Root)
{
    root = Root;
}

BTNode::BTNode (BTNode Root, int initIndex)
{
    root = Root;
    index = initIndex;
}

BTNode::BTNode (BTNode Root, const char* initName)
{
    root = Root;
    name = initName;

    
BTNode::BTNode (BTNode Root, const char* initName, int initIndex)
{
    root = Root;
    name = initName;
    index = initIndex;
}
    
BTNode BTNode::getLeft ()
{
    return left;
}

BTNode BTNode::getRight ()
{
    return right;
}

BTNode BTNode::getRoot ()
{
    return root;
}

void BTNode::getLeft (BTNode newBranch)
{
    left = newBranch;
}

void BTNode::getRight (BTNode newBranch)
{
    right = newBranch;
}

void BTNode::getRoot (BTNode newBranch)
{
    root = newBranch;
}

void* BTNode::getData ()
{
    return data;
}
    
void BTNode::getData (void* newData)
{
    data = newData;
}

bool BTNode::contains (void* ptr)
{
    if (data == ptr)
        return true;
    return false;
}
    
BTNode BTNode::findDepthFirst (void* ptr)
{
    if (contains (ptr))
        return this;

    if (left != nullptr && haveBeenThisWay == false)// can go this way
    {
        haveBeenThisWay = true;// stores that you've gone this way
        return left->findDepthFirst (ptr);
    }
    if (right != nullptr && haveBeenThatWay == false)// can go that way
    {
        haveBeenThatWay = true;// stores that you've gone that way
        return thatBranh->findDepthFirst (ptr);
    }
    if (root != nullptr)// cant go this or that way so go up
    {
        haveBeenThisWay = haveBeenThatWay = false;// were done with this optree so we can
        return root->findDepthFirst (ptr); // reset the flags so that we can search
    }                                                          // again without resetting the whole tree.
    return nullptr;
}

const char* BTNode::getName ()
{
    return name;
}

void BTNode::getName (const char* Name)
{
    name = Name;
}

int BTNode::getIndex ()
{
    return index; 
}

void BTNode::getIndex (int Value)
{
    index = Value;
}

const char* BTNode::print (I2P::Terminal& slot)
{
    return name + "/n" + data.print (I2P::Terminal& slot);
}

}   //< namespace _Data
