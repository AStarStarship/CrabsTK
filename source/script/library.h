/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/script/expr.h
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

#ifndef SCRIPT_LIBRARY_H
#define SCRIPT_LIBRARY_H

#include "config.h"

namespace _ {
#if USE_MORE_ROM
template<typename TIndex, typename TKey, typename TData,
         uint_t MaxStackHeight>
class Library: public Operand
{
    public:

    /** Creates an empty automata. */
    Library ()
    :   num_members_ (0),
        num_collisions_ (0),
        header_size_ (0),
        collisions_size_ (0),
        data_size_ (0) {

    }

    virtual ~Library () {

    }

    /** Returns true if the current Bag has a hash table. */
    bool HasHashTable () {
        return collisions_size_ != 0;
    }

    /** Gets the number of keys in the current scope. */
    uint_t GetBagType () {
        return 0;
    }

    /** Gets the number of keys in the current scope. */
    uint_t GetNumKeys () {
        return num_members_;
    }

    /** Gets the size of the header in bytes. */
    uint_t GetHeaderSize () {
        return header_size_;
    }

    /** Gets the data size in bytes. */
    uint_t GetDataSize () {
        return data_size_;
    }
    
    /** Attempts to add the Page data into the Object at the given BaseAddress.
        @return Returns nil upon success and an error char upon failure. */
    const Op* Add (byte type, const char* key, void* data) {
        TIndex size_of_type = getSizeOfType (type);
        if (size_of_type == 0)
        {
            return 0;
        }
        return 0;
    }
     
    /** Attempts to insert the Page data into the Object at the given index.
        @return Returns nil upon success and an error char upon failure. */
    const Op* Insert (byte type, const char* key, void* data, TIndex index = 0) {
        TIndex l_numOps = numNumbers;
        if (index > l_numOps) index = l_numOps;

        return 0;
    }

    /** Attempts to remove the Page data into the Object at the given index.
        @return Returns nil upon success and an error char upon failure. */
    const Op* Remove (TIndex index) {
        return 0;
    }

    /** Attempts to clear the page at the given index.
        @return Returns nil upon success and an error char upon failure. */
    const Op* Clear (TIndex index) {
        return 0;
    }
    
    /** Attempts to find the given op name.
        @return Returns an invalid index upon failure. */
    TIndex Find (const char* key) {
        return 0;
    }

    /** Searches for the given query and returns a bag of query results.  */
    bool Search (const char* query, Library<TIndex, TKey, TData, TData,
        MaxStackHeight>* results) {
        return false;
    }

    /** gets the size of the item at the given index. */
    byte GetOpSize (TIndex index) {
        return 0;
    }

    /** gets the size of the item at the given index. */
    byte SetOpSize (TIndex index, TData newSize) {
        return 0;
    }
    
    /** Returns the data address of the given op if it exists.
        @return Returns a pointer to one of the ChineseRoom error strings upon failure. */
    void* GetDataAddress (TIndex index) {
        #if _BufferUIntSize >= 64
        IndexType64_t* UI8_ptr = (IndexType64_t*)address;
        #elif _BufferUIntSize >= 32
        IndexType32_t* UI4_ptr = (IndexType32_t*)address;
        #elif _BufferUIntSize >= 16
        IndexType16_t* UI2_ptr = (IndexType16_t*)address;
        #elif _BufferUIntSize != 8
        #error IndexType_t invalid size!
        #endif

        IndexType_t size = 0;

        byte type = (*address) & 0x1f;

        switch (type)
        {
            case RS:
                #if _BufferUIntSize >= 16
                /// Library format: { UI1, UI1, UI2, UI2 }
                return size + sizeof (Library) + * (address + 1) * 
                       sizeof (byte) + *(UI2_ptr + 2) + * (UI8_ptr + 4);
                #else
                return 0;
                #endif
            case GS:
                #if _BufferUIntSize >= 32
                /// Library format: { UI1, UI1, UI2, UI4 }
                return size + sizeof (Bag32) + * (UI2_ptr + 2) * 
                       sizeof (byte) + * (UI4_ptr + 4) + * (UI8_ptr + 8);
                #else
                return 0;
                #endif
            case FS:
                #if _BufferUIntSize >= 64
                /// Library format: { UI2, UI2, UI4, UI8 }
                return size + sizeof (Library) + * (UI2_ptr + 2) * 
                       sizeof (byte) + * (UI4_ptr + 4) + * (UI8_ptr + 8);
                #else
                return 0;
                #endif
        }
        return 0;
    }

    /** Shrinks the currently selected bag's buffer to the min size. */
    void Shrink () {

    }
    
    /** Handles Script Commands.
        @param text     Beginning of the Text buffer. 
        @param strand_end End of the Text buffer.
        @return Returns nil upon success and an error string upon failure. */
    virtual const char* Sudo (const char* text, const char* strand_end) {
        return nullptr;
    }

    /** Abstract Script Op(s).
        @param index The index of the expression.
        @param expr  The Expr to read and write from.
        @return      Returns nil upon success, a Set header upon query, and an 
                     error_t ticket upon Read-Write failure. */
    virtual const Op* Star (wchar_t index, Expr* expr) {
        static const Op kThis = { "Library",
            OpFirst ('A'), OpLast ('A'),
            "", kOpOperand, 0 };

        switch (index) {
            case '?': return ExprEnquiry (expr, kThis);
            case 'A': {
                static const Op This = { "Foo",
                    Params<0> (), Params<0> (),
                    "Foo is getting old I know.", kOpOperation, 0 };
                return 0;
            }
            default:
                return 0;
        }
        return 0;
    }

    private:

    //NONCOPYABLE (Library)

    int reserved;           //< Reserved for 64-bit memory alignment.
    Library** root_;        //< Pointer to the dynamically allocated bags.
    Library* bag_;          //< The currently selected bag.
    uint_t index_,          //< The index of the currently selected bag.
        height_,      //< The number of bags on the stack.
        num_libraries_;     //< The number of libraries.
    byte type_;             //< The current type of bag.
    //Bag<TIndex, TKey, TData, TData> bag;
    TIndex num_keys_,       //< The current number of Star members.
        buffer_size_;       //< The current size of the header and names buffer in bytes.
    TKey header_size_,      //< The current size of the header and names in bytes.
        collisions_size_;   //< The current size of the header and names buffer in bytes.
    TData data_size_;       //< The current total size of the bag.
};

/** Destructs the given bag. */
template<typename TIndex, typename TKey, typename TData,
    uint MaxStackSize>
KABUKI void Destruct (Library<TIndex, TKey, TData, TData, MaxStackSize>* r) {
    if (r == nullptr) return;
    delete reinterpret_cast<char*> (r);
}
#endif  //< MEMORY_PROFILE > 2
}       //< namespace _

#endif  //< SCRIPT_LIBRARY_H
