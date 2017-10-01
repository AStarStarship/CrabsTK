/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_G/Scene.h
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

#include <string>

#include "Cell.h"
#include "Entity.h"

namespace _G {

class _KabukiTheater_ Scene : public Cell
/** A graphical scene.
*/
{
    public:
	
	Scene ();

	Scene (const string& Description);
	
	void destroy ();

	void draw (Cell& C);
	
    private:

	Layer* head;

	void destroyScene (Cell* l);
    /*< Deconstructs the scene. */
    
    void print () const;
    /*< Prints this object to a terminal. */
};
}   //  _G
