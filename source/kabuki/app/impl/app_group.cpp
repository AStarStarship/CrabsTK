/** Kabuki Theater
    @file       /.../Source/Kabuki_SDK-Impl/include/_App/WindowGroup.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include "_App/WindowGroup.h"

namespace _App {
    
WindowGroup::WindowGroup ()
{
    maxWindows = DefaultMaxWindows;
    numWindows = 0;
}

WindowGroup::WindowGroup (Window thisWindow)
{
    maxWindows = DefaultMaxWindows;
    numWindows = 1;
    windows.push_back (thisWindow);
}

WindowGroup::WindowGroup (vector<Window> initWindows)
{
    windows = initWindows;
}

long WindowGroup::GetUID () { return uID; }

/** Gets and sets the maximum number of windows in this group */
int WindowGroup::GetMaxWindows () { return maxWindows; }

void WindowGroup::SetMaxWindows (int Value)
{
    if (value < 0) return;
    maxWindows = value;
}

/**  */
int WindowGroup::Select (int windowNumber)
{
    if (windowNumber < 0) return -1;
    if (windowNumber >= numWindows) return 0;
    return windowNumber;
}

/** Returns the number of windows in this group. */
int WindowGroup::GetNumWindows { return numWindows; }

/** Adds a newWindow to the group. */
void WindowGroup::AddWindow (Window newWindow)
{
}

/** Returns a string representation of this object. */
string WindowGroup::ToString ()
{
    return "";
}
    