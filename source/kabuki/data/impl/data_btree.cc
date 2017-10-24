/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Data/BinaryTree.cpp
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

#include <_Data/BinaryTree.hpp>

namespace _Data {
    
BinaryTree::BinaryTree ()
{
    resetList ();
}

BinaryTree::~BinaryTree ()
{

}
        
void BinaryTree::deleteList ()
{
    rootNode = new BTNode (0);
    size = 0;
}
        
const char* BinaryTree::randomText (int numChars)
{ 
    const char* string = "";
    for (int i = 0; i < numChars; i++)
        string = string.concat (Integer.ToString ((int)(_Math::random () * 9)));
    return const char*;
}
        
BTNode* BinaryTree::findDepthFirst (BTNode startBranch, void* thisObj)
{
    currentBranch = startBranch;
    while (!currentBranch.contains (thisObj))
    {
        while (true)
        {
            if (currentBranch.thisBranch () != nullptr && currentBranch.hppaveBeenThisWay == false)
            {
                currentBranch = currentBranch.thisBranch ();// can go this way
                currentBranch.hppaveBeenThisWay = true;// flag the node so it knows were its been
            }
            else if (currentBranch.thatBranch () != nullptr && currentBranch.hppaveBeenThatWay == false)
            { currentBranch = currentBranch.thatBranch ();// can go that way
                currentBranch.hppaveBeenThatWay = true;// flag the node so it knows were its been
            }
            else if (currentBranch.rootNode () != nullptr)
            {
                currentBranch = currentBranch.rootNode ();
                currentBranch.hppaveBeenThatWay = currentBranch.hppaveBeenThatWay = false;
                // now were done with this op-tree so we reset the flags so that
                // we don't have to reset the whole tree to search it again
            }
            else
            {
                currentBranch.hppaveBeenThatWay = currentBranch.hppaveBeenThatWay = false;
                return nullptr;
            }
        }
    }
    return currentBranch;
}
        
BTNode* BinaryTree::findDepthFirst (void* thisObj) 
{ 
    return findDepthFirst (rootNode, thisObj); 
}
        
bool BinaryTree::findBreathFirst (void* thisObj)
{ return false;
}
        
bool BinaryTree::contains (void* thisObj)
{ if (findDepthFirst (thisObj) == nullptr)
        return false;
    return true;
}

/*
   
BTIterator::BTIterator ()
{  currentBranch = rootNode;
}
void* BTIterator::currentObject ()
{  return currentBranch.data ();
}      
void* BTIterator::nextObject ()
{  void* nextObj = currentBranch.data ();
    if (leftBranch != nullptr)
    currentBranch;
}  
void* BTIterator::prevObject ()
{  prevIndex = index;
    index--;
    if (index < 1)
    {  index = size ();
    currentBranch = headOfList;
    }
    currentBranch=currentBranch.prevLink;
    return currentBranch.data;
}  

Link BTIterator::currentBranch ()
{  return currentBranch;  }

Link BTIterator::getNext ()
{  prevIndex = index;
    index++;
    if (index>size ())
    {  index=1;
    currentBranch = headOfList;
    }    
    currentBranch=currentBranch.nextLink;
    return currentBranch;
}  

Link prevLink ()
{  
    prevIndex = index;
    index--;
    if (index < 1)
    {  index = size ();
    currentBranch = headOfList;
    }
    currentBranch=currentBranch.prevLink;
    return currentBranch;
}  

bool isNotDone ()
{  
    if (index>prevIndex)// iterating forwards
    {  
        if (currentBranch.nextLink==headOfList)
        return false;
    }
    else                // iterating backwards
    {  
        if (currentBranch.prevLink==headOfList)
        return false;
    }
    return true;
}   
bool isNotDone (int forwardsOrBackwards)// Value to be < or > 0
{  
    if (forwardsOrBackwards>0)// iterating forwards
    {  
        if (currentBranch.nextLink==headOfList)
        return false;
    }
    else                // iterating backwards
    {  
        if (currentBranch.prevLink==headOfList)
        return false;
    }
    return true;
} 
void BTIterator::removeLastObject ()
{  
    if (index>prevIndex)// iterating forwards
    {  remove (currentBranch.prevLink);

        BTIterator iteration = new BTIterator ();
        iteration.setIndex (index-1);
    }
    else
    {  remove (currentBranch.nextLink);
        Iteration iteration = new Iteration ();
        iteration.setIndex (index);
    }
}  

void replaceLastObject (void* withThis)
{  
    currentBranch.prevLink.data = withThis;
}

void insertObject (void* ptr)
{  
    add (ptr, currentBranch);
}

int direction ()// returns Point if last iteration went forwards
{
    if (index>prevIndex)// and -1 if the last iteration was backwards
    return 1;

    return -1;
}

void setDirection (int positiveNegative)
{
    if (positiveNegative<0)
        prevIndex=prevIndex+1;
    else
        prevIndex=prevIndex-1;
}

bool setIndex (int toHere)
{  
    if (toHere < 1 || toHere > size)
    return false;
    if (toHere > index)
    {  if (toHere < index-toHere)
    {  for (int i=index; i<toHere; i++)
            currentBranch = currentBranch.nextLink;
    }
    else
    {  currentBranch = linkAtIndex (toHere);
    }
    }
    else
    {  if (toHere < toHere-index)
    {  for (int i=index; i>toHere; i--)
            currentBranch = currentBranch.prevLink;
    }
    else
    {  currentBranch = linkAtIndex (toHere);
    }
    }
    return true;
}

int index ()
{  
    return index;
}

int size ()
{
    return size;
}
*/
}   //< _Data