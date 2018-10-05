/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    ~/kabuki/f2/test_footer.h
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
#include <pch.h>

#ifndef INCLUDED_KABUKI_F2_TEST_FOOTER
#define INCLUDED_KABUKI_F2_TEST_FOOTER 1

#if SEAM > 0 && SEAM <= SEAM_N
#undef ASSERT(condition)
#undef TEST1(item)
#undef TEST(a, b)
#undef PRINTF(x, ...)
#undef PAUSE(message)
#undef PAUSEF(message)
#undef TEST_BEGIN
#undef TEST_END
#endif

#endif  //< #ifndef INCLUDED_KABUKI_F2_TEST_FOOTER
