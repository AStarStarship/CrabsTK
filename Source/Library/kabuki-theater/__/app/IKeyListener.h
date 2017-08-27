/** Kabuki Theater
    @file    /.../Source/__/IKeyListener.hpp
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

namespace __ {

class IKeyListener
/*< Interface with an event.
    
    @code
    #include <_/IEvent.hpp>
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
{
    public:
    
    virtual void pressKey (KeyEvent e) = 0;
    /*< Event handler for when key is pressed. */
    
    virtual void releaseKey (KeyEvent e) = 0;
    /*< Event handler for when key is pressed. */
    
    virtual void typeKey (KeyEvent e) = 0;
    /*< Event handler for when key is typed. */

    virtual void print (Terminal& io) = 0;
    /*< Prints this object to the terminal. */
};

}   //<  __
