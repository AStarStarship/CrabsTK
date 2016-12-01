/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Scene.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include <string>

#include "LaYer.h"
#include "Entity.h"

namespace _G {

/**  */
class _G_API Scene : public Layer
{
    public:
	
	Scene ();

	Scene (const const char*& Description);
	
	void Delete ();

	void Draw (_G::Cell& C);
	
    private:

	Layer* head;

	void DeconstructScene (Layer* L);
    
    void Print (I2P::Terminal& Slot);
    /*< Prints this object to a terminal. */
};
}   //< namespace _G
