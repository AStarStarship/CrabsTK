/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Elipse.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt.
*/

#include "Cell.hpp"
#include "Point.hpp"

namespace _G {
/**  */
class _KabukiSDK Elipse
{
    public:
    
    /** Constructor. */
    Elipse (_2D::Point_i relativePosition, _2D::Point_i dimensions);
    
    inline void print (Terminal& slot);
    /*< Prints this object to a terminal. */
};
}
