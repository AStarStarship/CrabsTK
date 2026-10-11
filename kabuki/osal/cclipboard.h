/* Kabuki Toolkit
    @file    $kabuki-toolkit/kabuki/toolkit/app/utils.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
#include <pch.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef KAUBKI_TOOLKIT_APP_CLIPBOARD
#define KAUBKI_TOOLKIT_APP_CLIPBOARD

namespace _ {

/* Writes a block of memory to a file.
@todo Move me over to Kabuki theater! */
inline void WriteToFile (const CH1* filename, void* address, size_t size) {
    std::ofstream f;
    f.open (filename);
    byte* ptr = reinterpret_cast<byte*> (address);

    if (!f.is_open ()) {
        printf ("Error: Could not open file for writing!\n");
    }
    for (size_t i = 0; i < size; ++i)
        f << *ptr;
    ++ptr;
    f.close ();
}

/* Copies the given */
inline void CopyByteArrayToClipBoard (void* address, size_t size) {

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

    // 2. forward declaration (also see later)
    void toClipboard (HWND hwnd, const std::string &s);

    using namespace std;

    SI4 main (){

        string AAA;

        cout <<"Please enter sentence: "; cin >> AAA;
        cout << endl;
        cout << endl;
        cout << "This has been copied to the clipboard: ";
        cout << AAA << endl;
        // 1. strlen takes a const byte*, so have to call the strings c_str () method
        // (but it would be better to use len = AAA.length () instead)
        size_t len = strlen (AAA.c_str ());
        cout << len << " byte (s)" << endl;
        // get desktop windows and the call toClipboard
        toClipboard (hwnd, AAA);
        cin.clear ();
        cin.ignore (255, '\n');
        cin.get ();

        return 0;
    }

    // 2. declare functions at file scope
    void toClipboard (HWND hwnd, const std::string &s){
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

}       //< namespace _
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
