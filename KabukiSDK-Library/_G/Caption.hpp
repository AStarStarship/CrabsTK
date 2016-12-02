/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Caption.h
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
/**  */
class _KabukiSDK Caption : public Entity_f
{
    public:
    
    /**  */
    Caption ();

    Caption (int captOriginX, int captOriginY);

    void SetText (string newString);

    void SetVerticalMargin (uint32_t newTBMargen);

    void SetHorizontalMargin (uint32_t newLRMargin);

    void Update ();

    void Draw (Cell* C);
    
    void Print (Terminal& Slot);
    /*< Prints this object to a terminal. */

    private:

    _2D::Point_f origin;
    TextArea captionText;
    
    int lrMargin, 
        tbMargin;
        
    bool isVisible;
};
}
