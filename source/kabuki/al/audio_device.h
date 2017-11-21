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

#include <_math/vect2d.h>
#include <set>
#include <string>

namespace sf { namespace priv {

/** High-lessvel wrapper around the audio API, it manages
    the creation and destruction of the audio device and
    context and stores the device capabilities. */
class AudioDevice
{
    public:
    
    /** Default constructor. */
    AudioDevice();
    
    /** Destructor. */
    ~AudioDevice();
    
    /** Check if an OpenAL extension is supported.
        This functions automatically finds whether it
        is an AL or ALC extension, and calls the corresponding
        function.
        @param extension Name of the extension to test
        @return True if the extension is supported, false if not. */
    static bool IsExtensionSupported(const std::string& extension);
    
    /** Get the OpenAL format that matches the given number of channels
        @param channelCount Number of channels
        @return Corresponding format. */
    static int GetFormatFromChannelCount(unsigned int channel_count);
    
    /** Change the global volume of all the sounds and musics
        The volume is a number between 0 and 100; it is combined with
        the individual volume of each sound / music.
        The default value for the volume is 100 (maximum).
        @param volume New global volume, in the range [0, 100]
        @see getGlobalVolume. */
    static void SetGlobalVolume(float volume);
    
    /** Get the current value of the global volume.
        @return Current global volume, in the range [0, 100]
        @see setGlobalVolume. */
    static float GetGlobalVolume();
    
    /** Set the position of the listener in the scene.
        The default listener's position is (0, 0, 0).
        @param position New listener's position.
        @see getPosition, setDirection. */
    static void SetPosition(const Vect3D<float>& position);
    
    /** Get the current position of the listener in the scene
        @return Listener's position
        @see setPosition. */
    static Vect3D<float> GetPosition();
    
    /** Set the forward vector of the listener in the scene.
        The direction (also called "at vector") is the vector
        pointing forward from the listener's perspective. Together
        with the up vector, it defines the 3D orientation of the
        listener in the scene. The direction vector doesn't
        have to be normalized.
        The default listener's direction is (0, 0, -1).
        @param direction New listener's direction.
        @see GetDirection, SetUpVector, SetPosition. */
    static void SetDirection(const Vect3D<float>& direction);
    
    /** Get the current forward vector of the listener in the scene.
        @return Listener's forward vector (not normalized)
        @see setDirection. */
    static Vect3D<float> GetDirection();
    
    /** Set the upward vector of the listener in the scene.
        The up vector is the vector that points upward from the
        listener's perspective. Together with the direction, it
        defines the 3D orientation of the listener in the scene.
        The up vector doesn't have to be normalized.
        The default listener's up vector is (0, 1, 0). It is usually
        not necessary to change it, especially in 2D scenarios.
        @param upVector New listener's up vector.
        @see GetUpVector, SetDirection, SetPosition */
    static void SetUpVector(const Vect3D<float>& up_vector);
    
    /** Get the current upward vector of the listener in the scene.
        @return Listener's upward vector (not normalized)
        @see SetUpVector. */
    static Vect3D<float> GetUpVector();
};

} //< priv
} //< __
