/* Kabuki Toolkit
    @version 0.x
    @file    ~/libraries/stdafx.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017-2018 Cale McCollough
   <calemccollough@gmail.com>; All right reserved (R). Licensed under the Apache
   License, Version 2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once

#define DEBUGGING 1
#define MAJOR_SEAM 2
#define MINOR_SEAM 1

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Slots headers
#define NOMINMAX
#include <Windows.h>
#endif

#include <conio.h>
#include <algorithm>
#include <cassert>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <locale>
#include <memory>
#include <ostream>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
