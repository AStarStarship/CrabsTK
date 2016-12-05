/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Caption.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include <stdint.h>
#include <string>
using namespace std;

#include "Cell.hpp"
#include "Entity.hpp"

namespace _G {

class _KabukiSDK Caption : public Entity_f
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
    
    void print (Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    _2D::Point_f origin;
    TextArea captionText;
    
    int lrMargin, 
        tbMargin;
        
    bool isVisible;
};

}   //< _G
