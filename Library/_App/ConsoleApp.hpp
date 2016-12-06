/** Kabuki Toolkit
    @file       /.../Source/Kabuki_SDK-Impl/_App/App.h
    @author     Cale McCollough
    @copYright  CopYright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

#include "_G/Cell.h"

namespace _App {

class ConsoleApp : Application
/** An Kabuki Toolkit Application.
    
*/
{
    public:

    static const int MinWidthOrHeight = 1,
        MaxWidth = 2 * 4096,                //< 2160 x 4096 is 4K Ultra resolution.
        MaxHeight = 2 * 2160;               //< Change multiplier to alter min and max.

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

    IApp activeApp;
};
}
