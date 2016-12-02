/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Scene.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include <string>

#include "LaYer.hpp"
#include "Entity.hpp"

namespace _G {

/**  */
class _KabukiSDK Scene : public Layer
{
    public:
	
	Scene ();

	Scene (const string& Description);
	
	void Delete ();

	void Draw (_G::Cell& C);
	
    private:

	Layer* head;

	void DeconstructScene (Layer* L);
    
    void Print (Terminal& Slot);
    /*< Prints this object to a terminal. */
};
}   //< namespace _G
