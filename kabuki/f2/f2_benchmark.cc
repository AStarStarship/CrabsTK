/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/f2_benchmark.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <stdafx.h>

#if SEAM >= SEAM_0_0_02
#include "tbenchmark.h"

#include "tbinary.h"
#include "trng.h"
#include "ttest.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;
using namespace std::chrono;

namespace _ {

BenchmarkCase::BenchmarkCase(const char* name, TestCase* cases, int count)
    : name(name), cases(cases), count(count) {}

TestResult BenchmarkCase::Run(const char* args) {
  double nil_reading;
  int i;
  int columns_max;
  ASSERT(ScanUnsigned<>(args, columns_max));

  for (i = 0; i < count; ++i) {
    TestResult result = (*cases[i])(nullptr);
    Print(result.name);
    if (i < count) Print(',', ' ');
  }
  for (; i < columns_max; ++i) Print(',');
  Print();
  for (i = 0; i < count; ++i) {
    TestResult result = (*cases[i])(nullptr);
    if (i < count) Print(',', ' ');
  }
  Print();

  return TestResult();
}

int BenchmarkCase::GetCount() {}

Benchmark::Benchmark(const char* name, const char* filename,
                     BenchmarkCase* groups, int count)
    : name(name), filename(filename), groups(groups), count(count) {
  ASSERT(count > 0);
  int l_test_count_max = 0;
  for (intptr_t i = 0; i < count; ++i) {
  }
}

TestResult Benchmark::Run(const char* args) {
  std::ofstream out(filename);

  auto* coutbuf = std::cout.rdbuf();
  std::cout.rdbuf(out.rdbuf());

  for (size_t i = 1; i < count; ++i) {
    BenchmarkCase group = groups[i];
    TestResult result = group.Run(args);
    ASSERT(result.name);
    Print(result.name);
  }
  BenchmarkCase *cursor = groups, *end = groups + count - 1;
  while (cursor <= end) {
    (cursor++)->Run(args);
  }
  std::cout.rdbuf(coutbuf);
  return TestResult();
}

}  // namespace _

#endif  //< #if SEAM >= SEAM_0_0_02
