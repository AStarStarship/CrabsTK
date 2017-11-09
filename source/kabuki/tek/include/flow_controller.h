/** kabuki::tek
    @file    ~/source/kabuki/tek/include/flow_controller.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/
 
#ifndef KABUKI_TEK_FLOW_CONTROLLER_H
#define KABUKI_TEK_FLOW_CONTROLLER_H

#include "module_config.h"

namespace kabuki { namespace tek {

/** Converts gallows to milliliters. */
uint32_t ConvertGallonsToMilliiters  (float Value);

/** Converts milliliters to gallons. */
float ConvertMilliitersToGallons  (uint32_t Value);

class FlowController {
    public:

    /** Constructs a smart waterer. */
    FlowController (PinName sensorPin, PinName solenoidPin, AnalogIn potPin,
                        uint32_t maxFlow_mL);

    /** StartWaterings to the beginning of the watering cycle. */
    void StartWatering (int index);

    /** Increments the flow rate sensor pulse counter. */
    void PulseFlowSensor ();

    /** Prints the state of object to the debug stream. */
    void Print (int index);

    /** Prints the state of object when done watering to the debug stream. */
    inline void StopWatering (int index);

    /** Polls the pot and updates the target flow. */
    void UpdateTargetFlow ();

    /** Updates the float rate. */
    void Update (int index);

    /** Opens the solenoid valve. */
    void OpenValve ();

    /** Closes the solenoid valve. */
    void CloseValve ();

    private:

    InterruptIn sensor;         //< Flow sensor pin.
    DigitalOut  valve;          //< Solenoid valve pin.
    AnalogIn    pot;            //< Potentiometer pin.
    volatile uint16_t count;    //< Flow sensor pulse count.
    uint16_t    flow_rate_ml;   //< Flow rate in milliliters.
    uint32_t    total_flow_ml,  //< Total flow in mL.
                target_flow_ml, //< Target flow in mL per cycle.
                maxFlow_mL;     //< Maximum target flow in mL per cycle.
    uint16_t    lastSample;     //< Last pot value read.
};

}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_FLOW_CONTROLLER_H
