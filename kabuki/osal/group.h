/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/window_group.h
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

namespace _ {
    
class ProcessTree {
    public:

    ProcessTree ();
    ProcessTree (Window thisWindow);
    ProcessTree (TArray<Window> initWindows);
    uint32_t GetUID () { return uID; }

/** Gets and sets the maximum number of windows in this group */
    SI4 ProcessTree::GetMaxWindows () { return maxWindows; }

    void ProcessTree::SetMaxWindows (uint32_t value)
    {
        if (value < 0) return;
        maxWindows = value;
    }

    /**  */
    SI4 Select (SI4 windowNumber);

    SI4 GetNumWindows{ return numWindows; }

    /** Adds a newWindow to the group. */
    void AddWindow (Window newWindow);
    /** Returns a string representation of this object. */
    _::Printer& Print (_::Printer& print);

    private:

    uint32_t maxWindows;

};

}       //< namespace _
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_APP_GROUP
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
    