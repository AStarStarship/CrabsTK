/** The Chinese Room
    @version 0.x
    @file    /.../error.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

                                All right reserved (R).

             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License.
             You may obtain a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
             See the License for the specific language governing permissions and
             limitations under the License.
*/

#ifndef CHINESEROOM_ERROR_H
#define CHINESEROOM_ERROR_H

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

INLINE const char* ErrorString (Error e)
/** Returns the text label of the Error. */
{
#if USING_CONSOLE
    static const char* strings[] = {
        "Buffer underflow",             //< 0
        "Buffer overflow",              //< 1
        "Varint overflow",              //< 2
        "Invalid hash",                 //< 3
        "Read invalid type",            //< 4
        "Invalid rx_state",             //< 5
        "Invalid tx_state",             //< 6
        "Invalid index",                //< 7
        "Invalid equerry",              //< 8
        "Invalid type",                 //< 9
        "Type mismatch",                //< 10
        "Invalid argument number",      //< 11
        "Invalid room number",          //< 12
        "Too many parameters",          //< 13
        "Null pointer",                 //< 14
        "Null header",                  //< 15
        "Null room",                    //< 16
        "Scan error",                   //< 17
        "Object stack overflow",        //< 18
        "No device selected",           //< 19
        "Stack overflow",               //< 20
        "Invalid number of pages",      //< 21
        "Too many pops",                //< 22
        "Rx string buffer overflow",    //< 23
        "Rx handling error state",      //< 24
        "Null device push",             //< 25
        "Read error",                   //< 26
        "Write error",                  //< 27
        "Nonexistent member",           //< 28
        "Out of memory",                //< 29
        "Array overflow",               //< 30
        "Invalid Member",               //< 31
        "Invalid error"                 //< n
    };
    if (e < 0 || e > InvalidErrorError) 
        return strings[InvalidErrorError];
    return strings[e];
#else
    return kEmptyString;
#endif
}

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
    ErrorList ()
    :   num_errors_ (0) {
        /// Nothing to do here! :-)
    }

    /** Clears the error list. */
    void Clear () {
        num_errors_ = 0;
    }

    /** Gets the number of errors. */
    int GetNumErrors () {
        return num_errors_;
    }

    /** Gets the number of errors. */
    int GetMaxNumErrors () {
        return kMaxErrors;
    }
    
    /** Reports an error with the given message. 
        @param e        The error type.
        @param header   The header where the error occurred.
        @param offset   The offset in the current header.
    */
    ticket_t Report (Error e, const uint_t* header = nullptr, byte offset = 0,
                     void* source = nullptr) {
        if (num_errors_ >= kMaxErrors)
        {
            printf ("Error: ErrorList overflow!\n");
            return ~0;
        }
        errors_ [num_errors_] = e;
        headers_[num_errors_] = header;
        offsets_[num_errors_] = offset;
        sources_[num_errors_] = source;

        ++num_errors_;
        return num_errors_ - 1;
    }
    byte* GetOffsets () {
        return offsets_;
    }

    const uint_t** GetHeaders () {
        return headers_;
    }

    void** GetSources () {
        return sources_;
    }


    /** Gets the list of errors. */
    Error* GetErrors () {
        return errors_;
    }

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
INLINE ErrorList& Errors () {
    static ErrorList el;
    return el;
}

/** Reports an error for later handling. */
INLINE ticket_t Report (Error e, const uint_t* header, byte offset, 
                        void* source) {
    return Errors ().Report (e, header, offset, source);
}

/** Reports an error for later handling. */
INLINE ticket_t Report (Error e) {
    return Errors ().Report (e, 0, 0, 0);
}

/** Reports an error for later handling. */
INLINE ticket_t Report (Error e, const uint_t* header) {
    return Errors ().Report (e, header, 0, 0);
}

/** Reports an error for later handling. */
INLINE ticket_t Report (Error e, const uint_t* header, void* source) {
    return Errors ().Report (e, header, 0, source);
}

/** Reports an error for later handling. */
INLINE ticket_t Report (Error e, volatile const uint_t* header,
                        byte* source) {
    return Errors ().Report (e, const_cast<const uint_t*>(header), 0, 0);
}

/** Prints out an error report to the stdout.
@param e The error type.
@param params The parameter header.
@param param_index the index in the params where the error occurred.
@param source The source buffer address. */
INLINE void PrintError (Error e, const uint_t* params, byte param_index,
                        void* source) {
    enum {
        kNumLines = 10
    };
    for (int i = 0; i < kNumLines; ++i)
        std::cout << '\n';
    printf ("%s error in argument %u:%s of ", ErrorString (e), param_index,
            TypeString (ParamNumber (params, param_index)));
    PrintEsc (params);
    for (int i = 0; i < kNumLines; ++i)
        std::cout << '\n';
}

/** Prints out an error report to the stdout.
@param e The error type.
@param params The parameter header.
@param param_index the index in the params where the error occurred.
@param source The source buffer address. */
INLINE void PrintReport (ErrorList* list, Error e, const uint_t* params,
                         byte param_index, void* source) {
    error_t num_errors = list->GetNumErrors ();
    Error* errors = list->GetErrors ();
    byte* offsets = list->GetOffsets ();
    const uint_t** headers = list->GetHeaders ();
    void** sources = list->GetSources ();
    printf ("\nError Report:\n");

    for (error_t i = 0; i < num_errors; ++i) {
        printf ("%4u: ", i);
        PrintError (errors[i], headers[i], offsets[i], sources[i]);
    }
}

}       //< namespace _
#endif  //< CHINESEROOM_ERROR_H
