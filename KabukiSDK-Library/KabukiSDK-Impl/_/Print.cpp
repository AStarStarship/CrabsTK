/** Kabuki Software Development Kit
    @file    /.../KabukiSDK-Library/KabukiSDK-Impl/Print.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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

#include <_/Print.hpp>

#include <stdio.h>

namespace _ {

void printLine (int LineWidth, string Token)
{
    for (int i = 0; i < LineWidth; ++i) 
        printf (Token);
}

void printLines (int NumRows)
{
    printf ("\r");
    for (int i = 0; i < NumRows - 1; ++i)
        printf ("\n");
}

}   //< namespace _
