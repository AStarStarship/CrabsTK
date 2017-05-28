/*< Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_App/WindowGroup.hpp
    @author  Cale McCollough
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 Cale McCollough <https://calemccollough.github.io>

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include <vector>

#include "Window.hpp"

namespace _App {

class _KabukiTheater_ WindowGroup
/*< Class that stores a group of _App.Window objects. *
{
    public:
    
    static const int DefaultMaxWindows = 1024;

    WindowGroup ();
    /*< Default constructor. **/

    WindowGroup (Window& thisWindow);
    /*< Constructor initializes with a single window. */

    WindowGroup (vector<Window> initWindows);
    /*< Constructor initializes with an array of windows. */

    long getUID ();
    /*< Returns the UID of this Group. */

    int getMaxWindows ();
    /*< Gets and sets the maximum number of windows in this group */
    
    void setMaxWindows (int Value);
    /*< Gets and sets the maximum number of windows in this group */

    int select (int windowNumber);
    /*<  */

    int getNumWindows ();
    /*< Returns the number of windows in this group. */

    void addWindow (Window newWindow);
    /*< Adds a newWindow to the group. */

    void print (I2P::Terminal& ioSlot);
    /*< Prints this object to the terminal. */

    private:

    long uID;               //< The UID of this Group.

    int numWindows,         //< The number of windows in this group.
               maxWindows;          //< The maximum number of windows allowed in a group.

    vector<Window> windows;   //< The Window (s) in this group. 
};
}