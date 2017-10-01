/** The Chinese Room
    @version 0.x
    @file    ~/chinses_room/include/error.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/

#ifndef CHINESE_ROOM_ERROR_H
#define CHINESE_ROOM_ERROR_H

#include "args.h"
#include "utils.h"

namespace _ {

/** List of common errors. */
typedef enum {
    BufferUnderflowError = 0,
    BufferOverflowError,
    VarintOverflowError,
    InvalidHashError,
    ReadInvalidTypeError,
    InvalidRxStateError,
    InvalidTxStateError,
    InvalidIndexError,
    InvalidEnqueryError,
    InvalidRxTypeError,
    TypeMismatchError,
    InvalidArgumentError,
    InvalidRoomNumberError,
    TooManyParametersError,
    NullPointerError,
    NullHeaderError,
    NullRoomError,
    ScanErrorError,
    NoDevceSelectedError,
    NoDeviceInSelectedIndexError,
    StackOverflowError,
    InvalidNumBookPagesError,
    TooManyPopsError,
    RxStringBufferOverflowError,
    RxHandlingErrorError,
    NullDevicePushError,
    ReadErrorError,
    WriteErrorError,
    NonexistentMemberError,
    OutOfMemoryError,
    ArrayOverflowError,
    InvalidMemberError,
    InvalidErrorError,
} Error;

/** Returns the text label of the Error. */
KABUKI const char* ErrorString (Error e);

/** An array of error strings.
    
    @code
    int max_errors = 5;
    byte* errorBuffer[max_errors];
    ErrorList errors (ErrorBuffer, max_errors);
    @endcode

    @todo This code is too coupled with the Rx and Tx sockets. The code needs to store void* and 
          The type of error should dictate what these void* are.
*/
class ErrorList {
    //NONCOPYABLE (ErrorList)
    public:

    /** Constructs an ErrorList from the given buffer and max number of errors. */
    ErrorList ();

    /** Clears the error list. */
    void Clear ();

    /** Gets the number of errors. */
    int GetNumErrors ();

    /** Gets the number of errors. */
    int GetMaxNumErrors ();
    
    /** Reports an error with the given message. 
        @param e        The error type.
        @param header   The header where the error occurred.
        @param offset   The offset in the current header.
    */
    ticket_t Report (Error e, const uint_t* header = nullptr, byte offset = 0,
                     void* source = nullptr);

    byte* GetOffsets ();

    const uint_t** GetHeaders ();

    void** GetSources ();

    /** Gets the list of errors. */
    Error* GetErrors ();

    private:
    
    error_t num_errors_;                //< The max number of errors.
    Error errors_[kMaxErrors];          //< An array of error types.
    byte offsets_[kMaxErrors];          //< An array of error types.
    const uint_t* headers_[kMaxErrors]; //< An array of pointers to error headers.
    void* sources_[kMaxErrors];         //< An array of pointers to error sources.
};

/** Function that returns a static Error list with the given ErrorListNumber.
    The templates function allows for easy generation of multiple error lists.
    All you do to create another list is to alter the ErrorListNumber.
    @code
    ticket_t errorNumber = errors_<
    @endcode
*/
KABUKI ErrorList& Errors ();

/** Reports an error for later handling. */
KABUKI ticket_t Report (Error e, const uint_t* header, byte offset, 
                        void* source);

/** Reports an error for later handling. */
KABUKI ticket_t Report (Error e);

/** Reports an error for later handling. */
KABUKI ticket_t Report (Error e, const uint_t* header);

/** Reports an error for later handling. */
KABUKI ticket_t Report (Error e, const uint_t* header, void* source);

/** Reports an error for later handling. */
KABUKI ticket_t Report (Error e, volatile const uint_t* header,
                        byte* source);

/** Prints out an error report to the stdout.
    @param e The error type.
    @param params The parameter header.
    @param param_index the index in the params where the error occurred.
    @param source The source buffer address. */
KABUKI void PrintError (Error e, const uint_t* params, byte param_index,
                        void* source);

/** Prints out an error report to the stdout.
    @param e The error type.
    @param params The parameter header.
    @param param_index the index in the params where the error occurred.
    @param source The source buffer address. */
KABUKI void PrintReport (ErrorList* list, Error e, const uint_t* params,
                         byte param_index, void* source);

}       //< namespace _
#endif  //< CHINESE_ROOM_ERROR_H
