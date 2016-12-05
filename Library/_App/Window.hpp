/** Kabuki Toolkit
    @file       /.../Source/Kabuki_SDK-Impl/_App/Window.h
    @author     Cale McCollough
    @copyright  Copyright 2016 Cale McCollough ©
    @license    http://www.apache.org/licenses/LICENSE-2.0
    @brief      This file contains the _ class.
*/

#include <stdint.h>

#include "_G/Cell.h"

namespace _App
{
/** A Window of an software application. */
class Window
{
    public:

    int Show ();

    int Hide ();

    int Close ();

    void Update ();

    void Draw (_G::Cell& C);

    char GetTransparencyLevel ();
    
    void SetTransparencyLevel (uint8_t Value);

    private:

    uint8_t transparencyLevel;
}
}
