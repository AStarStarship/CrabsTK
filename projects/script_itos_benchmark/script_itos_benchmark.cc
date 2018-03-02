/** Kabuki Toolkit
    @version 0.x
    @file    ~/tests/script_itos_benchmark.cc
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
#include "script_itos_benchmark.h"
#include "../../source/crabs/print_itos.h"

typedef unsigned int uint;

using namespace std;
using namespace std::chrono;

inline void PrintBinary (uint32_t value) {
    enum { kSize = sizeof (uint32_t) * 8 };

    for (int i = kSize; i > 0; --i) {
        char c = (char)('0' + (value >> (kSize - 1)));
        putchar (c);
        value = value << 1;
    }
}

void BenchmarkScriptItos () {
    enum {
        kNumTests         = 500 * 1000 * 1000,
        kSize             = 24,
    };

    //static const char kBenchmarkHeader[] = "\nScript ItoS Benchmarks,,,,"
    //                                       "\n#Bits, Null, sprintf_s, Mod10, "
    //                                       "Mod100, Script, % Faster\n";
    static const char kBenchmarkHeader[] = "\nScript ItoS Benchmarks,,,,"
                                           "\n#Bits, Null, Mod10, "
                                           "Mod100, Script, % Faster\n";
    char     text[kSize],
             expecting[kSize];
    char   * result;
    uint32_t value;
    uint32_t bits_mask = ~(uint32_t)0,
             value_mask;
    int      count;
    auto start = high_resolution_clock::now (),
         stop  = high_resolution_clock::now ();
    auto delta = duration_cast<milliseconds>
                 (stop - start).count ();

    std::random_device rd;
    std::mt19937_64 eng (rd ());
    std::uniform_int_distribution<uint32_t> distr;

    double nil_time,
             cpu_times[5];

    printf ("\n\nTesting random numbers...\n\n");

    for (int i = 0; i < kNumTests / 10; ++i) {
        value = distr (eng);
        sprintf_s (expecting, kSize, "%u", value);
        result = _::Print (value, text, text + kSize - 1);
        if (!result) {
            printf ("\nAn error occurred in Print :-(\n");
            system ("PAUSE");
            break;
        }
        *result = 0;
        if (strcmp (expecting, text)) {
            printf ("\n\nERROR in Print: Expecting \"%s\":%u and found \"%s\":%u",
                    expecting, (uint)strlen (expecting), text,
                    (uint)strlen (text));
            putchar ('\n');
            system ("PAUSE");
        }

        result = PrintMod10 (value, text, text + kSize);
        if (!result) {
            printf ("\nAn error occurred in PrintMod10 :-(\n");
            system ("PAUSE");
            break;
        }
        *result = 0;
        if (strcmp (expecting, text)) {
            printf ("\n\nERRORin PrintMod10: Expecting \"%s\":%u and found \"%s\":%u",
                    expecting, (uint)strlen (expecting), text,
                    (uint)strlen (text));
            putchar ('\n');
            system ("PAUSE");
        }

        result = PrintMod100 (value, text, text + kSize);
        if (!result) {
            printf ("\n%i.) Expecting \"%s\":%u", i + 1, expecting,
                    (uint)strlen (expecting));
            printf ("\nAn error occurred in PrintMod100:-(\n");
            system ("PAUSE");
            break;
        }
        *result = 0;
        if (strcmp (expecting, text)) {
            printf ("\n%i.) Expecting \"%s\":%u", i + 1, expecting,
                    (uint)strlen (expecting));
            printf ("\n\nERROR in PrintMod100: Expecting \"%s\":%u and found \"%s\":%u",
                    expecting, (uint)strlen (expecting), text,
                    (uint)strlen (text));
            putchar ('\n');
            system ("PAUSE");
        }
    }
    
    printf ("\n\nBenchmarking ItoS algorithms...\n\n");

    ofstream file ("script_itos_benchmarks.csv");

    if (!file.is_open ()) {
        cout << "Unable to open file";
        return;
    }
    
    // Start weird fix for timer problem with 1 bit.
    start = high_resolution_clock::now ();
    for (count = kNumTests; count > 0; --count) {
        result = PrintNil (0, text, text + kSize);
    }
    stop = high_resolution_clock::now ();
    delta = duration_cast<milliseconds> (stop - start).count ();
    // End weird fix.

    cout << kBenchmarkHeader;
    file << kBenchmarkHeader;
    for (int num_bits = 1; num_bits <= 32; ++num_bits) {
        bits_mask = bits_mask << 1;
        value_mask = ~bits_mask;
        file << num_bits << ",";
        cout << '\n' << num_bits << ".) ";

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintNil (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        nil_time = (double)delta;
        file << delta << ',';
        cout << delta << ',';
        /*
        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintSprintf (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        cpu_times[0] = (double)delta;
        file << delta << ',';
        cout << delta << ',';*/

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintMod10 (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        cpu_times[1] = (double)delta;
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = PrintMod100 (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        cpu_times[2] = (double)delta;
        file << delta << ',';
        cout << delta << ',';

        start = high_resolution_clock::now ();
        for (count = kNumTests; count > 0; --count) {
            value = distr (eng) & value_mask;
            result = _::Print (value, text, text + kSize);
        }
        stop = high_resolution_clock::now ();
        delta = duration_cast<milliseconds> (stop - start).count ();
        cpu_times[3] = (double)delta;
        file << delta << ',';
        cout << delta << ',';
        double percent_faster = ((cpu_times[2] - nil_time) / (cpu_times[3] - nil_time)) - 1.0;
        file << percent_faster << '\n';
        cout << percent_faster;
    }
    file.close ();
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
    std::cout << " }; \n\n", digits;
    system ("PAUSE");
}

int main() {
    BenchmarkScriptItos ();
    putchar ('\n');
    system ("PAUSE");
    return 0;
}
