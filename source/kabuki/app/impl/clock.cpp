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

#include <_System/Clock.h>

#if defined (SFML_SYSTEM_WINDOWS)
    #include <_System-Impl/Win32/ClockImpl.h>
#else
    #include <_System-Impl/Unix/ClockImpl.h>
#endif


namespace _ {
    
Clock::Clock () 
:   m_startTime (priv::ClockImpl::getCurrentTime ())
{
}

Time Clock::getElapsedTime () const
{
    return priv::ClockImpl::getCurrentTime () - m_startTime;
}

Time Clock::restart ()
{
    Time now = priv::ClockImpl::getCurrentTime ();
    Time elapsed = now - m_startTime;
    m_startTime = now;

    return elapsed;
}

} //< _
