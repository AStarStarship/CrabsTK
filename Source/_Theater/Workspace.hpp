/** Kabuki Theater
    @file    /.../Source/_Theater/Workspace.hpp
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


#include <KabukiToolkit-Config.hpp>
#include "../_Dev/ControlSurface.hpp"

namespace _Theater {

class _KabukiTheater_ Workspace
/*<  */
{
    public:

    Workspace ();
    /*< Constructor. */
    
    Workspace  (const Workspace& o);
    /*< Copy constructor copies the other ojbect. */

    virtual ~Workspace ();
    /*< Destructor. */

    void add  (Controller* c);
    /*< Adds the given controller to the workspace. */

    int getNumControlSurfaces ();
    /*< Gets the num_control_surfaces. */

    void print (I2P::Terminal& io);
    /*< Prints this object to the terminal. */

    private:

    std::vector<Controller*> controllers;     //< The array of iSymmetric control surfaces. 
};

}   //< namespace _VEnv
