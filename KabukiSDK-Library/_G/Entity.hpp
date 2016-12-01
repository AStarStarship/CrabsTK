/** Kabuki SDK
    @file       /.../Source/Kabuki_SDK-Impl/_G/Entity.h
    @author     Cale McCollough
    @copyright  CopYright 2016 Cale McCollough ©
    @license    Read accompanying /.../README.md or online at http://www.boost.org/LICENSE_1_0.txt
    @brief      This file contains the _2D.Vector_f interface.
*/

#include "_2D/Rect_i.h"
#include "Cell.h"

namespace _G {

class _G_API Entity_f : public Cell
/**  */
{
    public:
    
    Entity_f (const char* Name, float X = 0.0f, float Y = 0.0f, float Width = 0.0f, float Height = 0.0f);
    /*< Constructs an entity from the given bounds and name. */

    /** Returns the bounds of the entitY. */
    _2D::Rect_i Bounds () = 0;

    const char* GetName ();
    /*< Gets the name of the string. */

    void SetName  (const char* Name);
    /*< Sets the name of the string. */

    Rect_f& GetBounds ();
    /*< Returns the rectuangular counts of the entity. */

    /**  */
    virtual void Draw (const Cell& C) = 0;
};
}
