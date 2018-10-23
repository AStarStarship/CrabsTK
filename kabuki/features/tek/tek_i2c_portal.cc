/** kabuki::tek
    @version 0.x
    @file    ~/source/kabuki/tek/impl/tek_i2c_portal.cc
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

#include "i2c_portal.h"

using namespace _ {;

namespace kabuki { namespace tek {

I2cPortal::I2cPortal (Expr* expr, PinName sda_pin, PinName scl_pin) :
    expr_ (expr),
    i2c_  (sda_pin, scl_pin) {
    /// Nothing to do here!
}

void I2cPortal::Feed () {
    //while (serial_.readable ()) slot.StreamTxByte ();
}

void I2cPortal::Pull () {
/*
    //const int addr7bit = 0x48;      // 7-bit I2C address.
    const int addr8bit = 0x48 << 1; // 8-bit I2C address: 0x90.

    byte cmd[80]; //< @todo what size do  I need?
    cmd[0] = 0x01;
    cmd[1] = 0x00;
    i2c_.write (addr8bit, cmd, 2);

    wait (0.5);

    cmd[0] = 0x00;
    i2c_.write (addr8bit, cmd, 1);
    i2c_.read (addr8bit, cmd, 2);

    float tmp = (float ((cmd[0] << 8) | cmd[1]) / 256.0);
    printf ("\n| Temp = %.2f\n", tmp);
    */
}

}       //< namespace tek
}       //< namespace kabuki
