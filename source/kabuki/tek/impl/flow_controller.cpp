/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/Sensuators/auto_solenoid_valve.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough](calemccollough.github.io)

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

#include <tek/Sensuators/flying_fader.h>

namespace tek { namespace Sensuators {

uint32_t ConvertGallonsToMilliiters  (float Value)
{
    return  (uint32_t)  (Value * 3785.41f);
}

float ConvertMilliitersToGallons  (uint32_t Value)
{
    return  (3785.41f /  (float)Value);
}

SmartSolenoidValve::SmartSolenoidValve  (PinName SensorPin, PinName SolenoidPin, 
    AnalogIn PotPin, uint32_t maxFlow_mL)
:   sensor         (SensorPin),
    valve          (SolenoidPin),
    pot            (PotPin),
    count          (0),
    flowRate_mL    (0),
    totalFlow_mL   (0),
    targetFlow_mL  (maxFlow_mL / 2),
    maxFlow_mL     (maxFlow_mL),
    //flowRate       (0.0f),
    lastSample     (pot.read_u16 ())
{
    sensor.rise  (this, &SmartSolenoidValve::PulseFlowSensor);
}

void SmartSolenoidValve::StartWatering  (int Index)
{
    count = flowRate_mL = totalFlow_mL = 0;   
     
    if  (targetFlow_mL == 0) return;
    
    OpenValve ();
    
    PrintLine ();
    Print  (Index);
}

void SmartSolenoidValve::Print  (int index)
{
    printf ("\r\n%i: flow rate =  %u mL/s, total =  %u mL, target =  %u mL\r\n", 
        index, flowRate_mL, totalFlow_mL, targetFlow_mL);
}

void SmartSolenoidValve::UpdateTargetFlow ()
{
    uint16_t sample = pot.read_u16 (),
        tempLastSample = lastSample;
    
    lastSample = sample;
    
    if ((sample > tempLastSample - 300) &&  (sample < tempLastSample + 300))
        return;
    
    uint32_t tempTargetFlow_mL =  (sample * maxFlow_mL) / 0xffff;
        
    targetFlow_mL = tempTargetFlow_mL < 100 ? 0 : tempTargetFlow_mL;
    
    if  (totalFlow_mL >= tempTargetFlow_mL)
        CloseValve ();
    else
        OpenValve ();
}

inline void SmartSolenoidValve::StopWatering  (int Index)
{
    CloseValve ();
    Print  (Index);
    printf ("\r\nDone watering");
    PrintLine ();
}

void SmartSolenoidValve::Update  (int Index)
{
    UpdateTargetFlow ();
    
    /// Update the flow sensor pulse count and flow calculations.
    
    uint16_t localCount = count;
    
    uint32_t tempFlowRate_mL =  (2000 * count) / 540;
    flowRate_mL = tempFlowRate_mL;
    totalFlow_mL += tempFlowRate_mL;
    
    if  (totalFlow_mL >= targetFlow_mL)
         StopWatering  (Index);
    else
        Print  (Index);
}

void SmartSolenoidValve::PulseFlowSensor () {
    ++count;
}

void SmartSolenoidValve::OpenValve () {
    valve = 1;
}

void SmartSolenoidValve::CloseValve () {
    valve = 0;
}

const Member* SmartSolenoidValve::op (byte index, Uniprinter* io) {
    static const Member this_member = { "SmartSolenoidValve", NumMembers (0), FirstMember('A'), "tek::sensors") };
    return &this_member;
    
    switch (Index)
    {
        case 'A':
            static const Member* member_A = { "Open", nullptr, nullptr, "Opens the valve." };
            if (!io) return &member_A;
    }
    
}

}   //< namespace Sensuators
}   //< namespace tek
