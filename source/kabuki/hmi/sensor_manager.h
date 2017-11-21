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

#include <_hmi/Sensor.h>
#include <_hmi/SensorImpl.h>
#include <_/NonCopyable.h>


namespace _hmi { namespace priv {
    
class SensorManager : NonCopyable
/*< Global sensor manager. */
{
    public:

    static SensorManager& getInstance();
    /*< Get the global unique instance of the manager.
       
        @return Unique instance of the sensor manager. */

    bool isAvailable(Sensor::Type sensor);
    /*< Check if a sensor is available on the underlying platform.
       
        @param sensor Sensor to check.
       
        @return True if the sensor is available, false otherwise. */

    void setEnabled(Sensor::Type sensor, bool enabled);
    /*< Enable or disable a sensor.
       
        @param sensor  Sensor to modify
        @param enabled Whether it should be enabled or not. */

    bool isEnabled(Sensor::Type sensor) const;
    /*< Check if a sensor is enabled.
       
        @param sensor Sensor to check
       
        @return True if the sensor is enabled, false otherwise. */

    Vector3f getValue(Sensor::Type sensor) const;
    /*< Get the current value of a sensor.
       
        @param sensor Sensor to read
       
        @return Current value of the sensor. */

    void update();
    /*< Update the state of all the sensors. */

    private:

    SensorManager();
    /*< Default constructor. */

    ~SensorManager();
    /*< Destructor. */

    struct Item
    /*< Sensor information and state. */
    {
        bool available;    ///< Is the sensor available on this device?
        bool enabled;      ///< Current enable state of the sensor
        SensorImpl sensor; ///< Sensor implementation
        Vector3f value;    ///< The current sensor value
    };

    Item m_sensors[Sensor::Count]; ///< Sensors information and state
};

} // namespace priv
}   //< _hmi
