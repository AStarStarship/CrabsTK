/** Kabuki Toolkit
    @file    ~/library/kabuki/toolkit/app/process_tree.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#include "app.h"

namespace app {

/** Class that stores a group of app.Window objects. */
class KTH_APP_API ProcessTree
{
    public:
    
    static const int DefaultMaxWindows = 1024;

    /** Default constructor. **/
    ProcessTree ();

    /** Constructor initializes with a single window. */
    ProcessTree (Window& this_window);

    /** Constructor initializes with an array of windows. */
    ProcessTree (vector<Window> init_windows);

    /** Returns the UID of this Group. */
    uint32_t GetUID ();

    /** Gets and sets the maximum number of windows in this group */
    int GetMaxWindows ();
    
    /** Gets and sets the maximum number of windows in this group */
    void SetMaxWindows (int value);

    /** . */
    int Select (int window_number);

    /** Returns the number of windows in this group. */
    int GetNumWindows ();

    /** Adds a newWindow to the group. */
    void AddWindow (Window newWindow);

    /** Prints this object to the stdout. */
    void Print ();

    private:

    uint32_t uid_;                  //< The UID of this Group.
    int num_windows_,           //< The number of windows in this group.
        max_windows_;           //< The maximum number of windows allowed in a group.
    vector<Window> windows_;    //< The Window (s) in this group. 
};

}       //< namespace app
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_APP_PROCESSTREE
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

