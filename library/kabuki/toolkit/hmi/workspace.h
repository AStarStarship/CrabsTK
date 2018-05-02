/** Kabuki Toolkit
    @file    /.../Source/_play/project.h
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

#ifndef HEADER_FOR_KABUKI_TOOLKIT_HMI_PROJECT
#define HEADER_FOR_KABUKI_TOOLKIT_HMI_PROJECT

#include "widget.h"

namespace kt { namespace hmi {

/**  */
class KABUKI Project
{
    public:

    /** Constructor. */
    Project ();
    
    /** Copy constructor copies the other ojbect. */
    Project  (const Project& p);

    /** Destructor. */
    virtual ~Project ();

    /** Adds the given controller to the workspace. */
    void Add  (Widget* w);

    /** Gets the number of widgets in the project. */
    int GetNumWidgets ();

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& print);

    private:

    data::Array<Widget*> widgets; //< The array of Widgets. 
};

}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_HMI_PROJECT
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
