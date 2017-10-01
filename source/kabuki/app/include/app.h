/** Kabuki Theater
    @file       ~/Source/kabauki_theater/app/app.h
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

#ifndef KABUKI_THEATER__APP_H
#define KABUKI_THEATER__APP_H

#include <config.h>

namespace app {

/** Interface for an Kabuki Theater app.
*/
struct App {
    /*
    const int MaxWindows = 256;
        
    long UID;
    numWindows;
        
    Window windows[];
    Window activeWindow;
    */
    
    /** Initializer. */
    virtual void Initialize () = 0;
        
    /** Returns the process ID of this App. */
    virtual long GetUID () = 0;

    /** Sets the activeWindow to the new index. */
    virtual int ActivateWindow (int i) = 0;
        
    /** Sets the activeWindow to the new window. */
    virtual int ActivateWindow (Window w) = 0;

    /**  */
    virtual WindowGroup GetWindows () = 0;

    /**  */
    virtual int Show () = 0;
        
    /**  */
    virtual int Hide () = 0;
        
    /**  */
    virtual int Close () = 0;

    /** Returns a link to this application's drawing context. */
    virtual _g::Cell& GetDrawCell () = 0;
        
    /**  */
    virtual void Draw (_g::Cell& c) = 0;
        
    /**  */
    virtual void Redraw () = 0;
        
    /** Prints this object to a Terminal.  */
    virtual void Print () = 0;
};      //< struct App
}       //< namespace app
#endif  //< KABUKI_THEATER__APP_H
