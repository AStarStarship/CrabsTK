/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/script/error.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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

#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR___ERROR
#define HEADER_FOR___ERROR

#include "config.h"

namespace _ {

typedef enum ScriptError {
    kErrorInputNil = 0,         //< Script Error  0: Nil/null input.
    kErrorInputTooLow,          //< Script Error  1: Input too low.
    kErrorInputTooHigh,         //< Script Error  2: Input too high.
    kErrorBufferOverflow,       //< Script Error  3: Buffer overflow.
    kErrorBufferUnderflow,      //< Script Error  4: Buffer underflow.
    kErrorVarintOverflow,       //< Script Error  5: Varint overflow.
    kErrorInvalidHash,          //< Script Error  6: Invalid hash.
    kErrorInvalidType,          //< Script Error  7: Invalid type.
    kErrorInvalidIndex,         //< Script Error  8: Invalid index.
    kErrorInvalidEnquery,       //< Script Error  9: Invalid enquiry.
    kErrorInvalidArgument,      //< Script Error 10: Invalid argument.
    kErrorInvalidDoor,          //< Script Error 11: Invalid door.
    kErrorTooManyParameters,    //< Script Error 12: Too many parameters.
    kErrorStackOverflow,        //< Script Error 13: Stack overflow.
    kErrorTooManyPops,          //< Script Error 14: Too many pops.
    kErrorTextOverflow,         //< Script Error 15: Text overflow.
    kErrorInvalidErrorHandler,  //< Script Error 16: Invalid error handler.
    kErrorInvalidOperand,       //< Script Error 17: Invalid operand.
    kErrorArrayOverflow,        //< Script Error 18: Array overflow.
    kErrorInvalidOperation,     //< Script Error 19: Invalid operation.
    kErrorMalformedUtf8,        //< Script Error 20: Malformed UTF-8.
    kErrorMalformedUtf16,       //< Script Error 21: Malformed UTF-16.
    kErrorMalformedUtf32,       //< Script Error 22: Malformed UTF-32.
    kErrorObjectLocked,         //< Script Error 23: Object locked.
    kErrorInvalidArgs,          //< Script Error 24: Invalid args.
    kErrorAuthenticationError,  //< Script Error 25: Authentication error.
    kErrorRoomNotFound,         //< Script Error 26: Room not found.
    kErrorImplementation,       //< Script Error 27: Implementation error.
} Error;

/** Returns an array or string pointers for the KabukiError(s). */
KABUKI const char** ErrorString ();

/** Returns one of the Errors () strings. */
KABUKI const char* ErrorString (Error error);

}       //< namespace _

#endif  //< HEADER_FOR___ERROR
