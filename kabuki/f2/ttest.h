/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/ttest.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <stdafx.h>

#if SEAM >= SEAM_0_0_0
#ifndef INCLUDED_F2_TTEST
#define INCLUDED_F2_TTEST 1

#include "test.h"

#include "tconsole.h"

namespace _ {
template <TestCase... N>
static inline SeamMajor MajorSeams(int seam_page, int seam_major,
                                   int& minor_seam_count) {
  static TestCase major_seams[sizeof...(N)] = {N...};
  minor_seam_count = sizeof...(N);
  return SeamMajor(seam_page, seam_major, major_seams, sizeof...(N));
}

template <SeamMajor*... N>
static inline SeamPage PageSeams(int seam_page, int& page_seam_count) {
  static SeamMajor* page_seams[sizeof...(N)] = {N...};
  page_seam_count = sizeof...(N);
  return SeamPage(seam_page, page_seams, sizeof...(N));
}

}  // namespace _

#endif  //< INCLUDED_F2_TTEST

#endif  //< #if SEAM >= SEAM_0_0_0
