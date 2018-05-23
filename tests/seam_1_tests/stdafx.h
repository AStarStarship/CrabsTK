/** Kabuki Toolkit
    @version 0.x
    @file    ~/library/stdafx.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-8 Cale McCollough <calemccollough@gmail.com>;
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

#define DEBUGGING  1
#define MAJOR_SEAM 1
#define MINOR_SEAM 2

#ifdef WIN32
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Slots headers
#define NOMINMAX
#include <Windows.h>
#endif

#include <algorithm>
#include <chrono>
#include <cfloat>
#include <chrono>
#include <conio.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <locale>
#include <limits>
#include <memory>
#include <ostream>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <functional>
#include <cstdio>
