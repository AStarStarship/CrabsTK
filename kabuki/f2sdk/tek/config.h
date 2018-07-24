/** kabuki::tek
    @file    ~/source/kabuki/tek/config.h
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
 
#ifndef KABUKI_TEK_MODULE_COFIG_H
#define KABUKI_TEK_MODULE_COFIG_H

#include "../config.h"
#include <mbed.h>

using namespace _ {;

typedef byte     offset_t;
typedef uint16_t ch_t;

enum {
	kBitToByteShift = sizeof (byte) * 8,
	//< Number of bits to shift to get the byte offset.
	kBitNumberMask  = 0xff,
	//< Mask for the bit number, which is byte.
};

#endif	//< KABUKI_TEK_MODULE_COFIG_H
