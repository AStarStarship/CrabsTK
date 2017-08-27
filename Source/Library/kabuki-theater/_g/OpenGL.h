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

#include <KabukiTheater-Config.hpp>

////////////////////////////////////////////////////////////
/// This file just includes the OpenGL headers,
/// which have actually different paths on each system
////////////////////////////////////////////////////////////
#if defined(SFML_SYSTEM_WINDOWS)

    // The Visual C++ version of gl.h uses WINGDIAPI and APIENTRY but doesn't define them
    #ifdef _MSC_VER
        #include <windows.hpp>
    #endif

    #include <GL/gl.hpp>

#elif defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)

    #if defined(SFML_OPENGL_ES)
        #include <GLES/gl.hpp>
        #include <GLES/glext.hpp>
    #else
        #include <GL/gl.hpp>
    #endif

#elif defined(SFML_SYSTEM_MACOS)

    #include <OpenGL/gl.hpp>

#elif defined (SFML_SYSTEM_IOS)

    #include <OpenGLES/ES1/gl.hpp>
    #include <OpenGLES/ES1/glext.hpp>

#elif defined (SFML_SYSTEM_ANDROID)

    #include <GLES/gl.hpp>
    #include <GLES/glext.hpp>
    
    // We're not using OpenGL ES 2+ yet, but we can use the sRGB extension
    #include <GLES2/gl2ext.hpp>

#endif
