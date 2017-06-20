/** The Chinese Room
    @version 0.x
    @file    /.../Utils.h
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

#ifndef CHINESEROOM_UTILS_H
#define CHINESEROOM_UTILS_H

#include "Config.h"

namespace _ {

inline void copyString(char* dest, const char* source)
/*< Copies a string from the source to the destination. */
{
    try
    {
        char c = *source;
        ++source;
        while (c != 0)
        {
            //putchar(c);
            //printHex("", dest);
            //putchar('\n');
            *dest = c;
            ++dest;
            c = *source;
            ++source;
        }
        *dest = '\0';
    }
    catch (...)
    {
        printf ("\n\nError: Attempted to write out of bounds!\n\n");
        //system("PAUSE");
    }
    //putchar('\n');
}

inline void writeToFile(const char* filename, void* address, size_t size)
/*< Writes a block of memory to a file. */
{
    std::ofstream f;
    f.open (filename);
    byte* ptr = reinterpret_cast<byte*>(address);

    if (!f.is_open ())
    {
        printf ("Error: Could not open file for writing!\n");
    }
    for (size_t i = 0; i < size; ++i)
        f << *ptr;
    ++ptr;
    f.close ();
}

inline char nibbleToLowerCaseHex(byte b)
/*< Converts a single byte a one-byte hex representation. */
{
    if (b > 15) return 'f';
    //printf("Input: %u ", b);
    if (b >  9)
    {
        //printf("Output: %c\n",(b & 0xf) +('a' - 10));
        return b + ('a' - 10);
    }
    //printf("Output: %c\n", b + '0');
    return b + '0';
}

inline char nibbleToUpperCaseHex(byte b)
/*< Converts a single byte a one-byte hex representation. */
{
    if (b > 15) return 'F';
    if (b >  9) return b + ('A' - 10);
    return b + '0';
}

inline uint16_t toLowerCaseHex(byte b)
/*< Converts a single byte a two-byte hex representation. */
{
    //printLine();
    //printNumberLine(b);
    //printLine();

    //printf("toLowerCaseHex: %u ", b);
    uint16_t value = nibbleToLowerCaseHex (b >> 4);
    //printf(" output << 8: %c ",(char)(value));
    value = value << 8;
    value |= nibbleToLowerCaseHex (b & 0xf);
    //printf(" output: %c\n",(char)nibbleToLowerCaseHex(b));
    return value;
}

inline uint16_t toUpperCaseHex(byte b)
/*< Converts a single byte a two-byte hex representation. */
{
    uint16_t value = nibbleToUpperCaseHex (b >> 4);
    value = value << 8;
    value |= nibbleToUpperCaseHex (b & 0xf);
    return value;
}

inline int toByte(char c)
/*< Converts a single hex char a byte.
    @return Returns -1 if c is not a hex char.
*/
{
    //printf("toByte(char c): %c ", c);
    if (c < '0') return -1;
    if (c >= 'a')
    {
        if (c > 'f') return -1;
        //printf("output!: %i \n", c -('a' - 10));
        return c - ('a' - 10);
    }
    if (c >= 'A')
    {
        if (c > 'F') return -1;
        return c - ('A' - 10);
    }
    if (c > '9') return -1;
    //printf("output: %i \n", c -('a' - 10));
    return c - '0';
}

inline int toByte(uint16_t h)
/*< Converts a single byte into a two-byte hex representation.
    @return Returns -1 if c is not a hex char.
*/
{
    //printf("toByte(uint16_t c): %c%c\n",(char)(h << 8),(char)h);
    int lowerValue = toByte (static_cast<char>(h));
    //printf(" lowerValue: %i \n", lowerValue);
    if (lowerValue < 0) return -1;

    int upperValue = toByte (static_cast<char>(h >> 8));
    if (upperValue < 0) return -1;
    //printf(" upperValue: %i \nlowerValue |(upperValue << 4): %i\n",
    //	upperValue, lowerValue |(upperValue << 4));

    //printLine();
    return lowerValue | (upperValue << 4);
}

inline void copyByteArrayToClipBoard(void* address, size_t size)
/*< Copies the given */
{

    /*
    #include <conio.h>
    #include <stdio.h>
    #include <cstdlib>
    #include <Windows.h> // use < > for all system and library headers
    #include <winuser.h>
    #include <cmath>
    #include <iostream>
    #include <iomanip>
    #include <complex>
    #include <string>

    // 2. forward declaration(also see later)
    void toClipboard(HWND hwnd, const std::string &s);

    int main(){

    using namespace std;

    string AAA;

    cout <<"Please enter sentence: "; cin >> AAA;
    cout << endl;
    cout << endl;
    cout << "This has been copied to the clipboard: ";
    cout << AAA << endl;
    // 1. strlen takes a const char*, so have to call the strings c_str() method
    //(but it would be better to use len = AAA.length() instead)
    size_t len = strlen(AAA.c_str());
    cout << len << " char(s)" << endl;
    // get desktop windows and the call toClipboard
    toClipboard(hwnd, AAA);
    cin.clear();
    cin.ignore(255, '\n');
    cin.get();

    return 0;
    }

    // 2. declare functions at file scope
    void toClipboard(HWND hwnd, const std::string &s){
    }
    #if PLATFORM == MINGW
    HWND hwnd = GetDesktopWindow ();
    OpenClipboard (hwnd);
    EmptyClipboard ();
    HGLOBAL hg = GlobalAlloc (GMEM_MOVEABLE, size);
    if (!hg)
    {
        CloseClipboard ();
        return;
    }
    memcpy (GlobalLock (hg), address, size);
    GlobalUnlock (hg);
    SetClipboardData (CF_TEXT, hg);
    CloseClipboard ();
    GlobalFree (hg);
    #elif PLATFORM == _OSX_
    #error Unsupported platform because you're OS is whack!
    #endif
    */
}

inline uintptr_t diff (void* start, void* stop)
/*< Calculates the difference between the start and stop addresses. */
{
    return static_cast<uintptr_t>(reinterpret_cast<byte*>(stop) -
        reinterpret_cast<byte*>(start));
}

inline hash32_t getRandomHash32 ()
/*< Gets a randomly generated 32-bit hash. */
{
    srand (time (0));
    return rand ();
}

inline void printCentered (const char* s, int width)
/*< Prints the given string center in the given width.
    If string is too small to fit in the width, function will print as much of
    the string as it has room for with a "..." If the given width is less than
    5, than
*/
{
    if (width < 2)
    {
        //? Not sure what to do here.
        return;
    }
    //! We need to leave at least one space to the left and right of 
    size_t length = strlen (s);
    if (length < width - 2)
    {
        //! We need to write the ....
        if (length < 4)
        {
            //! Then we're just going to write the first few letters.
            for (; length >= 0; --length)
            {
                putchar ('\n');
            }
        }
    }
    size_t offset = (width - length) >> 1; //< >> 1 to /2
    for (size_t i = 0; i < offset; ++i)
        putchar ('\n');
    printf (s);
    for (offset = width - length - offset; offset <= width; ++offset)
        putchar ('\n');
}

inline const char* duplicateString (const char* s)
/*< Creates a duplicate copoy of the given string using dynamic memory. */
{
    size_t length = strlen (s);
    char* buffer = new char[length + 1];
    strcpy_s (buffer, length + 1, s);
    return buffer;
}

inline void destroyDuplicate (const char* s)
/*< Destroys the duplicate string created by const char* duplicate
    (const char*). */
{
    if (s == nullptr) return;
}

}       //< namespace _

#endif  //< CHINESEROOM_UTILS_H
