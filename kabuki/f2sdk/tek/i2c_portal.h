/** kabuki::tek
    @file    ~/source/kabuki/tek/include/i2c_portal.h
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

#ifndef KABUKI_TEK_I2C_PORTAL_H
#define KABUKI_TEK_I2C_PORTAL_H

#include "config.h"

namespace kabuki { namespace tek {

/** A Script Portal for a half-duplex I2C data link. */
class I2cPortal: public _::Portal {

    /** Constructor creates a loop back port. */
    I2cPortal (_::Expr* expr, PinName sda_pin, PinName scl_pin);
    
    /** Feeds tx messages through the a without scanning them. */
    virtual void Feed ();

    /** Pulls rx messages through the a and runs them through the scanner. */
    virtual void Pull ();

    private:

    _::Expr* expr_; //< Expr for this Portal.
    I2C i2c_;             //< mbed Serial port.
};      //< class I2CPortal
}       //< namespace tek
}       //< namespace kabuki
#endif  //< KABUKI_TEK_H_I2C_PORTAL_H
