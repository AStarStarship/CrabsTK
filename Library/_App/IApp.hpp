/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_App/IApplication.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
*/

namespace _App
{
    /** Interface for an Kabuki SDK app. */
    class IApplication
    {
        public:
        
        /**
        const int MaxWindows = 256;
        
        long UID;
        numWindows;
        
        Window windows[];
        Window activeWindow;
        */
        
        /** Initializer. */
        void Initialize () = 0;
        
        /** Returns the process ID of this App. */
        long GetUID () = 0;

        /** Sets the activeWindow to the newIndex. */
        int ActivateWindow (int newIndex) = 0;
        
        /** Sets the activeWindow to the newWindow. */
        int ActivateWindow (Window newWindow) = 0;

        WindowGroup Windows () = 0;

        int Show () = 0;
        int Hide () = 0;
        int Close () = 0;

        /** Returns a link to this application's drawing context. */
        _G2D Cell ( = 0);
        void Draw (_G2D.Cell& C) = 0;
        void Redraw () = 0;
        
        /** Returns a text representation of this object. */
        string ToString () = 0;
    };
}
