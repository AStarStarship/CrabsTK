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

#pragma once

#include <KabukiTheater-Config.h>
#include <_hmi/Joystick.h>
#include <_/String.h>
#include <algorithm>


namespace _hmi { namespace priv {

struct JoystickCaps
/*< Structure holding a joystick's information. */
{
    JoystickCaps()
    {
        buttonCount = 0;
        std::fill(axes, axes + Joystick::AxisCount, false);
    }

    uint buttonCount;               ///< Number of buttons supported by the joystick
    bool         axes[Joystick::AxisCount]; ///< Support for each axis
};

struct JoystickState
/*< Structure holding a joystick's state. */
{
    JoystickState()
    {
        connected = false;
        std::fill(axes, axes + Joystick::AxisCount, 0.f);
        std::fill(buttons, buttons + Joystick::ButtonCount, false);
    }

    bool  connected;                      ///< Is the joystick currently connected?
    float axes[Joystick::AxisCount];      ///< Position of each axis, in range [-100, 100]
    bool  buttons[Joystick::ButtonCount]; ///< Status of each button (true = pressed)
};

} // namespace priv
}   //< _hmi


#if defined(SFML_SYSTEM_WINDOWS)

    #include <_hmi/Win32/JoystickImpl.h>

#elif defined(SFML_SYSTEM_LINUX)

    #include <_hmi/Unix/JoystickImpl.h>

#elif defined(SFML_SYSTEM_FREEBSD)

    #include <_hmi/FreeBSD/JoystickImpl.h>

#elif defined(SFML_SYSTEM_MACOS)

    #include <_hmi/OSX/JoystickImpl.h>

#elif defined(SFML_SYSTEM_IOS)

    #include <_hmi/iOS/JoystickImpl.h>

#elif defined(SFML_SYSTEM_ANDROID)

    #include <_hmi/Android/JoystickImpl.h>

#endif
