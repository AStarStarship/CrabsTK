/** Kabuki Theater
    @file       /.../Source/KabukiTheater-Impl/_G/Layer.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include "Entity.hpp"

namespace _G {

class Layer
/*<  */
{
    public:

    Entity& element;   	//< The element of this layer.

    Layer* next;        //< The next layer in the pipeline.

    Layer (Entity& e, Layer* nextLayer);
	
    void remove ();
    /*<  */

    void draw (Cell& C);
    /*<  */
};

}   //< _G
