/** Kabuki Tek
    @version 0.x
    @file    /.../Source/KabukiToolkit-Demos/MachinaSpeculatrix.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                        All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License");
        you may not use this file except in compliance with the License.
        You may obtain a copy of the License at
        
                http://www.apache.org/licenses/LICENSE-2.0
        
        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#pragma once

#include <KabukiTek.hpp>
using namespace _;

#include <_Tek/Sensors/Photosensor.hpp>
#include <_Tek/Sensors/Switch.hpp>
#include <_Tek/Actuators/HBridgeMotor.hpp>
using namespace _Tek::Sensors;
using namespace _Tek::Actuators;

class MachinaSpeculatrix: public Room
/** The Machina Speculatrix was the first robot in the world.
    The Machina Speculatrix was invented between 1948-9 by William Grey Walter, 
    and is known as the world first robot. It consists of a light sensor, a 
    touch sensor, and H-bridge motor for front-back movement, and another for 
    left-right steering.
*/
{
  public:

    MachinaSpeculatrix ()
    /** Constructs a Machina Speculatrix. */
    :   lightSensor (),
        touchSensor (),
        motor (),
        steering ()
    {
    }
    
    virtual ~MachinaSpeculatrix () {}
    /*< Virtual destructor. */

    virtual const Member* op (Terminal* io, int index) override
    /** I2P operations. */
    {
        /// There are two search options
        ///if 	io(Query != 0) index = find (this, Query);
        //if (Query != 0) index = find<MachinaSpectulatrix, 5> (this, index);

        switch (index)
        {
			if (!io) return memberDevice <"MachinaSpeculatrix", 5, 
				"The Machina Speculatrix was the first robot in the world. It was invented between 1948-9 by William Grey "
				"Walter. It consists of a light sensor, a touch sensor, and H-bridge motor for front-back movement, and "
				"another for left-right steering.">();
            case 0: Member m0 = { "lightSensor", 0, 0, "A photosensor to measure light." };
                if (!io) return m0;
            	return io->push (lightSensor);

            case 1: if (!io) return member ("touchSensor",
                "A momentary switch on the front of the robot to detect collisions."):
                return io->push (touchSensor);

            case 2: return io ? enquery (io, "motor",
                "The front-back motor."):
                return io->push (motor);

            case 3: return io ? enquery (io, "steering", "The front-back motor."):
                return io->push (motor);

            case 4:
                static const uint_t rxHeader4[] = { 2, SI4, STR, 140 },
                    txHeader4[] = { 1, STR, 140 };

                if (!io) return enquery (io, "ListenTalkExample", rxHeader4, txHeader4,
                    "Example of a multi-parameter function with return value. User inputs an STX and a SV4, "
                    "target machine does some processing on the data and return the result. If the string parameter "
                    "does not match the sprintf formatting, the function will generate an error message and send it "
                    "back. Another thing to note is that we can define a function call in I2P without having to create "
                    "a function in our code. This helps save on typing and debug time.");

                static const char format[] = "\r\n\nHello world! I heard you say \"%s\" and the number \"%i\".\r\n\n\0";

                int32_t numberIn;
                int RxBufferSize;
                char stringIn[140],
                    responce[140];

                if (io->read (rxHeader4, &numberIn, &stringIn)) return io->ReadError ();

                /// Now we know that our data is loaded and good to go.

                sprintf (responce, format, stringIn, numberIn);

                return io->write (txHeader4, &responce);
        }
    }
	
    const char* init () override
    /*< Main program loop does nothing at the moment. */
	{
		
	}
	
  private:
    
    Photosensor lightSensor;        //< A light sensor.
    Switch touchSensor;             //< A touch sensor.
    HBridgeMotor motor,             //< Front-back motor.
        steering;                   //< Left-right motor.
};

