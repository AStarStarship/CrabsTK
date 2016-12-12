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

#ifndef SFML_THREADIMPL_HPP
#define SFML_THREADIMPL_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>
#include <_/NonCopyable.hpp>
#include <pthread.h>


namespace _
{
class Thread;

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Unix implementation of threads
////////////////////////////////////////////////////////////
class ThreadImpl : NonCopyable
{
    public:
    
    ThreadImpl(Thread* owner);
    /*< Default constructor, launch the thread
    ///
    /// @param owner The Thread instance to run */

    void wait();
    /*< Wait until the thread finishes. */
    
    void terminate();
    /*< Terminate the thread. */

    private:
    
    static void* entryPoint(void* userData);
    /*< Global entry point for all threads
    ///
    /// @param userData User-defined data (contains the Thread instance)
    ///
    /// @return Os specific error code. */

    pthread_t m_thread;   ///< pthread thread instance
    bool      m_isActive; ///< Thread state (active or inactive)
};

} // namespace priv

} // namespace _


#endif // SFML_THREADIMPL_HPP
