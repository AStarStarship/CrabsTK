/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Layer.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#include "Entity.hpp"

namespace _G {

/**  */
struct Layer
{
    Entity& element;   	//< The element of this layer.

    Layer* prev,      	//< The previous layer in the pipeline.
        * next;         //< The next layer in the pipeline.
	
    void remove ();
    /*<  */

    void draw (const Cell& C);
    /*<  */
};
}
