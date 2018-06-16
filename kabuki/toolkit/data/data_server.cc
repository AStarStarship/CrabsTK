/** Kabuki Toolkit
    @file    ~/source/data/data_server.cc
    @author  Cale McCollough <calemccollough.github.io>
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
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#if MAJOR_SEAM == 2 && MINOR_SEAM == 1
#define PRINTF(format, ...) printf(format, __VA_ARGS__);
#define PRINT_PAUSE(message)\
    printf ("\n\n%s\n", message); system ("PAUSE");
#else
#define PRINTF(x, ...)
#define PRINT_PAUSE(message)
#endif

#include "server.h"

namespace _ {

Server::Server ():
    Room ("kabuki.security.Server") {

}

const Op* Server::Star (wchar_t index, Expr* expr) {
    static const Op kThis = { "kabuki.security.Server",
        OpFirst ('A'), OpLast ('A'),
        "Datagram security server.", 0
    };
    //void* args[1];
    switch (index) {
        case '?': return ExprQuery (expr, &kThis);
        case 'A': {
            static const Op kOpA = { "foo",
                Params<0> (), Params<0> (),
                "Why does everyone use foo?", 0
            };
            if (!expr) return &kOpA;
        }
    }
    return nullptr;
}

}       //< security
}       //< kabuki
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
