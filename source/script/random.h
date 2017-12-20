/** Kabuki Toolkit
    @version 0.x
    @file    ~/source/script/random.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#ifndef HEADER_FOR_SCRIPT_RANDOM
#define HEADER_FOR_SCRIPT_RANDOM

namespace _ {

/** Primes the randomizer with the current system time. */
inline void PrimeRandomizer () {
    srand ((uint)time (nullptr));
}

/** Gets a random number. */
template<typename T>
inline T NextRandom () {
    auto random_number = rand ();
    //enum {
    //    kSize = sizeof (random_number),
          //< Will be used for portable 32/64-bit clocks.
    //};
    
    return static_cast<T>(random_number);
}


/** Gets a random number. */
template<typename T>
inline T Random () {
    PrimeRandomizer ();
    return NextRandom<T> ();
}

/** Gets a random number. */
inline float RandomFloat () {
    srand ((uint)time (nullptr));
    uint32_t random = Random<uint32_t> ();
    return *reinterpret_cast<float*> (&random);
}

/** Gets a random number. */
inline double RandomDouble () {
    srand ((uint)time (nullptr));
    uint64_t random = Random<uint64_t> ();
    return *reinterpret_cast<double*> (&random);
}

}       //< namespace _
#endif  //< HEADER_FOR_SCRIPT_RANDOM
