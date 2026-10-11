/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /project/script2_benchmark/00_foo.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if SEAM == BENCHMARK_SCRIPT2_0
#include "test_debug.inl"
#else
#include "test_release.inl"
#endif

using namespace _;

namespace kabuki { namespace script2_benchmark { 
inline const CH1* _0_Foo (CH1* seam_log, CH1* seam_end, const CH1* args) {
#if SEAM >= BENCHMARK_SCRIPT2_0
  TEST_BEGIN;
  
  PRINT_HEADING ("Testing putchar vs stdout");
    std::chrono::time_point<std::chrono::system_clock> start,
        end;
    start = std::chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i) 
        putchar ('.');
    end = std::chrono::system_clock::now ();

    std::chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = std::chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        printf (".");
    end = std::chrono::system_clock::now ();

    std::chrono::duration<double> elapsed_seconds_printf = end - start;

    start = std::chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        std::cout << ".";
    end = std::chrono::system_clock::now ();

    std::chrono::duration<double> elapsed_seconds_cout = end - start;

    cout << "\n\nputchar vs printf vs cout benchmark\n\n"
        "512 * 1024 single CH1 calls:\n"
        "putchar (CH1) calls: " <<
        elapsed_seconds_putchar.count () << "s\n" <<
        "printf (const CH1*) calls: " <<
        elapsed_seconds_printf.count () << "s\n" <<
        "cout << const CH1* calls: " <<
        elapsed_seconds_cout.count () << "s\n";

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i) {
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
    }
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        printf ("..........");

    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        printf ("..........");
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    cout << "\n512 * 1024 * 10 chars calls:\n"
        "putchar (CH1) calls: " <<
        elapsed_seconds_putchar.count () << "s\n" <<
        "printf (const CH1*) calls: " <<
        elapsed_seconds_printf.count () << "s\n" <<
        "cout << const CH1* calls: " <<
        elapsed_seconds_cout.count () << "s\n";

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        printf ("..........");

    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        printf ("%i%i%f%f", GetRandom);
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (SI4 i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    cout << "\n512 * 1024 * 10 chars calls:\n"
        "putchar (CH1) calls: " <<
        elapsed_seconds_putchar.count () << "s\n" <<
        "printf (const CH1*) calls: " <<
        elapsed_seconds_printf.count () << "s\n" <<
        "cout << const CH1* calls: " <<
        elapsed_seconds_cout.count () << "s\n";

#endif
  return 0;
}
}  //< namespace script2_benchmark
}  //< namespace kabuki
