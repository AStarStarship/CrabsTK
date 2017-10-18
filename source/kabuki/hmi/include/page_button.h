/** Kabuki Starship
    @file    ~/Source/_hmi/Buttons/PageButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
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


#include <KabukiTheater-Config.h>
#include "Button.h"
#include "WidgetPage.h"
#include "ControlLayer.h"

namespace _hmi { namespace Buttons {

class _HMI_API PageButton: public Button
/** A type of Button that loads a WidgetPage into a Control Layer
    The difference between a page and a macro button is that a page button
    swtiches between pages in a template and a PageButton will be able to perform other tasks
    in a virtual instrument such as loading one of the sub-menus. Not sure if this class is
    neccissary yet. */
{
      public:

    PageButton  (const WidgetPage &thePage=WidgetPage ());
    //< Constructor.
    PageButton  (const PageButton& objToCopy);
    //< Copy constructor.

   ~PageButton ();
   //< Destructor.
   
    void press  (const ControlLayer &parentLayer);
    //< The action  (s) performed when this button gets pressed.

    void depress  (const ControlLayer &parentLayer);
    //< The action  (s) performed when this button gets double pressed.

    void depress  (const ControlLayer &parentLayer);
    //< The action  (s) performed when this button gets depressed.
    
    WidgetPage* getPage ();
    //< Gets th WidgetPage that this 

    void SetPage  (WidgetPage* newPage);
    //< Sets thisPage to the newPage.
    
    void print () const;
    /*< Prints this object to a terminal. */

    private:

    WidgetPage* thisPage;     //< The page to load.
};
}   //< namespace Buttons
}   //< namespace _hmi

