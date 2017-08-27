/** The Chinese Room
    @version 0.x
    @file    precompiled_headers.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 [Cale McCollough] (calemccollough.github.io)

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
*/

#ifndef STDAFX_H
#define STDAFX_H

#if PLATFORM == MINGW || PLATFORM == WINDOWS
#include "targetver.h"
#endif

#include <stdio.h>
#include <tchar.h>
#include <cstdint>
#include <stdint.h>
#include <time.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <float.h>
#include <stdio.h>
#include <algorithm>
#include <locale>
#include <cstdlib>
#include <chrono>
#include <exception>
#include <iostream>
#include <math.h>
#if USING_UTF
#include <string>
#endif

#endif  //< STDAFX_H
