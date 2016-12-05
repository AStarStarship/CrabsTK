/** Kabuki Toolkit
    @file       /.../Source/Kabuki_SDK-Impl/include/_App/WindowGroup.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

#include "_App/WindowGroup.hpp"

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
    