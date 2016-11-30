/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Impl/_Data/LLList.cpp
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

LinkedList::LinkedList ()
{
    headOfList = new LLNode<T> (randomText (8));
    ResetList ();
}

LinkedList::LinkedList (int maxNumOfElements)
{
    headOfList = new LLNode (randomText (8));
    resetList ();

    if (maxNumOfElements > 0)
    {
        maxSize = maxNumOfElements;
    }
}


LinkedList::LinkedList (File listHead)
{
    headOfList = new LLNode (listHead);
    resetList ();
}

void LinkedList::resetList ()
{ headOfList.setThisLink (headOfList);
    headOfList.setThatLink (headOfList);
    size = 0;
}

bool LinkedList::add (void* newLink)
{ return insert (newLink, headOfList);
}

bool LinkedList::insert (void* newLink)
{
    return add (newLink, headOfList);
}

bool LinkedList::insert (void* newLink, int indexNum)
{ 
    return insert (newLink, linkAtIndex (indexNum)); 
}

bool LinkedList::add (void* newLink, LLNode afterHere)
{ 
    if (newLink == headOfList || size == maxSize)
        return false;

    size++;
    LLNode newLinkedObj = new LLNode (newLink, size);
    newLinkedObj.setThisLink (afterHere);
    newLinkedObj.setThatLink (afterHere.getRightChild ());

    afterHere.getRightChild ().setThisLink (newLinkedObj);
    afterHere.setThatLink (newLinkedObj);

    return true;
}

bool LinkedList::insert (void* newLink, LLNode beforeHere)
{ 
    if (newLink == headOfList || size == maxSize)
        return false;

    size++;
    LLNode newLinkedObj = new LLNode (newLink, size);
    newLinkedObj.setThatLink (beforeHere);
    newLinkedObj.setThisLink (beforeHere.getLeftChild ());

    beforeHere.getLeftChild ().setThatLink (newLinkedObj);
    beforeHere.setThisLink (newLinkedObj);

    return true;
}

LLNode LinkedList::find (void* objectToFind)
{
    Iterator iteration = new LLListIterator ();
    void* thisobject;
    while (iteration.isNotDone ())
    {
        thisobject = iteration.getNext ();
        if (thisobject == objectToFind)// iterator void*
            return iteration.currentLink ();
    }
    return nullptr;
}

bool LinkedList::remove (void* ptr) { return Remove (find (ptr)); }

bool LinkedList::remove (int atIndex)
{
    if (atIndex > 0 && atIndex <= size)
    {
        Iterator iteration = new LLListIterator ();
        void* thisobject;

        for (int i = 0; i < atIndex; i++)
            iteration.Next ();

        iteration.currentLink ().delete ();
        return true;
    }
    return false;
}

bool LinkedList::remove (int index1, int index2)
{
    // Index1 can not be less than zero or greater than index1
    if (index1 < 0 || index2 > size + 1 || index2 < index1)
    {
        Iterator iteration = new LLListIterator ();

        for (int i = 0; i < index1; i++)
            iteration.getNext ();

        for (int i = index1; i < index2; i++)
        {
            iteration.currentLink ().delete ();
            size--;
        }
        return true;
    }
    return false;
}

bool LinkedList::remove (LLNode* leftChild)
{
    if (leftChild.equals (headOfList))
        return false;

    leftChild.destroy ();
    size--;

    return true;
}

bool LinkedList::replace (int atIndex, void* withThis)
{
    if (atIndex > 0 && atIndex <= size)
    {
        linkAtIndex (atIndex).setData (withThis);
        return true;
    }
    return false;
}

bool LinkedList::keepOnlyThese (int[] indexList)
{
    if (indexList.length <= size)
    {
        for (int i = 0; i < indexList.length; i++)
            if (indexList[i] < 0 || indexList[i] > size)
                return false;

        Iterator iteration = new LLListIterator ();

        for (int i = 1; i < indexList.length; i++)
        {
            for (int a = indexList[i - 1]; a < indexList[i]; a++)
                iteration.currentLink ().delete ();
        }
    }
    return true;
}

bool LinkedList::contains (void* o)
{
    void* currentobject;

    Iterator iteration = linearIterator ();

    while (iteration.isNotDone ())
    {
        currentobject = iteration.getNext ();

        if (currentobject == o)
            return true;
    }
    return false;
}

void* LinkedList::element (int atIndex)
{
    if (atIndex > 0 && atIndex < size)
    {
        Iterator iteration = new LLListIterator ();
        for (int i = 0; i < atIndex; i++)
            iteration.getNext ();
        return iteration.currentobject ();
    }
    return nullptr;
}
LLNode* LinkedList::getLinkAtIndex (int atIndex)
{
    if (atIndex > 0 && atIndex < size)
    {
        Iterator iteration = new LLListIterator ();

        for (int i = 0; i < atIndex; i++)
            iteration.getNext ();
        return iteration.currentLink ();
    }
    return nullptr;
}

