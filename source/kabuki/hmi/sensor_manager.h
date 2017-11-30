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

#ifndef HEADER_FOR_KABUKI_HMI_SENSORMANAGER
#define HEADER_FOR_KABUKI_HMI_SENSORMANAGER

#include "sensor.h"
#include "sensor_impl.h>


namespace kabuki { namespace hmi { namespace priv {
    
/** Global sensor manager. */
class SensorManager {
    NONCOPYABLE (SensorManager)
    public:

    /** Get the global unique instance of the manager.
       
        @return Unique instance of the sensor manager. */
    static SensorManager& GetInstance();

    /** Check if a sensor is available on the underlying platform.
       
        @param sensor Sensor to check.
       
        @return True if the sensor is available, false otherwise. */
    bool IsAvailable(Sensor::Type sensor);

    /** Enable or disable a sensor.
       
        @param sensor  Sensor to modify
        @param enabled Whether it should be enabled or not. */
    void SetEnabled(Sensor::Type sensor, bool enabled);

    /** Check if a sensor is enabled.
       
        @param sensor Sensor to check
       
        @return True if the sensor is enabled, false otherwise. */
    bool IsEnabled(Sensor::Type sensor) const;

    /** Get the current value of a sensor.
       
        @param sensor Sensor to read
       
        @return Current value of the sensor. */
    Vector3f GetValue(Sensor::Type sensor) const;

    /** Update the state of all the sensors. */
    void Update();

    private:

    /** Default constructor. */
    SensorManager();

    /** Destructor. */
    ~SensorManager();

    /** Sensor information and state. */
    struct Item {
        bool       available; //< Is the sensor available on this device?
        bool       enabled;   //< Current enable state of the sensor
        SensorImpl sensor;    //< Sensor implementation
        Vector3f   value;     //< The current sensor value
    };

    Item m_sensors[Sensor::Count]; //< Sensors information and state
};

}       // namespace priv
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_SENSORMANAGER
