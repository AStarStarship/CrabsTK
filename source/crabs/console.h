/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/crabs/console.h
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

#ifndef HEADER_FOR_CRABS_CONSOLE
#define HEADER_FOR_CRABS_CONSOLE

#include "expr.h"

#if MAJOR_SEAM >= 1 && MINOR_SEAM >= 3

#if USING_TEXT_SCRIPT

namespace _ {

/** Gets or sets the Write buffer.
    
    To use dynamic  memory input a nil buffer pointer and non-zero size. To 
    delete the buffer enter 0 for the buffer size.

    @param buffer      The buffer, or nil to create dynamic buffer.
    @param buffer_size The size of the buffer in bytes.
    @return Returns a pointer to the buffer or a new buffer. */
KABUKI Slot& ConsoleSet (uintptr_t *buffer, uintptr_t buffer_size);

/** The primary out slot.
    To set the Print () @see SlotWriteSet 
*/
KABUKI Slot& Print ();

/** Dumps the Write Slot to the Console.
    @return Returns the Write Slot. */
KABUKI Slot& Scan ();

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

/** Writes the given slot to the Printer. */
KABUKI void ConsoleWrite (const char* slot);

/** Writes the given slot to the Printer. */
KABUKI void ConsoleWrite (Slot& slot);

/** Writes then clears the given slot to the Printer. */
KABUKI void ConsoleDump  (Slot& slot);

/** Terminal for reading and writing to Printer and Keyboard.
    
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

    Console () {
        char* buffer_begin = reinterpret_cast<char*> (input_buffer_);
        Slot::begin = buffer_begin;
        Slot::start = buffer_begin;
        Slot::stop  = buffer_begin;
        Slot::end   = buffer_begin;

    }

    Console (const Console& other) :
        Slot (other) {
        // Nothing to do here! ({:->)-+=<
    }

    void PromptInput (const char* header) {
        //ConsoleReadSlot (header, input_buffer_,
        //                 input_buffer_ + kInputBufferSize);

    }

    /** Script Operations.
        @param index The index of the expression.
        @param expr  The Expr to read and write from.
        @return      Returns nil upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Op* Star (wchar_t index, Expr* expr) {
        static const Op kThis = { "Console",
            OpFirst ('A'), OpLast ('A'),
            "A full-duplex computer text console.",
            '}', ';', ' ', false, nullptr, nullptr
        };
        void* args[1];
        switch (index) {
            case '?': return ExprQuery (expr, kThis);
            case 'A': {
                static const Op kOpA = { "Foo",
                    Bsq<1, SI4> (), Bsq<1, SI4> (),
                    "The classic one and only example function name.",
                    '(', ')', ' ', false, nullptr, nullptr
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
    uintptr_t input_buffer_[(kInputBufferSize >> kWordSizeShift) + 1];

};      //< class Console
}       //< namespace _
#endif
#endif  //< MAJOR_SEAM >= 1 && MINOR_SEAM >= 2
#endif  //< HEADER_FOR_CRABS_CONSOLE
