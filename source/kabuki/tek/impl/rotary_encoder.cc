/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/sensors/RotaryEncoder.cpp
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

#include <sensors/RotaryEncoder.h>

namespace tek { namespace sensors {

#include <Controller.h>
#include <RotaryEncoder.h>

RotaryEncoder::RotaryEncoder  (uint inputA, uint inputB): 
    inA  (inputA), 
    inB  (inputB)
{
    //Assert  (32 <  (inputB -  (inputA & BitNumberMask)),
    //    "Error: RotaryEncoder::RotaryEncoder  (uint,uint): Encoders are not word aligned!");
}

inline int16_t RotaryEncoder::GetAccelerationMultiplier  (int32_t currentTime)
{
    /*
    static const byte curves[3][_TekHMIHMI_NumAccellorationCurves] = {
        {
    
    int timeDelta = currentTime - lastMoveTime,
        lastMoveTime = currentTime;
    */
    return 0;
}

/** Returns non-zero if there is change in the rotary encoder state.
    @return Returns 0 if there is not change, l if the knob turned right, and -1 if the knob turned left. */
inline void RotaryEncoder::Poll  (Controller* controller, uint channel, byte* debouncedXOR, int currentTime)
{
    uint inputA = inA,
        inputB = inB;
    
    byte maskA = debouncedXOR[inputA >> BitToByteShift] &  (1 <<  (inputA & BitNumberMask)),
        maskB = debouncedXOR[inputB >> BitToByteShift] &  (1 <<  (inputB & BitNumberMask));
    
    if ((maskA & maskB) == 0) return; //< No change in state.
    
    //int16_t value =  (maskA ? -1 : 1) * GetAccelerationMultiplier  (currentTime);
    
    
}

byte RotaryEncoder::getState ()
{
    return 0;
}

const char* RotaryEncoder::setState (byte Value)
{
    return 0;
}

const char* RotaryEncoder::Op (byte index, Uniprinter* io)
{
    switch (Index)
    {
        case 0: return tek::NumMembers (0);
    }
    
    return Query ? Enquery ("RotaryEncoder", "tek::sensors"): InvalidIndex ();
}

}   //< namespace sensors
}   //< namespace tek
