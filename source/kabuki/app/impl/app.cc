/** Kabuki Theater
    @file       /.../Source/Kabuki_SDK-Impl/_App/App.h
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

#include "App.h"

namespace _App {
    
App::App (int initWidth, int initHeight)
{
    if (initWidth < MinWidthOrHeight)   initWidth = MinWidthOrHeight;
    else if (initWidth > MaxWidth)      initWidth = MaxWidth;

    if (initHeight < MinWidthOrHeight)  initHeight = MinWidthOrHeight;
    else if (initHeight > MaxWidth)     initHeight = MaxWidth;
}

long App::getUID { return uid; }

int App::activateWindow (int newWindowIndex)
{
    return windows.select (newWindowIndex);
}

int App::activateWindow (Window& newWindow)
{
    if (newWindow == nullptr)
        return -1;

    activeWindow = newWindow;

    return 0;
}

WindowGroup& getWindows { return windows; }

int App::show  ()
{
    return 0;
}

int App::hide  ()
{
    return 0;
}

int App::close ()
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
