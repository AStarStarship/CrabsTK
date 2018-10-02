/** Kabuki Arcade
    @file    ~/source/projects/kabuki_cards_server/pch.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017-2018 Cale McCollough <calemccollough@gmail.com>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once

#ifdef WIN32
#include "resultver.h"
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Mirrors headers
#define NOMINMAX
#include <Mirrors.h>
#endif

#include <algorithm>
#include <cfloat>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <devguid.h>
#include <exception>
#include <fstream>
#include <initguid.h>
#include <iostream>
#include <iterator>
#include <locale>
#include <math.h>
#include <limits>
#include <regex>
#include <setupapi.h>
#include <sstream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <time.h>
#include <vector>
#include <windows.h>