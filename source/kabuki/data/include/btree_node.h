/** kabuki::data
    @version 0.x
    @file    ~/source/data/include/btree_.h
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

namespace _Data {

class TNode
{
    public:
    
    TNode (int initIndex);
    /*<  */
    
    TNode (Text initName);
    /*<  */
    
    TNode (Text initName, int initIndex);
    /*<  */
    
    TNode (TNode rootNodeBranch);
    /*<  */
    
    TNode (TNode rootNodeBranch, int initIndex);
    /*<  */
    
    TNode (TNode rootNodeBranch, Text initName);
    /*<  */
    
    TNode (TNode rootNodeBranch, Text initName, int initIndex);
    /*<  */
    
    TNode left ();
    /*<  */
    
    TNode right ();
    /*<  */
    
    TNode rootNode ();
    /*<  */
    
    void setLeftBranch (TNode newBranch);
    /*<  */
    
    void setRightBranch (TNode newBranch);
    /*<  */
    
    void setRootNode (TNode newBranch);
    /*<  */
    
    void* data ();
    /*<  */
    
    void setData (void* newData);
    /*<  */
    
    bool contains (void* Ptr);
    /*<  */
    
    TNode findDepthFirst (void* Ptr);
    /*<  */
    
    Text name ();
    /*<  */
    
    void giveName (Text newName);
    /*<  */
    
    int index ();
    /*<  */
    
    void setIndex (int newIndex);
    /*<  */
    
    inline void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */
    
    private:
    
    void* data;                 //< Pointer to the node contents.

    TNode * left,               //< Pointer to the left child node.
        * right,                //< Pointer to the right child node.
        * rootNode;             //< Pointer to the root node.

    long index;                 //< The index of this node.

    bool hasBeenThisWay,        //< Flag that the left child has been traversed.
        hasBeenThatWay;         //< Flag that the right child has been traversed.
};
}   //< namespace _Data