/* Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/process_tree.h
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
#include <pch.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#include "app.h"

namespace _ {

/* Class that stores a group of app.Window objects. */
class KTH_APP_API ProcessTree
{
    public:
    
    static const SI4 DefaultMaxWindows = 1024;

    /* Default constructor. **/
    ProcessTree ();

    /* Constructor initializes with a single window. */
    ProcessTree (Window& this_window);

    /* Constructor initializes with an array of windows. */
    ProcessTree (TArray<Window> init_windows);

    /* Returns the UID of this Group. */
    uint32_t GetUID ();

    /* Gets and sets the maximum number of windows in this group */
    SI4 GetMaxWindows ();
    
    /* Gets and sets the maximum number of windows in this group */
    void SetMaxWindows (SI4 value);

    /* . */
    SI4 Select (SI4 window_number);

    /* Returns the number of windows in this group. */
    SI4 GetNumWindows ();

    /* Adds a newWindow to the group. */
    void AddWindow (Window newWindow);

    /* Prints this object to the stdout. */
    void Print ();

    private:

    uint32_t uid_;                  //< The UID of this Group.
    SI4 num_windows_,           //< The number of windows in this group.
        max_windows_;           //< The maximum number of windows allowed in a group.
    TArray<Window> windows_;    //< The Window (s) in this group. 
};

}       //< namespace _
#endif  //< KABUKI_TOOLKIT_APP_PROCESSTREE
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

