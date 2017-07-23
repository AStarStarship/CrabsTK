/** The Chinese Room
    @version 0.x
    @file    \...\dictionary.h
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

#ifndef CHINESEROOM_DICTIONARY_H
#define CHINESEROOM_DICTIONARY_H

#include "device.h"
#include "book.h"

namespace _ {

template<typename TIndex, typename TKey, typename TData, typename THash, 
    uint_t MaxStackHeight>
class Dictionary: public Device
{
    public:

    /** Constructs an empty dictionary. */
    Dictionary ()
    :   num_members_ (0),
        num_collisions_ (0),
        header_size_ (0),
        collisions_size_ (0),
        data_size_ (0) {

    }

    /** Returns true if the current Book has a hash table. */
    bool HasHashTable () {
        return collisions_size_ != 0;
    }

    /** Gets the number of keys in the current scope. */
    uint_t GetBookType () {
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
    uint_t GetDataSize {
        return data_size_;
    }
    
    /** Attempts to add the Page data into the Object at the given BaseAddress.
        @return Returns nullptr upon success and an error string upon failure. */
    ticket_t Add (byte type, const char* key, void* data) {
        TIndex size_of_type = getSizeOfType (type);
        if (size_of_type == 0)
        {
            return 0;
        }
        return 0;
    }
     
    /** Attempts to insert the Page data into the Object at the given index.
        @return Returns nullptr upon success and an error string upon failure. */
    ticket_t Insert (byte type, const char* key, void* data, TIndex index = 0) {
        TIndex l_numMembers = numNumbers;
        if (index > l_numMembers) index = l_numMembers;

        return 0;
    }

    /** Attempts to remove the Page data into the Object at the given index.
        @return Returns nullptr upon success and an error string upon failure. */
    ticket_t Remove (TIndex index) {
        return 0;
    }

    /** Attempts to clear the page at the given index.
        @return Returns nullptr upon success and an error string upon failure. */
    ticket_t Clear (TIndex index) {
        return 0;
    }
    
    /** Attempts to find the given member name.
        @return Returns an invalid index upon failure. */
    TIndex Find (const char* key) {
        return 0;
    }

    /** Searches for the given query and returns a book of query results.  */
    bool Search (const char* query, Dictionary<TIndex, TKey, TData, THash, 
        MaxStackHeight>* results) {
        return false;
    }

    /** gets the size of the item at the given index. */
    byte GetMemberSize (TIndex index) {
        return 0;
    }

    /** gets the size of the item at the given index. */
    byte SetMemberSize (TIndex index, TData newSize) {
        return 0;
    }
    
    /** Returns the data address of the given member if it exists.
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
            case BK2:
                #if _BufferUIntSize >= 16
                /// Dictionary format: { UI1, UI1, UI2, UI2 }
                return size + sizeof (Dictionary) + * (address + 1) * 
                       sizeof (byte) + *(UI2_ptr + 2) + * (UI8_ptr + 4);
                #else
                return 0;
                #endif
            case BK4:
                #if _BufferUIntSize >= 32
                /// Dictionary format: { UI1, UI1, UI2, UI4 }
                return size + sizeof (Book32) + * (UI2_ptr + 2) * 
                       sizeof (byte) + * (UI4_ptr + 4) + * (UI8_ptr + 8);
                #else
                return 0;
                #endif
            case BK8:
                #if _BufferUIntSize >= 64
                /// Dictionary format: { UI2, UI2, UI4, UI8 }
                return size + sizeof (Dictionary) + * (UI2_ptr + 2) * 
                       sizeof (byte) + * (UI4_ptr + 4) + * (UI8_ptr + 8);
                #else
                return 0;
                #endif
        }
        return 0;
    }

    /** Shrinks the currently selected book's buffer to the min size. */
    void Shrink () {

    }

    /** ChineseRoom Operations. */
    const Member* Op (Rx* rx, Tx& tx, char index) override {
        switch (index)
        {
            case 0:
                return 0;
            default:
                return 0;
        }
        return 0;
    }

    private:

    //NONCOPYABLE (Dictionary)

    int reserved;           //< Reserved for 64-bit memory alignment.
    Dictionary** root_;     //< Pointer to the dynamically allocated books.
    Dictionary* book_;      //< The currently selected book.
    uint_t index_,          //< The index of the currently selected book.
        stack_height_,      //< The number of books on the stack.
        num_libraries_;     //< The number of libraries.
    byte type_;             //< The current type of book.
    //Book<TIndex, TKey, TData, THash> book;
    TIndex num_keys_,       //< The current number of Device members.
        buffer_size_;       //< The current size of the header and names buffer in bytes.
    TKey header_size_,   //< The current size of the header and names in bytes.
        collisions_size_;   //< The current size of the header and names buffer in bytes.
    TData data_size_;       //< The current total size of the book.
};

/** Destructs the given book. */
template<typename TIndex, typename TKey, typename TData, typename THash, 
    uint MaxStackSize>
inline void destruct (Dictionary<TIndex, TKey, TData, THash, MaxStackSize>* r) {
    if (r == nullptr) return;
    delete reinterpret_cast<byte*> (r);
}

}       //< namespace _

#endif  //< CHINESEROOM_DICTIONARY_H
