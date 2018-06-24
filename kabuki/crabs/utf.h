/* Kabuki Toolkit
@version 0.x
@file    $kabuki-toolkit/kabuki/crabs/utf.h
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
#if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
#ifndef HEADER_FOR_CRABS_PRINTER
#define HEADER_FOR_CRABS_PRINTER
// Dependencies:
#include "config.h"
// End dependencies.

#if USING_UTF8
#include "utf8.h"
#endif
#if USING_UTF16
#include "utf16.h"
#endif
#if USING_UTF32
#include "utf32.h"
#endif

namespace _ {
#if CRABS_NATIVE_UTF == UTF8
using Printer = Printer1;
using Center = Center1;
using Right = Right1;
#elif CRABS_NATIVE_UTF == UTF16
using Printer = Printer2;
using Center = Center2;
using Right = Right2;
#elif CRABS_NATIVE_UTF == UTF32
using Printer = Printer4;
using Center = Center4;
using Right = Right4;
#endif
}  // namespace _
#endif  //< #if HEADER_FOR_CRABS_PRINTER
#endif  //< #if SEAM_MAJOR > 0 || SEAM_MAJOR == 0 && SEAM_MINOR >= 2
