/*< Kabuki Theater
    @file       /.../Source/Kabuki_SDK-Impl/_App/IApplication.hpp
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

#pragma once

#include <KabukiTheater-Config.hpp>

namespace _App {

class IApplication
/*< Interface for an Kabuki Theater app.
*/
{
    public:
        
    /*<
    const int MaxWindows = 256;
        
    long UID;
    numWindows;
        
    Window windows[];
    Window activeWindow;
    */
    
    virtual void initialize () = 0;
    /*< Initializer. */
        
    virtual long getUID () = 0;
    /*< Returns the process ID of this App. */

    virtual int activateWindow (int i) = 0;
    /*< Sets the activeWindow to the new index. */
        
    virtual int activateWindow (Window w) = 0;
    /*< Sets the activeWindow to the new window. */

    virtual WindowGroup getWindows () = 0;
    /*<  */

    virtual int show () = 0;
    /*<  */
        
    virtual int hide () = 0;
    /*<  */
        
    virtual int close () = 0;
    /*<  */

    virtual _G::Canvas& getCanvas () = 0;
    /*< Returns a link to this application's drawing context. */
        
    virtual void draw (_G::Canvas& c) = 0;
    /*<  */
        
    virtual void redraw () = 0;
    /*<  */
        
    virtual void print (Terminal& io) = 0;
    /*< Prints this object to a Terminal.  */
};
}   //< _App
