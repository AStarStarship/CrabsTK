/** The Chinese Room
    @version 0.x
    @file    /.../error.h
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

#ifndef CHINESEROOM_ERROR_H
#define CHINESEROOM_ERROR_H

#include "args.h"
#include "print.h"

namespace _ {

// typedef for an error: we have less than 255
typedef unsigned char error_t;

/** List of common errors. */
typedef enum {
    BufferUnderflowError = 0,
    BufferOverflowError,
    VarintOverflowError,
    InvalidHashError,
    ReadInvalidTypeError,
    InvalidStateError,
    InvalidIndexError,
    InvalidEnqueryError,
    UnsupportedTypeError,
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
    InvalidErrorError,
} Error;

inline const char** ErrorStrings ()
/** Returns the text label of the Error. */
{
    static const char* errorStrings[] = {
        "Buffer underflow",             //< 1
        "Buffer overflow",              //< 2
        "Varint overflow"
        "Invalid hash",
        "Read invalid type",
        "Invalid state",
        "Invalid index",
        "Invalid equerry",
        "Unsupported type",
        "Type mismatch error"
        "Invalid argument number"
        "Invalid room number",
        "Too many parameters",
        "Null pointer",
        "Null header",
        "Null room",
        "Scan error",
        "Object stack overflow",
        "No device selected error",
        "Stack overflow",
        "Invalid number of pages",
        "Too many pops",
        "Rx string buffer overflow error",
        "Rx handling error state",
        "Null device push error",
        "Read error",
        "Write error",
        "Nonexistent member",
        "Out of memory",
        "Invalid error",
    };
    return errorStrings;
}

inline const char* ErrorString (Error e)
/** Returns the text label of the Error. */
{
    if (e < 0 || e >= InvalidErrorError) return ErrorStrings ()[InvalidErrorError];
    return ErrorStrings ()[e];
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
                     byte* source = nullptr) {
        if (num_errors_ >= kMaxErrors)
        {
            printf ("Error: ErrorList overflow!\n");
            return ~0;
        }
        errors_[num_errors_] = e;
        headers_[num_errors_] = header;
        offsets_[num_errors_] = offset;
        sources_[num_errors_] = source;

#if DEBUG_CHINESEROOM
        PrintError (e, header, offset, source);
#endif

        ++num_errors_;
        return num_errors_ - 1;
    }

    /** Prints out an error report to the stdout.
    @param e The error type.
    @param params The parameter header.
    @param param_index the index in the params where the error occurred.
    @param source The source buffer address. */
    void PrintReport (Error e, const uint_t* params, byte param_index, byte* source) {
        error_t num_errors = num_errors_;
        Error* errors = errors_;
        byte* offsets = offsets_;
        const uint_t** headers = headers_;
        byte** sources = sources_;
        printf ("\nError Report:\n");

        for (error_t i = 0; i < num_errors_; ++i) {
            printf ("%4u: ", i);
            PrintError (errors[i], headers[i], offsets[i], sources[i]);
        }
    }

    /** Prints out an error report to the stdout.
        @param e The error type. 
        @param params The parameter header. 
        @param param_index the index in the params where the error occurred.
        @param source The source buffer address. */
    void PrintError (Error e, const uint_t* params, byte param_index, 
                     byte* source)
    {
        enum {
            kNumLines = 10
        };
        PrintLines (kNumLines);
        printf ("%s error in argument %u:%s of ", ErrorString (e), param_index, 
                TypeString (ParamNumber (params, param_index)));
        PrintParams (params);
        PrintLines (kNumLines);
    }

    /** Gets the list of errors. */
    Error* GetErrors () {
        return errors_;
    }

    /** Prints the error list to the error stream. 
    void Print () {
        static const byte StartString[] = "\r\nStart\0",
            StopString[] = "\r\nStop\0",
            BeginString[] = "\r\nBegin\0",
            EndString[] = "\r\nEnd\0",
            pointToErrorLocationString[] = "Start of error ---v\0";

        const byte* pointerHex;

        if (sizeof (intptr_t) == 64)
            pointerHex = "0x%08x %s";
        else if (sizeof (intptr_t) == 32)
            pointerHex = "0x%04x %s";
        else// if (sizeof (intptr_t) == 16)
            pointerHex = "0x%02x %s";

        for (int i = 0; i < num_errors_; ++i)
        {
            byte* GetAddress = sources_[i];
            Door* door = Doors ().getDoor (GetAddress);
            if (door = nullptr) return;
            Terminal* t = door->getTerminal (GetAddress);
            if (t = nullptr) return;

            printf ("%s error!\n\r", ErrorString (errors_[i]));

            t->print ();

            /// Print header.

            const uint_t* header = headers_[i] - offsets_[i] * sizeof (const uint_t*);

            uint_t numParameters = *header;

            printf ("\r\nHeader:\n\rNum parameters: %u\n\r", numParameters);

            for (uintptr_t i = 0; i < numParameters; ++i)
            {
                header += sizeof (const uint_t*);
                uint_t type = *header;
                if (TypeHasBuffer (type))
                {
                    uint_t bufferSize = * (header += sizeof (const uint_t*));
                    printf ("%u", bufferSize);
                }

                printf (" Pointer: ");
                header += sizeof (const uint_t*);
                type = *header;
                printf ("%s", TypeString (type));
            }

            printf ("\n\rError location: 0x");
            printf (pointerHex, errors_[i]);
        }
    }*/

    private:
    
    error_t num_errors_;                //< The max number of errors.
    Error errors_[kMaxErrors];          //< An array of error types.
    byte offsets_[kMaxErrors];          //< An array of error types.
    const uint_t* headers_[kMaxErrors]; //< An array of pointers to error headers.
    byte* sources_[kMaxErrors];         //< An array of pointers to error sources.
};

/** Function that returns a static Error list with the given ErrorListNumber.
    The templates function allows for easy generation of multiple error lists.
    All you do to create another list is to alter the ErrorListNumber.

    @code
    ticket_t errorNumber = errors_<
    @endcode
*/
static ErrorList& Errors () {
    static ErrorList el;
    return el;
}

/** Reports an error for later handling. */
inline ticket_t Report (Error e, const uint_t* header, byte offset, 
                             byte* source) {
    return Errors ().Report (e, header, offset, source);
}

/** Reports an error for later handling. */
inline ticket_t Report (Error e) {
    return Errors ().Report (e, 0, 0, 0);
}

/** Reports an error for later handling. */
inline ticket_t Report (Error e, const uint_t* header) {
    return Errors ().Report (e, header, 0, 0);
}

/** Reports an error for later handling. */
inline ticket_t Report (Error e, const uint_t* header, byte* source) {
    return Errors ().Report (e, header, 0, source);
}

/** Reports an error for later handling. */
inline ticket_t Report (Error e, volatile const uint_t* header,
                             byte* source) {
    return Errors ().Report (e, const_cast<const uint_t*>(header), 0, 0);
}

}       //< namespace _
#endif  //< CHINESEROOM_ERROR_H
