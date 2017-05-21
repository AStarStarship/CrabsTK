/** Kabuki Toolkit
    @file    /.../KabukiToolkit/_Dev/ControlLayerTests.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <HUD/ControlLayer.hpp>

namespace SymmetryStation { namespace HUD {

class _KabukiTheater_ ControlLayerTests: public UnitTest
{
    public:
    
    ControlLayerTests () : UnitTest ("Testing Controls::ControlLayer") {}

    void runTest ()
    {
        beginTest ("Testing Controls::ControlLayer class _KabukiToolkit\n" + PrintLine<'~'> ());

        ControlLayer layerT ();

        beginTest ("Testing const char* ToString ()");
        LogMessage (layerT.ToString ());

        LogMessage ("Done testing Controls::ControlLayer.");
    }
};
}   //< HUD
}   //< SymmetryStation
