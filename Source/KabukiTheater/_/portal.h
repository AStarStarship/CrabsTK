/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoom\Portal.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_PORTAL_H
#define CHINESEROOM_PORTAL_H

namespace _ {

struct Portal
/** A portal that connects two Chinese Rooms through a Terminal.

    # Common Portal Types
    * UART
    * Dual-UART
    * SPI
    * I2C
    * Quad SPI
    * TCP/UDP
    * Bluetooth
    * CAN
*/
{
    virtual void Feed () = 0;
    /** Feeds tx messages through the io. */

    virtual void Pull () = 0;
    /** Pulls rx messages through the io. */
};

}       //< namespace _
#endif  //< CHINESEROOM_PORTAL_H
