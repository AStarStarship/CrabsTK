/** Kabuki Theater
    @file    /.../Source/kabuki-theater/app/key_listener.h
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

#ifndef KABUKI_THEATER__KEY_LISTENER_H
#define KABUKI_THEATER__KEY_LISTENER_H

#include <KabukiTheater-Config.h>

namespace app {

/** Interface with an event.
    
    @code
    #include <_/IEvent.h>
    struct Example: public IEvent
    {
        virtual void trigger ()
        {
            
        }
        
        virtual print (Terminal)
        {
            
        }
    };
    @endcode
*/
class KeyListener {
    public:
    
    /** Event handler for when key is pressed. */
    virtual void PressKey (KeyEvent e) = 0;
    
    /** Event handler for when key is pressed. */
    virtual void ReleaseKey (KeyEvent e) = 0;
    
    /** Event handler for when key is typed. */
    virtual void TypeKey (KeyEvent e) = 0;

    /** Prints this object to the terminal. */
    virtual void Print () = 0;
};      //< class KeyListener
}       //< namespace app
#endif  //< KABUKI_THEATER__KEY_LISTENER_H
