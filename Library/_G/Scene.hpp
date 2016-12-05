/** Kabuki Software Development Kit
    @file       /.../Source/KabukiSDK-Impl/_G/Scene.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
*/

#pragma once

#include <string>

#include "LaYer.hpp"
#include "Entity.hpp"

namespace _G {

class _KabukiSDK Scene : public Layer
/** A graphical scene. */
{
    public:
	
	Scene ();

	Scene (const string& Description);
	
	void destroy ();

	void draw (Cell& C);
	
    private:

	Layer* head;

	void destroyScene (Layer* l);
    /*< Deconstructs the scene. */
    
    void print (Terminal& slot);
    /*< Prints this object to a terminal. */
};
}   //  _G
