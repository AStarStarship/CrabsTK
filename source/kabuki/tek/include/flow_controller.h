/** Kabuki Tek
    @file    /.../Source/tek/sensuators/flow_control.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2017 [Cale McCollough](calemccollough.github.io)

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once


#include <tek/config.hpp>

#include <mbed.hpp>
#include <stdint.hpp>

namespace tek { namespace sensuators {

/** Converts gallows to millileters. */
uint32_t ConvertGallonsToMilliiters  (float Value);

/** Converts millileters to gallons. */
float ConvertMilliitersToGallons  (uint32_t Value);

class FlowController
{
    public:
    
    /** Constructs a smart waterer. */
    SmartSolenoidValve  (PinName sensorPin, PinName solenoidPin, AnalogIn potPin,
        uint32_t maxFlow_mL);
        
    /** StartWaterings to the begining of the watering cycle. */
    void StartWatering  (int index);
    
    /** Increments theflow rate sensor pulse counter. */
    void PulseFlowSensor ();
    
    /** Prints the state of object to the debug stream. */
    void Print  (int index);
    
    /** Prints the state of object when done watering to the debug stream. */
    inline void StopWatering  (int index);
    
    /** Polls the pot and updates the target flow. */
    void UpdateTargetFlow ();
    
    /** Updates the float rate. */
    void Update  (int index);
    
    /** Opens the solenoid valve. */
    void OpenValve ();
    
    /** Closes the solenoid valve. */
    void CloseValve ();
    
    private:
    
    InterruptIn sensor;         //< The flow sensor pin.
    DigitalOut  valve;          //< The solenoid valve pin.
    AnalogIn pot;               //< The potentiometer pin.

    volatile uint16_t count;    //< Flow sensor pulse count.
    
    uint16_t flowRate_mL;       //< The floaw rate in milliliters.
    
    uint32_t totalFlow_mL,      //< The total flow in mL.
        targetFlow_mL,          //< The target flow in mL per cycle.
        maxFlow_mL;             //< The maximum target flow in mL per cycle.
    
    uint16_t lastSample;        //< The last pot value read.
};
}   //< namespace sensuators
}   //< namespace tek
