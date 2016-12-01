/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Layer.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include "Entity.h"

namespace _G {

/**  */
struct Layer
{
    Entity& Element;   	//< The element of this layer.

    Layer* Prev,      	//< The previous layer in the pipeline.
        * Next;         //< The next layer in the pipeline.
	
    void Remove ();
    /*<  */

    void Draw (const Cell& C);
    /*<  */
};
}
