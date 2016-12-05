/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/include/_App/WindowGroup.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

#pragma once

#include <vector>

#include "Window.h"

namespace _App
{
/** Class that stores a group of _App.Window objects. */
class WindowGroup
{
    public:
    
    static const int DefaultMaxWindows = 1024;

    /** Default constructor. **/
    WindowGroup ();

    /** Constructor initializes with a single window. */
    WindowGroup (Window& thisWindow);

    /** Constructor initializes with an array of windows. */
    WindowGroup (vector<Window> initWindows);

    /** Returns the UID of this Group. */
    long GetUID ();

    /** Gets and sets the maximum number of windows in this group */
    int GetMaxWindows ();
    void SetMaxWindows (int Value);

    /**  */
    int Select (int windowNumber);

    /** Returns the number of windows in this group. */
    int GetNumWindows ();

    /** Adds a newWindow to the group. */
    void AddWindow (Window newWindow);

    void Print (I2P::Terminal& Slot);
    /*< Prints this object to the terminal. */

    private:

    long uID;               //< The UID of this Group.

    int numWindows,         //< The number of windows in this group.
               maxWindows;          //< The maximum number of windows allowed in a group.

    vector<Window> windows;   //< The Window (s) in this group. 
};
}