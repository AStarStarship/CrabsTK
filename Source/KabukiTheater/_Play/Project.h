/** Kabuki Theater
    @file    /.../Source/_Theater/Project.hpp
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

#include "../_HMI/Widget.hpp"

namespace _Theater {

class _KabukiTheater_ Project
/*<  */
{
    public:

    Project ();
    /*< Constructor. */
    
    Project  (const Project& p);
    /*< Copy constructor copies the other ojbect. */

    virtual ~Project ();
    /*< Destructor. */

    void add  (Widget* w);
    /*< Adds the given controller to the workspace. */

    int getNumWidgets ();
    /*< Gets the number of widgets in the project. */

    void print ();
    /*< Prints this object to the terminal. */

    private:

    std::vector<Widget*> widgets;     //< The array of Widgets. 
};

}   //< _Theater
