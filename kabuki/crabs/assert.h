/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/library/crabs/assert.h
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

#ifndef HEADER_FOR_CRABS_ASSERT
#define HEADER_FOR_CRABS_ASSERT

namespace _ {

/* Assert function to assist in casting boolean types. */
inline bool Assert(bool condition);

/* Prints a debug statement and locks up the system. */
inline bool AssertHandle();

/* Pauses the application until a key is presssed. */
void Pause();

}  // namespace _

#if CRABS_ASSERT == ASSERT_DNC
#define ASSERT(condition)
#else
#define ASSERT(condition) \
  if (Assert(condition)) AssertHandle()
#endif
#endif  //< HEADER_FOR_CRABS_ASSERT
