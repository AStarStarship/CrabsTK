/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Data/TNode.cpp
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

TNode::
 (int initIndex)
{
    index = initIndex;
}

TNode::TNode (const char* initName)
{
    name = initName;
}

TNode::TNode (const char* initName, int initIndex)
{
    name = initName;
    index = initIndex;
}

TNode::TNode (TNode rootNodeBranch)
{
    rootNode = rootNodeBranch;
}

TNode::TNode (TNode rootNodeBranch, int initIndex)
{
    rootNode = rootNodeBranch;
    index = initIndex;
}

TNode::TNode (TNode rootNodeBranch, const char* initName)
{
    rootNode = rootNodeBranch;
    name = initName;
}

TNode::TNode (TNode rootNodeBranch, const char* initName, int initIndex)
{
    rootNode = rootNodeBranch;
    name = initName;
    index = initIndex;
}

TNode TNode::thisBranch ()
{
    return thisBranch;
}

TNode TNode::thatBranch ()
{
    return thatBranch;
}

TNode TNode::rootNode ()
{
    return rootNode; 
}

void TNode::setLeftBranch (TNode newBranch)
{
    thisBranch = newBranch;
}

void TNode::setRightBranch (TNode newBranch)
{
    thatBranch = newBranch;
}

void TNode::setRootNode (TNode newBranch)
{
    rootNode = newBranch;
}    

void* TNode::data ()
{
    return data;
}
void TNode::setData (void* newData)
{
    data = newData;
}
bool TNode::contains (void* thisObj)
{
    if (data == thisObj)
        return true;
    return false;
}

TNode* TNode::findDepthFirst (void* thisObj)
{
    if (contains (thisObj) )
        return this;
    
    if (thisBranch != nullptr && haveBeenThisWay == false)// can go this way
    {
        haveBeenThisWay = true;// stores that you've gone this way
        return thisBranch.findDepthFirst (thisObj);
    }
    if (thatBranch != nullptr && haveBeenThatWay == false)// can go that way
    {
        haveBeenThatWay = true;// stores that you've gone that way
        return thatBranch.findDepthFirst (thisObj);
    }
    if (rootNode != nullptr)// cant go this or that way so go up
    {
        haveBeenThisWay = haveBeenThatWay = false;// were done with this optree so we can
        return rootNode.findDepthFirst (thisObj); // reset the flags so that we can search
    }                                                          // again without resetting the whole tree.
    return nullptr;
}

const char* TNode::getName ()
{
    return name;
}

void TNode::setName (const char* newName)
{
    name = newName;
}

int TNode::getIndex ()
{
    return index;
}

void TNode::setIndex (int newIndex)
{
    index = newIndex;
}

void TNode::print (I2P::Terminal& slot)
{
    print (slot, name, "/n");
    data.print (slot);
}

}   //< namespace _Data
