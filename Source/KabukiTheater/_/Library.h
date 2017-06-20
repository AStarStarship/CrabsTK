/** The Chinese Room
    @version 0.x
    @file    /.../Source/ChineseRoom\Book.h
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

#ifndef CHINESEROOM_LIBRARY_H
#define CHINESEROOM_LIBRARY_H

#include "Config.h"
#include "IDevice.h"

namespace _ {

template<typename TIndex, typename THeader, typename TData, typename THash, uint_t MaxStackHeight>
class Library: public IDevice
{
    public:

    Library()
    /*< Constructs an empty dictionary. */
    :   numMembers (0),
        numCollisions (0),
        headerSize (0),
        collisionsSize (0),
        dataSize (0)
    {

    }

    bool hasHashTable()
    /*< Returns true if the current Book has a hash table. */
    {
        return collisionsSize != 0;
    }

    uint_t getBookType ()
    /*< Gets the number of keys in the current scope. */
    {
        return BK2template<typename TIndex, typename THeader, typename TData>
    }

    uint_t getNumKeys ()
    /*< Gets the number of keys in the current scope. */
    {
        return numMembers;
    }

    uint_t getHeaderSize ()
    /*< Gets the size of the header in bytes. */
    {
        return headerSize;
    }

    uint_t getDataSize
    /*< Gets the data size in bytes. */
    {
        return dataSize;
    }

    ticket_t add(byte type, const char* key, void* data)
    /*< Attempts to add the Page data into the Object at the given BaseAddress.
        @return Returns nullptr upon success and an error string upon failure. */
    {
        TIndex sizeOfTypeToAdd = getSizeOfType (type);
        if (sizeOfTypeToAdd == 0)
        {
            return 0;
        }
        return 0;
    }
     
    ticket_t insert(byte type, const char* key, void* data, TIndex index = 0)
    /*< Attempts to insert the Page data into the Object at the given index.
        @return Returns nullptr upon success and an error string upon failure. */
    {
        TIndex l_numMembers = numNumbers;
        if (index > l_numMembers) index = l_numMembers;

        return 0;
    }

    ticket_t remove(TIndex index)
    /*< Attempts to remove the Page data into the Object at the given index.
        @return Returns nullptr upon success and an error string upon failure. */
    {
        return 0;
    }

    ticket_t clear(TIndex index)
    /*< Attempts to clear the page at the given index.
        @return Returns nullptr upon success and an error string upon failure. */
    {
        return 0;
    }

    TIndex find(const char* key)
    /*< Attempts to find the given member name.
        @return Returns an invalid index upon failure. */
    {
        return 0;
    }

    bool search(const char* query, Library<TIndex, THeader, TData, THash, MaxStackHeight>* results)
    /*< Searches for the given query and returns a book of query results.  */
    {
        return false;
    }

    byte getMemberSize(TIndex index)
    /*< gets the size of the item at the given index. */
    {
        return 0;
    }

    byte setMemberSize(TIndex index, TData newSize)
    /*< gets the size of the item at the given index. */
    {
        return 0;
    }

    void* getDataAddress(TIndex index)
    /** Returns the data address of the given member if it exists.
        @return Returns a pointer to one of the ChineseRoom error strings upon failure. */
    {
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
                /// Library format: { UI1, UI1, UI2, UI2 }
                return size + sizeof (Library) + *(address + 1) * sizeof (byte) + *(UI2_ptr + 2) + *(UI8_ptr + 4);
                #else
                return 0;
                #endif
            case BK4:
                #if _BufferUIntSize >= 32
                /// Library format: { UI1, UI1, UI2, UI4 }
                return size + sizeof (Book32) + *(UI2_ptr + 2) * sizeof (byte) + *(UI4_ptr + 4) + *(UI8_ptr + 8);
                #else
                return 0;
                #endif
            case BK8:
                #if _BufferUIntSize >= 64
                /// Library format: { UI2, UI2, UI4, UI8 }
                return size + sizeof (Library) + *(UI2_ptr + 2) * sizeof (byte) + *(UI4_ptr + 4) + *(UI8_ptr + 8);
                #else
                return 0;
                #endif
        }
        return 0;
    }

    void shrink()
    /*< Shrinks the currently selected book's buffer to the min size. */
    {

    }

    const Member<T>* op(Terminal* io, byte index) override
    /*< ChineseRoom Operations. */
    {
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

    NONCOPYABLE(Library)

    int reserved;           //< Reserved for 64-bit memory alignment.
    Library** root;         //< Pointer to the dynamically allocated books.
    Library* book;          //< The currently selected book.
    uint_t currentIndex,    //< The index of the currently selected book.
        numBooks,           //< The number of books on the stack.
        numLibraries;       //< The number of libraries.
    byte currentType;       //< The current type of book.
    //Book<TIndex, THeader, TData, THash> book;
    TIndex numKeys,      //< The current number of IDevice members.
        bufferSize;         //< The current size of the header and names buffer in bytes.
    THeader headerSize,  //< The current size of the header and names in bytes.
        collisionsSize;     //< The current size of the header and names buffer in bytes.
    TData dataSize;      //< The current total size of the book.
};

template<typename TIndex, typename THeader, typename TData, typename THash, 
    uint MaxStackSize>
inline void destruct(Library<TIndex, THeader, TData, THash, MaxStackSize>* r)
/*< Destructs the given book. */
{
    if (r == nullptr) return;
    delete reinterpret_cast<byte*>(r);
}

}       //< namespace _

#endif  //< CHINESEROOM_LIBRARY_H
