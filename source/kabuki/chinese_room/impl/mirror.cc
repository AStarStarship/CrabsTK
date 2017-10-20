/** The Chinese Room
    @version 0.x
    @file    ~/source/kabuki/chinese_room/include/mirror.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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

#include <stdafx.h>
#include "../include/mirror.h"
#include "../include/portal.h"
#include "../include/expression.h"

namespace _ {

Mirror::Mirror () :
    a (nullptr)
{

}

void Mirror::Prime () {

}

uint_t Mirror::Length () {
    return MonoidRxSpace (ExpressionRx (a));
}

void Mirror::Feed (byte b) {

}

byte Mirror::Pull () {
    return 0;
}

Expression* Mirror::SetExpression (Expression* automata)
{
    if (automata == nullptr)
        return nullptr;
    a = automata;
}

Expression* Mirror::GetExpression () {
    return a;
}

const Operation* Read (Mirror* mirror, const uint_t* header, void** args) {

    return 0;
}

const Operation* Write (Mirror* mirror, const uint_t* header, void** args) {

    return 0;
}
Mirror MirrorInit (Expression* a, bool is_dynamic) {
    Mirror mirror;
    mirror.SetExpression (a);
    return mirror;
}

const Operation* Write (Expression* a, byte op, const char* string) {
    if (a == nullptr)
        return nullptr;
#if DEBUG_CHINESE_ROOM
    printf ("| Writting string to %p\n", a);
#endif
    Bout* tx = ExpressionTx (a);
    // Temp variables packed into groups of 8 bytes for memory alignment.
    byte c;

    uint_t size = tx->size,
        space;

    // Convert the socket offsets to pointers.
    byte* begin = MonoidTxSlot (tx),
        *end = begin + size,
        *start = begin + tx->start,
        *stop = begin + tx->stop;
    space = MonoidSpace (start, stop, size);
    //  * temp_ptr;
    //const byte* str_ptr;              //< This was not initialized???
    const byte* str_ptr = reinterpret_cast<const byte*> (string);
    //< I think this is what needs to happen.

    *stop = op;
    ++stop;
    // We know we will always write at least one null-term byte.
    c = *str_ptr;
    *stop = c;
    ++stop;

    while (c != 0) {
        if (space-- == 0) {
            ticket_t error_ticket = Report (BufferUnderflowError,
                                            Esc<1, STX> (), 0, start);
            return SetErrorTicket (error_ticket);
        }
        ++str_ptr;
        c = *str_ptr;     // Read byte.

        *stop = c;        // Write byte
        ++stop;

        if (stop >= end) stop -= size;
    }
    *stop = c;        // Write byte
    ++stop;
    tx->stop = Diff (stop, begin);
    return 0;
}

/** Compiler says this already has a body???
const Operation* Write (Mirror* mirror, const uint_t* params, void** args) {
    //ticket_t Write (Mirror* a, const uint_t* header, void** args) {
    return Write (mirror->GetExpression (), params, args);
}*/

template<uint_t kNumber>
Mirror& Log () {
    static byte* buffer = Buffer<kNumber, kLogSize> ();
    static Mirror s = LogInit (TxInit (buffer, kLogSize));
    return s;
}

Mirror& operator<< (Mirror& mirror, int8_t value) {
    char buffer[5];
    sprintf_s (buffer, 5, "%i", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, uint8_t value) {
    char buffer[4];
    sprintf_s (buffer, 4, "%u", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, int16_t value) {
    char buffer[7];
    sprintf_s (buffer, 7, "%i", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, uint16_t value) {
    char buffer[6];
    sprintf_s (buffer, 6, "%u", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, int32_t value) {
    char buffer[12];
    sprintf_s (buffer, 12, "%i", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, uint32_t value) {
    char buffer[11];
    sprintf_s (buffer, 11, "%u", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, int64_t value) {
    char buffer[22];
    sprintf_s (buffer, 22, "%I64d", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, uint64_t value) {
    char buffer[21];
    sprintf_s (buffer, 21, "%llu", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, float value) {
    char buffer[FLT_MAX_10_EXP + 2];
    sprintf_s (buffer, FLT_MAX_10_EXP + 2, "%f", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, double value) {
    char buffer[DBL_MAX_10_EXP + 2];
    sprintf_s (buffer, DBL_MAX_10_EXP + 2, "%f", value);
    Write (mirror.GetExpression (), '?', buffer);
    return mirror;
}

Mirror& operator<< (Mirror& mirror, const char* s) {
    Write (mirror.GetExpression (), '?', s);
    return mirror;
}


}       //< namespace _
