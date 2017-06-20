/** The Chinese Room
    @version 0.x
    @file    /.../Error.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_ERROR_H
#define CHINESEROOM_ERROR_H

#include "Config.h"

namespace _ {

typedef byte error_t;   //< We have less than 255 types of errors.

typedef enum
/** List of common errors. */
{
    BufferUnderflowError = 0,
    BufferOverflowError,
    VarintOverflowError,
    ReadInvalidTypeError,
    InvalidDataError,
    InvalidStateError,
    InvalidIndexError,
    InvalidEnqueryError,
    UnsupportedTypeError,
    TypeMismatchError,
    InvalidRoomNumberError,
    TooManyParametersError,
    NullPointerError,
    NullHeaderError,
    NullRoomError,
    ScanErrorError,
    NoDevceSelectedError,
    NoDeviceInSelectedIndexError,
    StackOverflowError,
    UTF16NotSupportedError,
    NonZeroPaddingError,
    InvalidNumBookPagesError,
    InvalidErrorError,
    TooManyPopsError,
    RxStringBufferOverflowError,
    RxHandlingErrorState,
    NullProcessError,
    NullDevicePushError,
    ReadErrorError,
    WriteErrorError,
    NonexistentMemberError,
    NumErrors,
} Error;

inline const char** errorStrings()
/*< Returns the text label of the Error. */
{
    static const char* errorStrings[] = {
        "Buffer underflow",
        "Buffer overflow",
        "Varint overflow"
        "Read invalid type",
        "Invalid data read",
        "Invalid state",
        "Invalid index",
        "Invalid equerry",
        "Unsupported type",
        "Type mismatch error"
        "Invalid argument number"
        "Too many parameters",
        "Null pointer",
        "Null header",
        "Null terminal",
        "Scan error",
        "Object stack overflow",
        "UTF-16 not supported",
        "Non-zero padding",
        "No device selected error",
        "Invalid number of Book pages",
        "Invalid error",
        "Too many pops",
        "Rx string buffer overflow error",
        "Rx handling error state",
        "Null program error",
        "Null device push error",
        "Read error",
        "Write error",
        "Nonexistent member",
    };
    return errorStrings;
}

inline const char* errorString(error_t e)
/*< Returns the text label of the Error. */
{
    if (e >= InvalidErrorError) return errorStrings ()[InvalidErrorError];
    return errorStrings ()[e];
}

inline const char* errorString(Error e)
/*< Returns the text label of the Error. */
{
    if (e < 0 || e >= InvalidErrorError) return errorStrings ()[InvalidErrorError];
    return errorStrings ()[e];
}

}       //< namespace _

#endif  //< CHINESEROOM_ERROR_H
