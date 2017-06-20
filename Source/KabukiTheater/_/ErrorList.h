/** The Chinese Room
    @file    /.../ChineseRoom/ErrorList.h
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

#ifndef CHINESEROOM_ERRORLIST_H
#define CHINESEROOM_ERRORLIST_H

#include "Error.h"
#include "Door.h"
#include "args.h"

namespace _ {

#ifndef MAX_NUM_ERRORS
#define MAX_NUM_ERRORS  8
typedef unsigned int ticket_t;
#endif

class ErrorList
/** An array of error strings.
    
    @code
    int maxNumErrors = 5;
    char* errorBuffer[maxNumErrors];
    ErrorList errors(ErrorBuffer, maxNumErrors);
    @endcode
*/
{
    public:

    enum {
        DefaultMaxNumErrors = 8     //< The default maxNumErrors.
    };

    ErrorList()
    /*< Constructs an ErrorList from the given buffer and max number of errors. */
    :   numErrors (0)
    {
        /// Nothing to do here! :-)
    }

    void clear()
    /*< Clears the error list. */
    {
        numErrors = 0;
    }

    int getNumErrors()
    /*< Gets the number of errors. */
    {
        return numErrors;
    }

    int getMaxNumErrors()
    /*< Gets the number of errors. */
    {
        return MAX_NUM_ERRORS;
    }

    ticket_t report(Error e, const uint_t* header = nullptr, byte offset = 0, byte* source = nullptr)
    /** Reports an error with the given message. 
        @param e        The error type.
        @param header   The header where the error occurred.
        @param offset   The offset in the current header.
    */
    {
        if (numErrors >= MAX_NUM_ERRORS)
        {
            printf ("Error: ErrorList overflow!\n");
            return ~0;
        }
        errors[numErrors] = e;
        headers[numErrors] = header;
        offsets[numErrors] = offset;
        sources[numErrors] = source;

        printReport (e, header, offset, source);

        ++numErrors;
        return numErrors - 1;
    }

    void printReport (Error e, const uint_t* header, byte offset, byte* source)
    {
        #if DEBUG
        printf ("%s: ", errorString (e));
        printParams (header);
        printf ("\n    offset: %u", offset);
        if (source == nullptr)
        {
            printf (" source: nullptr\n");
        }
        printf (" source: %s\n", typeString (*source));
        #endif
    }

    error_t* getErrors ()
    /*< Gets the list of errors. */
    {
        return errors;
    }

    void print()
    /*< Prints the error list to the error stream. */
    {
        static const char StartString[] = "\r\nStart\0",
            StopString[] = "\r\nStop\0",
            BeginString[] = "\r\nBegin\0",
            EndString[] = "\r\nEnd\0",
            pointToErrorLocationString[] = "Start of error ---v\0";

        const char* pointerHex;

        if(sizeof (intptr_t) == 64)
            pointerHex = "0x%08x %s";
        else if (sizeof (intptr_t) == 32)
            pointerHex = "0x%04x %s";
        else// if (sizeof (intptr_t) == 16)
            pointerHex = "0x%02x %s";

        for (int i = 0; i < numErrors; ++i)
        {
            byte* address = sources[i];
            Door* door = Doors ().getDoor (address);
            if (door = nullptr) return;
            Terminal* t = door->getTerminal (address);
            if (t = nullptr) return;

            printf ("%s error!\n\r", errorString (errors[i]));

            t->print ();

            /// Print header.

            const uint_t* header = headers[i] - offsets[i] * sizeof (const uint_t*);

            uint_t numParameters = *header;

            printf ("\r\nHeader:\n\rNum parameters: %u\n\r", numParameters);

            for (uintptr_t i = 0; i < numParameters; ++i)
            {
                header += sizeof (const uint_t*);
                uint_t type = *header;
                if (typeHasBuffer (type))
                {
                    uint_t bufferSize = *(header += sizeof (const uint_t*));
                    printf ("%u", bufferSize);
                }

                printf (" Pointer: ");
                header += sizeof (const uint_t*);
                type = *header;
                printf ("%s", typeString (type));
            }

            printf ("\n\rError location: 0x");
            printf (pointerHex, errors[i]);
        }
    }

    private:
    
    NONCOPYABLE(ErrorList)

    error_t numErrors;                      //< The max number of errors.
    error_t errors[MAX_NUM_ERRORS];         //< An array of error types.
    byte offsets[MAX_NUM_ERRORS];           //< An array of error types.
    const uint_t* headers[MAX_NUM_ERRORS];  //< An array of pointers to error headers.
    byte* sources[MAX_NUM_ERRORS];          //< An array of pointers to error sources.
};

ErrorList* errors()
/*< Function that returns a static Error list with the given ErrorListNumber.
    The templates function allows for easy generation of multiple error lists. 
    All you do to create another list is to alter the ErrorListNumber.

    @code
    ticket_t errorNumber = Errors<
    @endcode
*/
{
    static ErrorList errors;
    return &errors;
}

}       //< namespace _

#endif  //< CHINESEROOM_ERRORLIST_H
