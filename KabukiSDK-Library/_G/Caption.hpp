/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Caption.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#include <stdint.h>
#include <string>
using namespace std;

#include "Cell.h"
#include "Entity_f.h"

namespace _G {
/**  */
class _G_API Caption : public Entity_f
{
    public:
    
    /**  */
    Caption ();

    Caption (int captOriginX, int captOriginY);

    void SetText (std::string newString);

    void SetVerticalMargin (uint32_t newTBMargen);

    void SetHorizontalMargin (uint32_t newLRMargin);

    void Update ();

    void Draw (Cell* C);
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */

    private:

    _2D.Point_f origin;
    TextArea captionText;
    
    int lrMargin, 
        tbMargin;
        
    bool isVisible;
};
}
