/** The Chinese Room
    @version 0.x
    @file    targetver.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.

    @brief Including SDKDDKVer.h defines the highest available Windows platform.
    @desc  If you wish to build your application for a previous Windows 
           platform, include WinSDKVer.h and set the _WIN32_WINNT macro to the
           platform you wish to support before including SDKDDKVer.h.
*/

#pragma once

#if PLATFORM == MINGW || PLATFORM == WINDOWS
#include <SDKDDKVer.h>
#include <windows.h>
#endif
