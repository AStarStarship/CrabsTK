/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_flow_controller.cc
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

#include "../include/flying_fader.h"

namespace kabuki {
namespace tek {

uint32_t ConvertGallonsToMilliiters (float Value) {
    return  (uint32_t)(Value * 3785.41f);
}

float ConvertMilliitersToGallons (uint32_t Value) {
    return  (3785.41f / (float)Value);
}

FlowController::FlowController (PinName sensor_pin, PinName solenoid_pin,
                                        AnalogIn pot_pin, uint32_t maxFlow_mL)
    : sensor (SensorPin),
    valve (SolenoidPin),
    pot (PotPin),
    count (0),
    flow_rate_ml (0),
    total_flow_ml (0),
    target_flow_ml (maxFlow_mL / 2),
    maxFlow_mL (maxFlow_mL),
    //flowRate       (0.0f),
    lastSample (pot.read_u16 ()) {
    sensor.rise (this, &FlowController::PulseFlowSensor);
}

void FlowController::StartWatering (int Index) {
    count = flow_rate_ml = total_flow_ml = 0;

    if (target_flow_ml == 0) return;

    OpenValve ();

    PrintLine ();
    Print (Index);
}

void FlowController::Print (int index) {
    printf ("\r\n%i: flow rate =  %u mL/s, total =  %u mL, target =  %u mL\r\n",
            index, flow_rate_ml, total_flow_ml, target_flow_ml);
}

void FlowController::UpdateTargetFlow () {
    uint16_t sample = pot.read_u16 (),
        tempLastSample = lastSample;

    lastSample = sample;

    if ((sample > tempLastSample - 300) && (sample < tempLastSample + 300))
        return;

    uint32_t tempTargetFlow_mL = (sample * maxFlow_mL) / 0xffff;

    target_flow_ml = tempTargetFlow_mL < 100?0:tempTargetFlow_mL;

    if (total_flow_ml >= tempTargetFlow_mL)
        CloseValve ();
    else
        OpenValve ();
}

inline void FlowController::StopWatering (int Index) {
    CloseValve ();
    Print (Index);
    printf ("\r\nDone watering");
    PrintLine ();
}

void FlowController::Update (int Index) {
    UpdateTargetFlow ();

    /// Update the flow sensor pulse count and flow calculations.

    uint16_t localCount = count;

    uint32_t tempFlowRate_mL = (2000 * count) / 540;
    flow_rate_ml = tempFlowRate_mL;
    total_flow_ml += tempFlowRate_mL;

    if (total_flow_ml >= target_flow_ml)
        StopWatering (Index);
    else
        Print (Index);
}

void FlowController::PulseFlowSensor () {
    ++count;
}

void FlowController::OpenValve () {
    valve = 1;
}

void FlowController::CloseValve () {
    valve = 0;
}

const _::Operation* FlowController::Star (char_t index, _::Expression* expr)
{
    static const _::Operation this_op = { "FlowController",
        _::NumOperations (0), _::FirstOperation ('A'),
        "tek::sensors", 0
    };

    switch (index) {
        case '?': return &this_op;
        case 'A':
            static const _::Operation* op_A = {
                "Open", nullptr, nullptr, "Opens the valve." };
            if (!expr) return &op_A;
    }

}

}       //< namespace tek
}       //< namespace kabuki
