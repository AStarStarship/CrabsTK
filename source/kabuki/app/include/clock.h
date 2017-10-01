////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2017 Laurent Gomila (laurent@sfml-dev.org)
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

#ifndef SFML_CLOCK_H
#define SFML_CLOCK_H

#include <config.h>
#include "time.h"


namespace _ {
    
/*< Utility class that measures the elapsed time.
    @ingroup system
   
    _::Clock is a lightweight class for measuring time.
   
    Its provides the most precise time that the underlying
    OS can achieve (generally microseconds or nanoseconds).
    It also ensures monotonicity, which means that the returned
    time can never go backward, even if the system time is
    changed.
   
    Usage example:
    @code
    _::Clock clock;
    ...
    Time time1 = clock.getElapsedTime ();
    ...
    Time time2 = clock.restart ();
    @endcode
   
    The _::Time value returned by the clock can then be
    converted to a number of seconds, milliseconds or even
    microseconds.
   
    @see _::Time 
*/
class Clock {
    public:
    
    /** Default constructor
    
     The clock starts automatically after being constructed. */
    Clock ();
     
    /** Get the elapsed time

        This function returns the time elapsed since the last call
        to restart () (or the construction of the instance if restart ()
        has not been called).

        @return Time elapsed
     */
    Time getElapsedTime () const;

    /** Restart the clock

        This function puts the time counter back to zero.
        It also returns the time elapsed since the clock was started.

        @return Time elapsed */
    Time restart ();

    private:

    Time m_startTime; //< Time of last reset, in microseconds
};

} //< _
#endif // SFML_CLOCK_H
