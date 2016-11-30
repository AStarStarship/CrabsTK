/** Kabuki Software Development Kit
    @version 0.9
    @file    /.../KabukiSDK/_Math/Hex.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>

namespace _Math {

char hexToChar  (byte Value);
/*< Conserts the hex value to ASCII char. */

void hexToString  (char* result, int value);
/*< Converts a give Value to a hex string. */

void hexToString  (char* result, byte value);
/*< Converts a give Value to a hex string. */

int hexToInt  (const char* binaryHex);
/*< Converts . */

byte hexToByte  (char value);
/*< Converts the given hex char value to a byte value 0 - 15. */

}   //< namespace _Math
