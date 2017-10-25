/** kabuki::script
    @version 0.x
    @file    ~/source/kabuki/script/include/args.h
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
#include <stdafx.h>

#ifndef KABUKI_SCRIPT_ARGS_H
#define KABUKI_SCRIPT_ARGS_H

#include "types.h"

namespace _ {

/** An Script escape sequence header.
    struct KABUKI uses C++11 variadic template to ensure only one copy in ROM and to eliminate
    redundant typing. */
template<const uint_t... N>
KABUKI const uint_t* Params () {
    static const uint_t list[sizeof... (N)] = { N... };
    return list;
}

/**  Prints out the parameters to the debug console. */
KABUKI void ParamsPrint (const uint_t* params);

/*< Returns the requested parameter number. */
KABUKI uint_t ParamNumber (const uint_t* params, byte param_number);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k,
                    void* l);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k,
                    void* l, void* m);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k,
                    void* l, void* m, void* n);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, 
                    void* l, void* m, void* n, void* o);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, 
                    void* l, void* m, void* n, void* o, void* p);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, 
                    void* l, void* m, void* n, void* o, void* p, void* q,
                    void* r);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, 
                    void* l, void* m, void* n, void* o, void* p, void* q, 
                    void* r, void* string);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* string, void* t);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j,
                    void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r,
                    void* string, void* t, void* user);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* string, void* t, void* user,
                    void* v);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* string, void* t, void* user,
                    void* v, void* w);


/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* string, void* t, void* user,
                    void* v, void* w, void* x);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* string, void* t, void* user,
                    void* v, void* w, void* x, void* y);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* string, void* t, void* user,
                    void* v, void* w, void* x, void* y, void* z);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h);

KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p, const void* q, const void* r);

/** Sets the params to the given pointers corresponding to the index 1-26 of 
    the letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p, const void* q, const void* r,
                    const void* string);

/** Sets the params to the given pointers corresponding to the index 1-26 of 
    the letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p, const void* q, const void* r,
                    const void* string, const void* t);

/** Sets the params to the given pointers corresponding to the index 1-26 of
the letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* string, const void* t, const void* user);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j,
                    const void* k, const void* l, const void* m, const void* n, const void* o,
                    const void* p, const void* q, const void* r, const void* string, const void* t,
                    const void* user, const void* v);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j,
                    const void* k, const void* l, const void* m, const void* n, const void* o,
                    const void* p, const void* q, const void* r, const void* string, const void* t,
                    const void* user, const void* v, const void* w);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
    letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* string, const void* t, const void* user, const void* v,
                    const void* w, const void* x);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* string, const void* t, const void* user, const void* v,
                    const void* w, const void* x, const void* y);

/** Sets the params to the given pointers corresponding to the index 1-26 of the
    letters a-z. */
KABUKI void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* string, const void* t, const void* user, const void* v,
                    const void* w, const void* x, const void* y, const void* z);
}       //< namespace _
#endif  //< KABUKI_SCRIPT_ARGS_H
