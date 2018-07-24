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

#include "config.h"

namespace kabuki { namespace tek {

/** Converts gallows to milliliters. */
uint32_t ConvertGallonsToMilliiters  (float value);

/** Converts milliliters to gallons. */
float ConvertMilliitersToGallons  (uint32_t value);

class FlowController {
    public:

    /** Constructs a smart waterer. */
    FlowController (PinName sensor_pin, PinName solenoid_pin, PinName pot_pin,
                    uint32_t max_flow_ml);

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

    /** Script operations. */
    const _::Operation* Star (char_t index, _::Expr* expr);

    private:

    InterruptIn sensor_;         //< Flow sensor pin.
    DigitalOut  valve_;          //< Solenoid valve pin.
    AnalogIn    pot_;            //< Potentiometer pin.
    volatile uint16_t count_;    //< Flow sensor pulse count_.
    uint16_t    flow_rate_ml_;   //< Flow rate in milliliters.
    uint32_t    total_flow_ml_,  //< Total flow in mL.
                target_flow_ml_, //< Target flow in mL per cycle.
                max_flow_ml_;    //< Maximum target flow in mL per cycle.
    uint16_t    last_sample_;    //< Last pot value read.
};

class FlowControllerOp : public _::Operand {
    public:

    /** Constructs a smart waterer. */
    FlowControllerOp (FlowController* flow_co);

    /** Script operations. */
    virtual const _::Operation* Star (char_t index, _::Expr* expr);

    private:

    FlowController* flow_co_;  //< The FlowController.
};

}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_FLOW_CONTROLLER_H
