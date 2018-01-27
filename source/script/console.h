/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/console.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_SCRIPT_CONSOLE
#define HEADER_FOR_SCRIPT_CONSOLE

#include "expr.h"

#if USING_SCRIPT_TEXT

namespace _ {

/** Prints the given value to the stdout. */
inline void Write (int8_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (uint8_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (int16_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (uint16_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (int32_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (uint32_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (int64_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (uint64_t value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (float value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void Write (double value) {
    std::cout << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int8_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint8_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int16_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint16_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int32_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint32_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int64_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint64_t value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (float value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (double value) {
    std::cout << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void CErr (int8_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (uint8_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (int16_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (uint16_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (int32_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (uint32_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (int64_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (uint64_t value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (float value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void CErr (double value) {
    std::cerr << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int8_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint8_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int16_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint16_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int32_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint32_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (int64_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (uint64_t value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (float value) {
    std::cerr << "\n" << value;
}

/** Prints the given value to the stdout. */
inline void WriteLine (double value) {
    std::cerr << "\n" << value;
}

/** Reads a single char from the keyboard
    @param header The header to print. */
KABUKI char ConsoleReadChar (const char* header);

/** Reads an int from the keyboard.
    @param header The header to print.
    @param number The number to read. */
KABUKI int ConsoleReadInt (const char* header);

/** Reads a float from the keyboard.
    @param header The header to print.
    @param number The number to read. */
KABUKI float ConsoleReadFloat (const char* header);

/** Reads a new char from the keyboard.
    @param  header     The header to print.
    @param  target     Start of the buffer to read to.
    @param  target_end End of the buffer to read too.
    @return Returns a new char that you have to delete. */
KABUKI void ConsoleReadSlot (const char* header, char* target,
                               char* target_end);

/** Writes the given slot to the stdout. */
KABUKI void ConsoleWrite (const char* slot);

/** Writes the given slot to the stdout. */
KABUKI void ConsoleWrite (Slot& slot);

/** Writes then clears the given slot to the stdout. */
KABUKI void ConsoleDump  (Slot& slot);

/** Console for reading and writing to stdout and stderr
    
    Console writes to text buffer and prints to the std.

    # Keyboard Console Input Example

    @code
    | Enter Command:
    < parent child1.DoThis 1 2 3 DoThat "Example"
    < parent.child2.DoThat("Hello world!")
    @endcode
*/
class Console : public Slot, public Op {
    public:

    enum {
        kInputBufferSize = 256,
        kInitWidth       = 80,
        kInitHeight      = 64,
        kInitSize        = kInitWidth * kInitHeight,
    };

    Console () :
        Slot (kInitSize) {

    }

    Console (const Console& other) :
        Slot (other) {
        // Nothing to do here! ({:->)-+=<
    }

    void PromptInput (const char* header) {
        ConsoleReadSlot (header, input_buffer_,
                           input_buffer_ + kInputBufferSize);

    }

    /** Script Operations.
        @param index The index of the expression.
        @param expr  The Expr to read and write from.
        @return      Returns nil upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Op* Star (wchar_t index, Expr* expr) {
        static const Op kThis = { "Console",
            OpFirst ('A'), OpLast ('A'),
            "A full-duplex computer text console.", '}', ';', ' ', '-', nullptr
        };
        void* args[1];
        switch (index) {
            case '?': return ExprQuery (expr, kThis);
            case 'A': {
                static const Op kOpA = { "Foo",
                    _::Bsq<1, SI4> (), _::Bsq<1, SI4> (),
                    "The classic one and only example function name.",
                    '(', ')', ' ', ' ', nullptr
                };
                if (!expr) {
                    return &kOpA;
                }
                int32_t value;
                if (ExprArgs (expr, kOpA, Args (args, &value))) {
                    return expr->result;
                }
                // Function code goes here.
                return ExprResult (expr, kOpA, Args (args, &value));
            }
        }
        return nullptr;
    }

    private:

    size_t input_buffer_size_;
    char   input_buffer_[kInputBufferSize];

};      //< class Console
}       //< namespace _
#endif  //< USING_SCRIPT_TEXT
#endif  //< HEADER_FOR_SCRIPT_CONSOLE
