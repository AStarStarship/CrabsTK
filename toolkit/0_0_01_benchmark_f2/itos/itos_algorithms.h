/** Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    //seams/0_0_benchmark_f2/itos_algoihrms.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#if SEAM >= SEAM_00_00_00__01
#ifndef INCLUDED_SCRIPT_ITOS_TESTS
#define INCLUDED_SCRIPT_ITOS_TESTS

#include "config.h"

TestCase PrintNil(const char* args);

char* PrintNil(char* begin, char* end, uint32_t value);

char* PrintSprintf(char* begin, char* end, uint32_t value);

TestCase PrintSprintf(const char* args);

char* PrintMod10(char* begin, char* end, uint32_t value);

TestCase PrintMod10(const char* args);

char* PrintMod100(char* begin, char* end, uint32_t value);

TestCase PrintMod100(const char* args);

char* PrintSprintf(char* begin, char* end, uint32_t value);

TestCase PrintSprintf(const char* args);

char* PrintStbSprintf(char* begin, char* end, uint32_t value);

TestCase PrintStbSprintf(const char* args);

char* PrintMsd10(char* begin, char* end, uint32_t value);

TestCase PrintMsd10(const char* args);

char* PrintSizeLut(char* begin, char* end, uint32_t value);

void TestItoSSizeLut();

void BenchmarkScriptItos();

#endif  //< #ifndef INCLUDED_SCRIPT_ITOS_TESTS
#endif  //< #if SEAM >= SEAM_00_00_00__01
