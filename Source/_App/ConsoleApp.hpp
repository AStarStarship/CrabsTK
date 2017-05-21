/** Kabuki Theater
    @file    /.../Source/Kabuki_SDK-Impl/_App/App.hpp
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

#include "_G/Cell.hpp"

namespace _App {

class ConsoleApp
/** An Kabuki Theater Application.
    
*/
{
    public:

    enum {
        MinWidthOrHeight = 1,
        MaxWidth = 2 * 4096,                //< 2160 x 4096 is 4K Ultra resolution.
        MaxHeight = 2 * 2160,               //< Change multiplier to alter min and max.
    };

    ConsoleApp (const char* Args, int NumArgs);
    /*< Default constructor for a console app. */

    long getUID ();
    /*< Returns the process ID of this App. */

    int activateWindow (int newWindowIndex);
    /*< Sets the activeWindow to the newIndex. */

    int activateWindow (Window& newWindow);
    /*< Sets the activeWindow to the newWindow. */

    WindowGroup getWindows { return windows; }
    /*< Returns the */

    int show  ();
    /*<  */

    int hide  ();
    /*<  */

    int close ();
    /*<  */

    _G::Cell getDrawingContext ();
    /*< Returns a link to this application's drawing context. */

    void draw (_G::Cell& C);
    /*< Draws the graphics on in the Cell canvas g. */

    void redraw ();
    /*< Redraws the screen */

    const char* toString ();
    /*< Returns a text representation of this object. */
    
    private:

    int uid;                 //< The UID of this appliction.

    AppGroup windows;     //< This windows 
};
}
