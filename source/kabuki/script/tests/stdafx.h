/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/stdafx.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
#include <Windows.h>
#endif

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cfloat>
#include <algorithm>
#include <locale>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <math.h>
