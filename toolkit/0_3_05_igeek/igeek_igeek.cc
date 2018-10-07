/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/Cell_IGA.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

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

#include <Kabuki_Theater/Cell_IGA/Cell_IGA.h>

namespace Kabuki_Theater { namespace Cell_IGA {

Cell_IGA::Cell_IGA ()
{
    add (new LeukocyteWar ());
}

static void Cell_IGA::main (String[] args)
{
    JFrame window = new JFrame ("Kabuki.Cell_IGA");
    window.setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
    
    JScrollPane scrollableArea = new JScrollPane (new LeukocyteWar ());
    
    window.setContentPane (scrollableArea);
    
    window.pack ();
    window.setVisible (true);
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
