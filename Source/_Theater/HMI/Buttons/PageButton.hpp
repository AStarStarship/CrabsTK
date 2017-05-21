/** Kabuki Theater
    @file    /.../Source/_Theater/HMI/Buttons/PageButton.hpp
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


#include <KabukiTheater-Config.hpp>
#include "Button.hpp"
#include "TemplatePage.hpp"
#include "ControlLayer.hpp"

namespace _Theater { namespace _HMI { namespace Buttons {

/* @class _KabukiTheater_   PageButton
   @brief   A type of Button that loads a TemplatePage into a Control Layer
   @details The difference between a page and a macro button is that a page button
            swtiches between pages in a template and a PageButton will be able to perform other tasks
            in a virtual instrument such as loading one of the sub-menus. Not sure if this class _KabukiTheater_ is
            neccissary yet. */


class _KabukiTheater_ PageButton: public Button
{
      public:

    PageButton  (const TemplatePage &thePage=TemplatePage ());
    //< Constructor.
    PageButton  (const PageButton& objToCopy);
    //< Copy constructor.

   ~PageButton ();
   //< Destructor.
   
    void Press  (const ControlLayer &parentLayer);
    //< The action  (s) performed when this button gets pressed.

    void Depress  (const ControlLayer &parentLayer);
    //< The action  (s) performed when this button gets double pressed.

    void Depress  (const ControlLayer &parentLayer);
    //< The action  (s) performed when this button gets depressed.
    
    TemplatePage* GetPage ();
    //< Gets th TemplatePage that this 

    void SetPage  (TemplatePage* newPage);
    //< Sets thisPage to the newPage.
    
    void print (I2P::Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    TemplatePage* thisPage;     //< The page to load.
};
}   //< namespace Buttons
}   //< namespace _Theater { namespace _HMI

