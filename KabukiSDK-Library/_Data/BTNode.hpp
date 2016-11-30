/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/_Data/BTNode.hpp
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

#include <_Data/Data.hpp>

namespace _Data {
    
    /** A node in a binary tree. */
    class BTNode : public Node
    {
        void* data;
        BTNode left, right, root;
        const char* name;
        int index;
        bool haveBeenThisWay = false,
                       haveBeenThatWay = false;

        BTNode (int initIndex)
        {
            index = initIndex;
        }

        BTNode (const char* initName)
        {
            name = initName;
        }

        BTNode (const char* initName, int initIndex)
        {
            name = initName;
            index = initIndex;
        }

        BTNode (BTNode rooBTNodeBranch)
        {
            root = rooBTNodeBranch;
        }
        BTNode (BTNode rooBTNodeBranch, int initIndex)
        {
            root = rooBTNodeBranch;
            index = initIndex;
        }
        BTNode (BTNode rooBTNodeBranch, const char* initName)
        {
            root = rooBTNodeBranch;
            name = initName;
        }
        BTNode (BTNode rooBTNodeBranch, const char* initName, int initIndex)
        {
            root = rooBTNodeBranch;
            name = initName;
            index = initIndex;
        }
        BTNode left ()
        {
            return left;
        }
        BTNode right ()
        {
            return right;
        }
        BTNode root ()
        {
            return root;
        }
        void setLeftBranch (BTNode newBranch)
        {
            left = newBranch;
        }
        void setRightBranch (BTNode newBranch)
        {
            right = newBranch;
        }
        void setRooBTNode (BTNode newBranch)
        {
            root = newBranch;
        }
        void* data ()
        {
            return data;
        }
        void setData (void* newData)
        {
            data = newData;
        }
        bool contains (void* Ptr)
        {
            if (data == Ptr)
                return true;
            return false;
        }
        BTNode FindDepthFirst (void* Ptr)
        {
            if (contains (Ptr))
                return this;

            if (left != nullptr && haveBeenThisWay == false)// can go this way
            {
                haveBeenThisWay = true;// stores that you've gone this way
                return left->findDepthFirst (Ptr);
            }
            if (right != nullptr && haveBeenThatWay == false)// can go that way
            {
                haveBeenThatWay = true;// stores that you've gone that way
                return thatBranh->findDepthFirst (Ptr);
            }
            if (root != nullptr)// cant go this or that way so go up
            {
                haveBeenThisWay = haveBeenThatWay = false;// were done with this optree so we can
                return root.findDepthFirst (Ptr); // reset the flags so that we can search
            }                                                          // again without resetting the whole tree.
            return nullptr;
        }

        const char* Name
        {
            get { return name; }
            set { name = Value; }
        }
        
        int Index
        {
            get { return index; }
            set { index = Value; }
        }
        
        const char* print (I2P::Terminal& slot)
        {
            return name + "/n" + data.print (I2P::Terminal& slot);
        }
    }
}