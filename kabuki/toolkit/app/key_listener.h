/** Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/key_listener.h
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
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef KABUKI_THEATER__KEY_LISTENER_H
#define KABUKI_THEATER__KEY_LISTENER_H

#include "config.h"

namespace _ {

/** Interface with an event.
    
    @code
    #include <_/IEvent.h>
    struct Example: public IEvent
    {
        virtual void trigger ()
        {
            
        }
        
        virtual print (Expression)
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

}       //< namespace _
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_APP_KEYLISTENER
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
