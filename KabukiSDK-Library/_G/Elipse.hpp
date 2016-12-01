/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Elipse.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt.
*/

#include "Cell.h"
#include "Point.h"

namespace _G {
/**  */
class _G_API Elipse
{
    public:
    
    /** Constructor. */
    Elipse (_2D::Point_i relativePosition, _2D::Point_i dimensions);
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
};
}
