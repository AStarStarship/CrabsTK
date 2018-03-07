/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/kabuki/global.h
    @author  Cale McCollough <calemccollough.github.io>
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
#include <stdafx.h>
#include <assembly.h>

#if RUN_UNIT_TESTS

#ifndef HEADER_FOR_GLOBAL
#define HEADER_FOR_GLOBAL

#include "../../cpputest/include/CppUTest/CommandLineTestRunner.h"
#include "../../cpputest/include/CppUTest/TestHarness.h"

#include "../source/crabs/global.h"

void TestSeam1 ();
void TestSeam1_1 ();
void TestSeam1_2 ();
void TestSeam1_3 ();
void TestSeam1_4 ();
void TestSeam1_5 ();

#endif  //< HEADER_FOR_GLOBAL
#endif  //< RUN_UNIT_TESTS
