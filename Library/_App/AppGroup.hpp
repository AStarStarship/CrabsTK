/*< Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/include/_App/WindowGroup.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

#include <vector>

#include "Window.hpp"

namespace _App {

class WindowGroup
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