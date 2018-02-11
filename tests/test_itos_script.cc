/** Kabuki Starship
    @version 0.x
    @file    ~/source/kabuki/tests/script_itos_tests.cc
    @author  Cale McCollough <calemccollough.github.io>
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
#include "test_itos_script.h"
#include "test_itos_sizelut.h"
#include "../source/crabs/itos.h"

using namespace _;
using namespace std;
using namespace std::chrono;

#define TIMING

#ifdef TIMING
#define STOP_TIMER(delimiter)\
    cout << duration_cast<milliseconds>\
                     (high_resolution_clock::now()-start\
    ).count() << delimiter; 
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(delimiter)
#endif

#define PRINT_PAUSE(message)\
    cout << '\n' << message << "               "; system ("PAUSE");

void PrintBinary (uint64_t value) {
    for (int i = 0; i < 64; ++i) {
        cout << (char)('0' + (value >> 63));
        value = value << 1;
    }
}

void TestItoSScript () {
    static const uint64_t k10ToThe[20] = {
        1,//< 10^0
        10,//< 10^1
        100,//< 10^2
        1000,//< 10^3
        10000,//< 10^4
        100000,//< 10^5
        1000000,//< 10^6
        10000000,//< 10^7
        100000000,//< 10^8
        1000000000,//< 10^9
        10000000000,//< 10^10
        100000000000,//< 10^11
        1000000000000,//< 10^12
        10000000000000,//< 10^13
        100000000000000,//< 10^14
        1000000000000000,//< 10^15
        10000000000000000,//< 10^16
        100000000000000000,//< 10^17
        1000000000000000000,//< 10^18
        10000000000000000000,//< 10^19
    };

    static const uint64_t test_value[] = {
        k10ToThe[0] - 1, k10ToThe[0], k10ToThe[0] + 1,
        k10ToThe[1] - 1, k10ToThe[1], k10ToThe[1] + 1,
        k10ToThe[1] + k10ToThe[1] / 2,
        k10ToThe[2] - 1, k10ToThe[2], k10ToThe[2] + 1,
        k10ToThe[2] + k10ToThe[2] / 2,
        k10ToThe[3] - 1, k10ToThe[3], k10ToThe[3] + 1,
        k10ToThe[3] + k10ToThe[3] / 2,
        k10ToThe[4] - 1, k10ToThe[4], k10ToThe[4] + 1,
        k10ToThe[4] + k10ToThe[4] / 2,
        k10ToThe[5] - 1, k10ToThe[5], k10ToThe[5] + 1,
        k10ToThe[5] + k10ToThe[5] / 2,
        k10ToThe[6] - 1, k10ToThe[6], k10ToThe[6] + 1,
        k10ToThe[6] + k10ToThe[6] / 2,
        k10ToThe[7] - 1, k10ToThe[7], k10ToThe[7] + 1,
        k10ToThe[7] + k10ToThe[7] / 2,
        k10ToThe[8] - 1, k10ToThe[8], k10ToThe[8] + 1,
        k10ToThe[8] + k10ToThe[8] / 2,
        k10ToThe[9] - 1, k10ToThe[9], k10ToThe[9] + 1,
        k10ToThe[9] + k10ToThe[9] / 2,
        k10ToThe[10] - 1, k10ToThe[10], k10ToThe[10] + 1,
        k10ToThe[10] + k10ToThe[10] / 2,
        k10ToThe[11] - 1, k10ToThe[11], k10ToThe[11] + 1,
        k10ToThe[11] + k10ToThe[11] / 2,
        k10ToThe[12] - 1, k10ToThe[12], k10ToThe[12] + 1,
        k10ToThe[12] + k10ToThe[12] / 2,
        k10ToThe[13] - 1, k10ToThe[13], k10ToThe[13] + 1,
        k10ToThe[13] + k10ToThe[13] / 2,
        k10ToThe[14] - 1, k10ToThe[14], k10ToThe[14] + 1,
        k10ToThe[14] + k10ToThe[14] / 2,
        k10ToThe[15] - 1, k10ToThe[15], k10ToThe[15] + 1,
        k10ToThe[15] + k10ToThe[15] / 2,
        k10ToThe[16] - 1, k10ToThe[16], k10ToThe[16] + 1,
        k10ToThe[16] + k10ToThe[16] / 2,
        k10ToThe[17] - 1, k10ToThe[17], k10ToThe[17] + 1,
        k10ToThe[17] + k10ToThe[17] / 2,
        k10ToThe[18] - 1, k10ToThe[18], k10ToThe[18] + 1,
        k10ToThe[18] + k10ToThe[18] / 2,
        k10ToThe[19] - 1, k10ToThe[19], k10ToThe[19] + 1,
        k10ToThe[19] + k10ToThe[19] / 2
    };

    static const uint64_t problem_childs[] = {
        10277856742680,
        16733192600430,
        386547074382,
    };
    enum { kSize = 24 };

    char     text[kSize],
        expecting[kSize];
    //found[kSize];
    char*    result;
    uint64_t value;
    std::random_device rd;
    std::mt19937_64 eng (rd ());
    std::uniform_int_distribution<unsigned long long> distr;

    cout << "\n\nTesting Script ItoS Algorithm...\n\n";

    //PrintDigits99To99Lut ();

    cout << "\n\nTesting Problem Childs...\n\n";
    for (int i = 0; i < 2; ++i) {
        value = problem_childs[i];
        for (unsigned j = 0; j < 2; ++j) {

            // Write a bunches of xxxxxx to the buffer for debug purposes.
            result = text;
            for (int k = 24; k > 0; --k) {
                *result++ = 'x';
            }
            *(text + 23) = 0;

            sprintf_s (expecting, 24, "%I64u", value);
            cout << '\n' << i + 1 << ".) Expecting " << expecting << " with length "
                << strlen (expecting);
            PrintBinary (value);
            result = Print (value, text + j, text + kSize);
            if (!result) {
                PRINT_PAUSE ("An error occurred :-(")
                    break;
            }
            if (strcmp (expecting, text + j)) {
                cout << "\n\nERROR: Expecting \"" << expecting << "\":"
                    << strlen (expecting) << " and found \""
                    << text + j << "\":" << strlen (text + j);
                PRINT_PAUSE ("\n");
            }
        }
    }

    cout << "\n\nTesting edge conditions...\n\n";
    for (int i = 0; i < 79; ++i) {
        value = test_value[i];
        for (unsigned j = 0; j < 2; ++j) {

            // Write a bunches of xxxxxx to the buffer for debug purposes.
            result = text;
            for (int k = 24; k > 0; --k) {
                *result++ = 'x';
            }
            *(text + 23) = 0;

            sprintf_s (expecting, 24, "%I64u", value);
            cout << '\n' << i + 1 << ".) Expecting " << expecting << " with length "
                << strlen (expecting);
            PrintBinary (value);
            result = Print (value, text + j, text + kSize);
            if (!result) {
                PRINT_PAUSE ("An error occurred :-(")
                    break;
            }
            if (strcmp (expecting, text + j)) {
                cout << "\n\nERROR: Expecting \"" << expecting << "\":"
                    << strlen (expecting) << " and found \""
                    << text + j << "\":" << strlen (text + j);
                PRINT_PAUSE ("\n");
            }
        }
    }

    cout << "\n\nTesting random numbers...\n\n";

    for (int i = 0; i < 0x00ffffff; ++i) {
        for (unsigned j = 0; j < 2; ++j) {
            // Write a bunches of xxxxxx to the buffer for debug purposes.
            result = text + j;
            for (int k = kSize; k > 0; --k) {
                *result++ = 'x';
            }
            *result = 0;
            value = distr (eng);
            sprintf_s (expecting, 24, "%I64u", value);
            cout << '\n' << i + 1 << ".) Expecting " << expecting << " with length "
                << strlen (expecting);
            PrintBinary (value);
            result = Print (value, text + j, text + j + kSize);
            if (!result) {
                PRINT_PAUSE ("An error occurred :-(")
                    break;
            }
            if (strcmp (expecting, text + j)) {
                cout << "\n\nERROR: Expecting \"" << expecting << "\":"
                    << strlen (expecting) << " and found \"" << text + j << "\":"
                    << strlen (text + j);
                PRINT_PAUSE ("\n");
            }
        }
    }

    cout << "\n Done testing ItoS :)\n\n";
}

void BenchmarkItoS () {
    enum {
        kNumTests = 10 * 1000 * 1000,
        kSize = 24,
    };

    char     text[kSize];
    char   * result;
    uint64_t value;
    uint64_t bits_mask = ~(uint64_t)0,
             value_mask;
    int      count;
    auto start = high_resolution_clock::now (),
         stop  = high_resolution_clock::now ();
    auto delta = duration_cast<milliseconds>
                 (stop - start).count ();

    std::random_device rd;
    std::mt19937_64 eng (rd ());
    std::uniform_int_distribution<unsigned long long> distr;
    ofstream file ("itos_benchmark.csv");

    if (!file.is_open ()) {
        cout << "Unable to open file";
        return;
    }
    cout << "\nAligned and Unaligned ItoS Benchmarks";
    file << "\nAligned and Unaligned ItoS Benchmarks";
    cout << "\n#Bits, Null, Size LUT, Script\n";
    file << "#Bits, Null, Size LUT, Script\n";
    for (int num_bits = 1; num_bits <= 64; ++num_bits) {
        bits_mask = bits_mask << 1;
        value_mask = ~bits_mask;
        file << num_bits << ",";
        cout << num_bits << ".) ";
        PrintBinary (value_mask);
        cout << '\n';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintNull (value, text, text + kSize);
            result = PrintNull (value, text + 1, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintSizeLut (value, text, text + kSize);
            result = PrintSizeLut (value, text + 1, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = Print (value, text, text + kSize);
            result = Print (value, text + 1, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << '\n';
        cout << delta << '\n';
    }
    bits_mask = ~(uint64_t)0;
    cout << "\nUnaligned ItoS Benchmarks";
    file << "\nUnaligned ItoS Benchmarks";
    cout << "\n#Bits, Null, Size LUT, Script\n";
    file << "#Bits, Null, Size LUT, Script\n";
    for (int num_bits = 1; num_bits <= 64; ++num_bits) {
        bits_mask = bits_mask << 1;
        value_mask = ~bits_mask;
        file << num_bits << ",";
        cout << num_bits << ".) ";
        PrintBinary (value_mask);
        cout << '\n';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintNull (value, text + 1, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintSizeLut (value, text + 1, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = Print (value, text + 1, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << '\n';
        cout << delta << '\n';
    }
    bits_mask = ~(uint64_t)0;
    cout << "\nAligned ItoS Benchmarks";
    file << "\nAligned ItoS Benchmarks";
    cout << "\n#Bits, Null, Size LUT, Script\n";
    file << "#Bits, Null, Size LUT, Script\n";
    for (int num_bits = 1; num_bits <= 64; ++num_bits) {
        bits_mask = bits_mask << 1;
        value_mask = ~bits_mask;
        file << num_bits << ",";
        cout << num_bits << ".) ";
        PrintBinary (value_mask);
        cout << '\n';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintNull (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintSizeLut (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = Print (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        file << delta << '\n';
        cout << delta << '\n';
    }
    file.close ();
}

char* PrintNull (uint64_t value, char* text, char* text_end) {
    char buffer[24];
    if (!text) {
        return nullptr;
    }
    if (text > text_end) {
        return nullptr;
    }
    uint16_t  * text16 = nullptr;
    *buffer = 0;
    return text;
}

void PrintDigits99To99Lut () {
    cout << "\n\nstatic const uint16_t kDigits00To99[100] = {\n    ";
    enum { kDigitsLast = ('9' << 8) | '9' };
    uint16_t digits,
             tick = 0,
             tick_count = 9;
    uint16_t tens,
        ones;
    for (tens = '0'; tens <= '9'; ++tens) {
        for (ones = '0'; ones <= '9'; ++ones) {
            digits = (ones << 8) | tens;
            if (++tick == tick_count) {
                tick = 0;
                printf ("0x%x,\n    ", digits);
            }
            else {
                printf ("0x%x, ", digits);
            }
        }
    }
    printf (" }; \n\n", digits);
    system ("PAUSE");
}

#undef PRINT_PAUSE
