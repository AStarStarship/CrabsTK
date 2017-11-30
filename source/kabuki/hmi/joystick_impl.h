////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2016 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef HEADER_FOR_KABUKI_HMI_JOYSTICKIMPL
#define HEADER_FOR_KABUKI_HMI_JOYSTICKIMPL

#include "config.h"
#include "Joystick.h>
#include <_/String.h>
#include <algorithm>


namespace kabuki { namespace hmi { namespace priv {

/** Structure holding a joystick's information. */
struct JoystickCaps
{
    JoystickCaps()
    {
        buttonCount = 0;
        std::fill(axes, axes + Joystick::AxisCount, false);
    }

    uint buttonCount;               //< Number of buttons supported by the joystick
    bool         axes[Joystick::AxisCount]; //< Support for each axis
};

/** Structure holding a joystick's state. */
struct JoystickState
{
    JoystickState()
    {
        connected = false;
        std::fill(axes, axes + Joystick::AxisCount, 0.f);
        std::fill(buttons, buttons + Joystick::ButtonCount, false);
    }

    bool  connected;                      //< Is the joystick currently connected?
    float axes[Joystick::AxisCount];      //< Position of each axis, in range [-100, 100]
    bool  buttons[Joystick::ButtonCount]; //< Status of each button (true = pressed)
};

} // namespace priv
}       //< namespace hmi


#if defined(SFML_SYSTEM_WINDOWS)

    #include "Win32/JoystickImpl.h>

#elif defined(SFML_SYSTEM_LINUX)

    #include "Unix/JoystickImpl.h>

#elif defined(SFML_SYSTEM_FREEBSD)

    #include "FreeBSD/JoystickImpl.h>

#elif defined(SFML_SYSTEM_MACOS)

    #include "OSX/JoystickImpl.h>

#elif defined(SFML_SYSTEM_IOS)

    #include "iOS/JoystickImpl.h>

#elif defined(SFML_SYSTEM_ANDROID)

    #include "Android/JoystickImpl.h>

#endif
#endif  //< HEADER_FOR_KABUKI_HMI_JOYSTICKIMPL
