/** Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    //tests/script_itos_benchmark.cc
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

#if SEAM >= SEAM_0_0_0__01
#include "../../script2/ttest.h"

#include <chrono>
#include <fstream>
#include <random>

using namespace _;
using namespace std;
using namespace std::chrono;

uint32_t BSR(uint32_t value) {
  //#if defined(_MSC_VER) && defined(_M_AMD64)
  //  __asm("BSR ");
  //#else
  //  __asm("BSR ");
  //#endif
  return 0;
}

inline uint32_t BSRLoop(uint32_t n) {
  SI4 shift = 31;
  uint32_t shift_count = 31;
  while (shift-- > 0) {
    if (n >> shift_count--) return shift_count + 1;
  }
  return 32;
}

inline uint32_t BSRHack(uint32_t n) {
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  n |= (n >> 16);
  return n - (n >> 1);
}

inline uint64_t BSRHack(uint64_t n) {
  n |= (n >> 1);
  n |= (n >> 2);
  n |= (n >> 4);
  n |= (n >> 8);
  n |= (n >> 16);
  n |= (n >> 32);
  return n - (n >> 1);
}

void BenchmarkBSR() {
  enum {
    kLoopCount = 1000 * 1000 * 1000,
    kSize = 24,
  };

  static const CH1 kBenchmarkHeader[] =
      "\nReverse Scan Bit Benchmark,,,"
      "\nnil, rsb32, manual32, rsb64, manual64";

  auto start = high_resolution_clock::now(),
       stop = high_resolution_clock::now();
  auto delta = duration_cast<milliseconds>(stop - start).count();

  std::random_device rd;
  std::mt19937_64 eng(rd());
  std::uniform_int_distribution<uint32_t> random_ui4;

  double nil_time, cpu_times[5], percent_faster;

  printf("\n\nBenchmarking RBS vs Bit-Twiddling Hack...\n\n");

  ofstream file("0_0_benchmark_rsb.csv");

  if (!file.is_open()) {
    Print("Unable to open file");
    return;
  }

  Print(kBenchmarkHeader);
  file << kBenchmarkHeader;

  uint32_t spinner4 = 0;

  start = high_resolution_clock::now();

  // Nil measurement

  for (uint32_t j = kLoopCount; j > 0; --j) {
    for (uint32_t k = 0; k < 32; ++k) {
      spinner4 += random_ui4(eng);
    }
  }
  stop = high_resolution_clock::now();
  delta = duration_cast<milliseconds>(stop - start).count();
  nil_time = (double)delta;
  file << delta << ',';
  COut(delta).Print(',');

  start = high_resolution_clock::now();
  for (uint32_t j = kLoopCount; j > 0; --j) {
    for (uint32_t k = 0; k < 32; ++k) {
      spinner4 += BSR(random_ui4(eng));
    }
  }
  stop = high_resolution_clock::now();
  delta = duration_cast<milliseconds>(stop - start).count();
  cpu_times[1] = (double)delta;
  file << delta << ',';
  COut(delta).Print(',');

  start = high_resolution_clock::now();
  for (uint32_t j = kLoopCount; j > 0; --j) {
    spinner4 += BSRHack(random_ui4(eng));
  }
  stop = high_resolution_clock::now();
  delta = duration_cast<milliseconds>(stop - start).count();
  cpu_times[2] = (double)delta;
  file << delta << ',';
  COut(delta).Print(',');

  start = high_resolution_clock::now();
  for (SI4 count = kLoopCount; count > 0; --count) {
    spinner4 += BSRLoop(random_ui4(eng));
  }
  stop = high_resolution_clock::now();
  delta = duration_cast<milliseconds>(stop - start).count();
  cpu_times[3] = (double)delta;
  file << delta << ',';
  COut(delta).Print(',');

  double percent_faster =
      ((cpu_times[2] - nil_time) / (cpu_times[3] - nil_time)) - 1.0;
  file << percent_faster << '\n';
  COut(percent_faster);
  file.close();
}

TestCase TestMSbAsserted() {
  BenchmarkBSR();
  Pause("\n");
  return 0;
}

#endif  //< #if SEAM >= SEAM_0_0_0__01
