/*< Kabuki Theater
    @file       /.../Source/Kabuki_SDK-Impl/_App/IApplication.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
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
}
