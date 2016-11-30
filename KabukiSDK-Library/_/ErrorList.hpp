/** Kabuki Software Development Kit
    @file    /.../_/ErrorList.cpp
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

#include <KabukiSDK-Config.hpp>

#include <FreeI2P.hpp>

namespace _ {

class _KabukiSDK ErrorList
/** An array of error strings.

    @code
    int maxNumErrors = 5;
    char* errorBuffer[maxNumErrors];
    ErrorList errors (ErrorBuffer, maxNumErrors);
    @endcode
*/
{
    public:

    enum {
        DefaultMaxNumErrors = 8     //< The default maxNumErrors.
    };

    ErrorList (char** errorBuffer, int maximumNumErrors);
    /*< Constructs an ErrorList from the given buffer and max number of errors. */

    void clear ();
    /*< Clears the error list. */

    int getNumErrors ();
    /*< Gets the number of errors. */

    int getMaxNumErrors ();
    /*< Gets the number of errors. */

    void report  (const char* s);
    /** Reports an error with the given message. */

    const char** getErrors ();
    /*< Gets the list of errors. */
    
    inline void print (I2P::Terminal& slot);
    /*< Prints this object to a terminal. */

    private:

    int numErrors,          //< The number of errors.
        maxNumErrors;       //< The max number of errors.

    const char** errors;    //< Pointer to an array of pointers to error strings.
};

}   //< namespace _
