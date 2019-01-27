/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/app_process_tree.cc
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


#include "process_tree.h"

namespace _ {
    
ProcessTree::ProcessTree ()
{
    maxWindows = DefaultMaxWindows;
    numWindows = 0;
}

ProcessTree::ProcessTree (Window thisWindow)
{
    maxWindows = DefaultMaxWindows;
    numWindows = 1;
    windows.push_back (thisWindow);
}

ProcessTree::ProcessTree (TArray<Window> initWindows)
{
    windows = initWindows;
}

uint32_t ProcessTree::GetUID () { return uID; }

/** Gets and sets the maximum number of windows in this group */
SI4 ProcessTree::GetMaxWindows () { return maxWindows; }

void ProcessTree::SetMaxWindows (SI4 Value)
{
    if (value < 0) return;
    maxWindows = value;
}

/**  */
SI4 ProcessTree::Select (SI4 windowNumber)
{
    if (windowNumber < 0) return -1;
    if (windowNumber >= numWindows) return 0;
    return windowNumber;
}

/** Returns the number of windows in this group. */
SI4 ProcessTree::GetNumWindows { return numWindows; }

/** Adds a newWindow to the group. */
void ProcessTree::AddWindow (Window newWindow) {
}

/** Returns a string representation of this object. */
_::UTF& ProcessTree::Print (_::UTF& print) {
    return "";
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
