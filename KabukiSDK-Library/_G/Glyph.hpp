/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK/_G/Glyph.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D::Vector_f interface.
*/

#include <string>
using namespace std;

namespace _G {
/**  */
class _KabukiSDK Gylph
{
	    public:
	
	// Returns the unique id of this symbol.
	long GetUID ();

	/** Returns the key label of this object.
	A key label is a unique search key string with no whitespace that can be used to identify this object in a search.
	*/
	string GetKey ();

	//
	string GetDescription ();

	//< Prints this object to the console.
	void Print ();

	// Returns a text representation of this object.
	string ToString ();
	
	    private::

	long uID;       //< The UID of this object.
	string key,     //< The label for the Key of this symbol.
		description;        //< The description of this symbol.
};
}
