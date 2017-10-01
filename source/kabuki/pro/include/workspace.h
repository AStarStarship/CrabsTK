/** Kabuki Theater
    @file    /.../Source/_play/project.h
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
 
#ifndef KABUKI_THEATER__PLAY_PROJECT_H
#define KABUKI_THEATER__PLAY_PROJECT_H

#include "../_hmi/widget.h"

namespace _play {

/**  */
class _KabukiTheater_ Project
{
    public:

    /** Constructor. */
    Project ();
    
    /** Copy constructor copies the other ojbect. */
    Project  (const Project& p);

    /** Destructor. */
    virtual ~Project ();

    /** Adds the given controller to the workspace. */
    void add  (Widget* w);

    /** Gets the number of widgets in the project. */
    int GetNumWidgets ();

    /** Prints this object to the terminal. */
    void Print ();

    private:

    std::vector<Widget*> widgets;     //< The array of Widgets. 
};

}       //< namespace _play
#endif  //< KABUKI_THEATER__PLAY_PROJECT_H
