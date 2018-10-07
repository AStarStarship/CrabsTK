/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /header.h
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

// kabuki.f2
#define SEAM_00_00_00__00 1   //< kabuki.f2.test_itos_and_stoi
#define SEAM_00_00_00__01 2   //< kabuki.f2.test_ftos_and_stof
#define SEAM_00_00_00__02 3   //< kabuki.f2.test_core_text_functions
#define SEAM_00_00_00__03 4   //< kabuki.f2.test_ascii_strings
#define SEAM_00_00_00__04 5   //< kabuki.f2.test_ascii_clock
#define SEAM_00_00_00__05 6   //< kabuki.f2.test_ascii_
#define SEAM_00_00_00__06 7   //< kabuki.f2.
#define SEAM_00_00_00__07 8   //< kabuki.f2.
#define SEAM_00_00_00__08 9   //< kabuki.f2.
#define SEAM_00_00_00__09 10  //< kabuki.f2.
#define SEAM_00_00_00__10 11  //< kabuki.f2.
#define SEAM_00_00_00__11 12  //< kabuki.f2.
#define SEAM_00_00_00__12 13  //< kabuki.f2.
#define SEAM_00_00_00__13 14  //< kabuki.f2.
#define SEAM_00_00_01__00 15  //< kabuki.crabs
#define SEAM_00_00_02__00 16  //< kabuki.hal.
#define SEAM_00_00_03__00 17  //< kabuki.drivers
#define SEAM_00_00_04__00 18  //< kabuki.drivers
#define SEAM_00_00_04__00 19  //< kabuki.features.storage.blockdevice
#define SEAM_00_00_04__00 20  //< kabuki.features.storage.filesystem
#define SEAM_00_00_04__00 20  //< kabuki.features.hal
#define SEAM_00_00_04__00 21  //< kabuki.features.cellular
#define SEAM_00_00_04__00 22  //< kabuki.features.cryptocell
#define SEAM_00_00_04__00 23  //< kabuki.features.device_key
#define SEAM_00_00_04__00 24  //< kabuki.features.ble
#define SEAM_00_00_04__00 25  //< kabuki.features.bootloader
#define SEAM_00_00_04__00 26  //< kabuki.features.lorawan
#define SEAM_00_00_04__00 27  //< kabuki.features.lwipstack
#define SEAM_00_00_04__00 28  //< kabuki.features.tls
#define SEAM_00_00_04__00 29  //< kabuki.features.nanostack
#define SEAM_00_00_04__00 30  //< kabuki.features.nfc

// kabuki.features.unsupported

#if SEAM > 0 && SEAM <= SEAN_N
// Add global debug macros here.
#else
// This is for when the code is not being debugged and the global debug macros
// don't do anything.
#endif
