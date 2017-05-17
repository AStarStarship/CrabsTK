/** Kabuki Theater
    @file       /.../Source/Kabuki_SDK-Impl/_App/App.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

#include "App.hpp"

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

string App::toString ()
{
    return "";
}
