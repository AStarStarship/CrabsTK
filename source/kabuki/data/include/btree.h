/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/btree.h
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

#ifndef KABUKI_DATA_BTREE_H
#define KABUKI_DATA_BTREE_H

#include "config.h"

namespace kabuki { namespace data {
}       //< namespace data
}       //< namespace kabuki
#endif  //< KABUKI_DATA_BTREE_H

struct BTreeNode
    void* data;                 //< Pointer to the node contents.

    TNode * left,               //< Pointer to the left child node.
        * right,                //< Pointer to the right child node.
        * rootNode;             //< Pointer to the root node.

    long index;                 //< The index of this node.

    bool hasBeenThisWay,        //< Flag that the left child has been traversed.
        hasBeenThatWay;         //< Flag that the right child has been traversed.
};

#include "File.hpp"

namespace _Data {
    
class BinaryTree
/*< A binary tree. 
    @code
    
    @endcode
*/
{
    public:
    
    BinaryTree (size_t Size);
    /*< Constructs a BinaryTree with the given Size. */
    
    ~BinaryTree ();
    /*< Deletes the tree. */
    
    void resetList ();
    /*< Deletes all items. */
    
    const char* randomText (int numChars);
    
    BTNode* findDepthFirst (BTNode* startBTNode, void* Ptr);
    
    BTNode* findDepthFirst (void* Ptr);
    
    bool findBreathFirst (void* Ptr);
    
    bool contains (void* Ptr)

    int getIndex ();

    BTNode* getCurrentNode ();

    IIterator* getIterator ();

    size_t getSize ();
        
    private:
    
    BTNode* parent, 
        * leftChild,
        * rightChild;
    
    int size,
        index;
        
    size_t size;        //< The size of the tree node.
};
    
class InOrderBTIterator : public IIterator
/*< An iterator for a binary tree that iterates in-order.
*/
{
    public:

    InOrderBTIterator (BinaryTree& root);

    void* getCurrentObject ();

    void* getNextObject ();

    void* getPrevObject ();

    Node* getCurrentBTNode ();

    Node* getNextNode ();

    Node* getPrevNode ();

    bool isNotDone ();

    bool isNotDone (int forwardsOrBackwards);// Value to be < or > 0

    void removeLastObject ;

    void replaceLastObject (void* withThis);

    void insertObject (void* thisGuy);

    int getDirection ();
    //< returns Point if last iteration went forwards;

    void setDirection (int positiveNegative);

    bool setIndex (int Index);
    
    private:
    
    BTNode* current;        //< The current object.

};
}
