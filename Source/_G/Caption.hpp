/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Caption.hpp
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

#include <stdint.hpp>
#include <string>
using namespace std;

#include "Cell.hpp"
#include "Entity.hpp"

namespace _G {

class _KabukiTheater_ Caption : public Entity_f
/*<  */
{
    public:
    
    Caption ();
    /*<  */

    Caption (int captOriginX, int captOriginY);

    void setText (string newString);

    void setVerticalMargin (uint32_t newTBMargen);

    void setHorizontalMargin (uint32_t newLRMargin);

    void update ();

    void draw (Cell* c);
    
    void print (Terminal& io);
    /*< Prints this object to a terminal. */

    private:

    _2D::Point<float> origin;
    TextArea captionText;
    
    int lrMargin, 
        tbMargin;
        
    bool isVisible;
};

}   //< _G