bool LinkedList::isEmpty ()
{
    if (size == 0)
        return true;
    return false;
}

int LinkedList::getSize ()
{
    return size;
}

int LinkedList::getMaxSize ()
{
    return maxSize;
}

const char* LinkedList::randomText (int numChars)
{
    const char* const char* = "";
    for (int i = 0; i < numChars; i++)
        const char* = const char*.concat (Int32.ToString ((int)(Math.random ()9)));
    return const char*;
}

void LinkedList::increaseMaxSize (int increase)
{
    maxSize += Math.abs (increase);// Sorry, no negative numbers
}

void** LinkedList::toArray ()
{
    void** objectArray = new void*[size];

    int i = 0;
    Iterator iteration = linearIterator ();
    while (iteration.isNotDone ())
    {
        objectArray[i] = iteration.getNext ();
        i++;
    }
    return objectArray;
}

Iterator LinkedList::getIterator ()
{
    return new LLListIterator ();
}

Iterator* getLinearIterator ()
{
    return new LLListIterator ();
}



LLListIterator::LLListIterator ()
{
    currentLink = headOfList;
}

void* LLListIterator::currentobject ()
{
    return currentLink->data ();
}

void* LLListIterator::getNextobject ()
{
    inde = index;
    index++;
    if (index > size ())
    {
        index = 1;
        currentLink = headOfList;
    }
    currentLink = currentLink.getRightChild ();
    return currentLink.data ();
}

void* LLListIterator::getPrevobject ()
{
    inde = index;
    index--;

    if (index < 1)
    {
        index = size ();
        currentLink = headOfList;
    }

    currentLink = currentLink.getLeftChild ();

    return currentLink.data ();
}

LLNode* LLListIterator::getCurrentLink ()
{
    return currentLink;
}

LLNode* LLListIterator::getNextLink ()
{
    inde = index;
    index++;
    if (index > size ())
    {
        index = 1;
        currentLink = headOfList;
    }
    currentLink = currentLink.getRightChild ();
    return currentLink;
}

LLNode LLListIterator::getPrevLink ()
{
    inde = index;
    index--;
    if (index < 1)
    {
        index = size ();
        currentLink = headOfList;
    }
    currentLink = currentLink.getLeftChild ();
    return currentLink;
}

bool LLListIterator::isNotDone ()
{
    if (index > inde)// iterating forwards
    {
        if (currentLink.getRightChild () == headOfList)
            return false;
    }
    else                     // iterating backwards
    {
        if (currentLink.getLeftChild () == headOfList)
            return false;
    }
    return true;
}

bool LLListIterator::isNotDone (int forwardsOrBackwards)// Value to be < or > 0
{
    if (forwardsOrBackwards > 0)// iterating forwards
    { if (currentLink.getRightChild () == headOfList)
            return false;
    }
    else                     // iterating backwards
    {
        if (currentLink.getLeftChild () == headOfList)
            return false;
    }
    return true;
}

void LLListIterator::removeLastobject ()
{
    if (index > inde)// iterating forwards
    {
        remove (currentLink.getLeftChild ());
        LLListIterator iteration = new LLListIterator ();
        iteration.setIndex (index - 1);
    }
    else
    {
        remove (currentLink.getRightChild ());
        LLListIterator iteration = new LLListIterator ();
        iteration.setIndex (index);
    }
}

void LLListIterator::replaceLastobject (void* withThis)
{
    currentLink.getLeftChild ().setData (withThis);
}

void LLListIterator::insertobject (void* ptr)
{
    add (ptr, currentLink);
}

int LLListIterator::direction ()// returns position if last iteration went forwards
{
    if (index > inde)// and -1 if the last iteration was backwards
        return 1;

    return -1;
}

void LLListIterator::setDirection (int positiveNegative)
{
    if (positiveNegative < 0)
        inde = inde + 1;
    else
        inde = inde - 1;
}

bool LLListIterator::setIndex (int toHere)
{
    if (toHere < 1 || toHere > size)
        return false;
    if (toHere > index)
    {
        if (toHere < index - toHere)
        {
            for (int i = index; i < toHere; i++)
                currentLink = currentLink.getRightChild ();
        }
        else
        {
            currentLink = linkAtIndex (toHere);
        }
    }
    else
    {
        if (toHere < toHere - index)
        {
            for (int i = index; i > toHere; i--)
                currentLink = currentLink.getLeftChild ();
        }
        else
        {
            currentLink = linkAtIndex (toHere);
        }
    }
    return true;
}

int LLListIterator::index ()
{
    return index;
}

int LLListIterator::size ()
{
    return size;
}

}   //< namespace _Data
