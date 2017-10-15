/** Kabuki Tek
    @file    /.../Source/KabukiTek-Impl/sensors/Photosensor.cpp
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

#include <sensors/Photosensor.h>

namespace tek { namespace sensors {

Photosensor::Photosensor  (word AInPin)
:   input ((PinName)AInPin)
{
    /// Nothing to do here!
}

Photosensor::~Photosensor () {}

byte Photosensor::getState () { return 0; }

byte Photosensor::setState  (byte Value)
{
    return 0;
}

const char* Photosensor::read  (tek::Agent* A)
{
    static const word txHeader[] = { 1, 0,::FLT };
    if  (Enquery  (A)) return Header  (A, txHeader, 0);

    float reading = 0.0f;

    return Print  (A, txHeader, &reading);
}

byte PhotoSensor::getState ()
{
    return 0;
}

const char* PhotoSensor::setState (byte Value)
{
    return 0;
}

const char* Photosensor::Do  (const char* Query, byte Index,::Agent* A)
{
    //if  (Query) Index = Find  (Query, Index);

    switch  (Index)
    {
      case 0: return Query ? Enquery  (Query, "Read",
        "Reads the light sensor.")
        : Read  (A);

    }
    return Query ? Enquery  (Query, "Photosensor",
          "A standard photosensor hooked up to an ADC pin of a microcontroller.")
          : Errors  (InvalidIndex);
}

}   //< namespace sensors
}   //< namespace _
