/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/app.h
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
    
App::App (SI4 initWidth, SI4 initHeight)
{
    if (initWidth < MinWidthOrHeight)   initWidth = MinWidthOrHeight;
    else if (initWidth > MaxWidth)      initWidth = MaxWidth;

    if (initHeight < MinWidthOrHeight)  initHeight = MinWidthOrHeight;
    else if (initHeight > MaxWidth)     initHeight = MaxWidth;
}

uint32_t App::getUID { return uid; }

SI4 App::activateWindow (SI4 newWindowIndex)
{
    return windows.select (newWindowIndex);
}

SI4 App::activateWindow (Window& newWindow)
{
    if (newWindow == nullptr)
        return -1;

    activeWindow = newWindow;

    return 0;
}

ProcessTree& getWindows { return windows; }

SI4 App::show  ()
{
    return 0;
}

SI4 App::hide  ()
{
    return 0;
}

SI4 App::close ()
{
    return 0;
}

_G.::Cell& App::getCell ()
{
    return nullptr;
}

void App::draw (_G::Cell& c)
{

}

void App::redraw ()
{

}

void App::print ()
{
    printf ("");
}

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
