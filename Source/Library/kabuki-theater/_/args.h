/** The Chinese Room
    @version 0.x
    @file    ~/args.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>
                            All right reserved (R).
             Licensed under the Apache License, Version 2.0 (the "License"); 
             you may not use this file except in compliance with the License. 
             You may obtain a copy of the License at
                        http://www.apache.org/licenses/LICENSE-2.0
             Unless required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
    limitations under the License.
*/

#ifndef CHINESEROOM_ARGS_H
#define CHINESEROOM_ARGS_H

#include "types.h"

namespace _ {

/** An I2P escape sequence header.
struct uses C++11 variadic template to ensure only one copy in ROM and to eliminate
redundant typing. */
template<const uint_t... N>
INLINE const uint_t* Esc () {
    static const uint_t list[sizeof... (N)] = { N... };
    return list;
}

/**  Prints out the parameters to the debug console. */
INLINE void PrintEsc (const uint_t* params) {
    if (params < (const uint_t*)256) {
        std::cout << reinterpret_cast<uintptr_t> (params) << '\n';
        return;
    }
    uint_t num_params = *params++,
        i,
        value = 0;

    std::cout << "Esc<";
    if (num_params > kMaxNumParams) {
        printf ("Invalid num_params: %u>\n", num_params);
        return;
    }
    printf ("%u: ", num_params);
    for (i = 1; i < num_params; ++i) {
        value = *params++;
        std::cout << TypeString (value) << ", ";
        if (value == STX) {
            ++i;
            value = *params++;
            printf ("%u, ", value);
        } else if (value >= AR1 && value <= AR8) {
            ++i;
            value = *params++;
            printf ("%u, ", value);
            value = *params;
            printf ("%s ", TypeString (value));
        }
        params;
    }
    value = *params++;
    printf ("%s", TypeString (value));
    if (value == STX) {
        value = *params;
        printf (", %u>\n", value);
    } else if (value >= AR1 && value <= AR8) {
        ++i;
        value = *params++;
        printf (", %u, ", value);
        value = *params++;
        printf ("%s>\n", TypeString (value));
    } else {
        std::cout << ">\n";
    }
}

/*< Returns the requested parameter number. */
INLINE uint_t ParamNumber (const uint_t* params, byte param_number) {
    if (params == nullptr)
        return 0;
    uint_t num_params = *params++;
    int i;
    for (i = 0; i < param_number; ++i) {
        uint_t value = params[i];
        if (value == STX)
            ++param_number;
        else if (value >= AR1 && value <= AR8)
            param_number += 2;
    }
    return params[i];
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a) {
    ptrs[0] = a;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b) {
    ptrs[0] = a;
    ptrs[1] = b;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s, void* t) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j,
                    void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r,
                    void* s, void* t, void* u) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;
    ptrs[20] = u;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
                    void* v) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;
    ptrs[20] = u;
    ptrs[20] = v;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
                    void* v, void* w) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;
    ptrs[20] = u;
    ptrs[21] = v;
    ptrs[22] = w;

    return ptrs;
}


/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
                    void* v, void* w, void* x) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;
    ptrs[20] = u;
    ptrs[21] = v;
    ptrs[22] = w;
    ptrs[23] = x;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
                    void* v, void* w, void* x, void* y) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;
    ptrs[20] = u;
    ptrs[21] = v;
    ptrs[22] = w;
    ptrs[23] = x;
    ptrs[24] = y;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
                    void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
                    void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
                    void* v, void* w, void* x, void* y, void* z) {
    ptrs[0] = a;
    ptrs[1] = b;
    ptrs[2] = c;
    ptrs[3] = d;
    ptrs[4] = e;
    ptrs[5] = f;
    ptrs[6] = g;
    ptrs[7] = h;
    ptrs[8] = i;
    ptrs[9] = j;
    ptrs[10] = k;
    ptrs[11] = l;
    ptrs[12] = m;
    ptrs[13] = n;
    ptrs[14] = o;
    ptrs[15] = p;
    ptrs[16] = q;
    ptrs[17] = r;
    ptrs[18] = s;
    ptrs[19] = t;
    ptrs[20] = u;
    ptrs[21] = v;
    ptrs[22] = w;
    ptrs[23] = x;
    ptrs[24] = y;
    ptrs[25] = z;

    return ptrs;
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;

    return const_cast<void**>(const_ptrs);
}

INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i)
    /** Sets the params to the given pointers corresponding to the index 1-26 of the
    letters a-z. */
{
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p, const void* q, const void* r) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p, const void* q, const void* r,
                    const void* s) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b, const void* c,
                    const void* d, const void* e, const void* f, const void* g, const void* h,
                    const void* i, const void* j, const void* k, const void* l, const void* m,
                    const void* n, const void* o, const void* p, const void* q, const void* r,
                    const void* s, const void* t) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;

    return const_cast<void**>(const_ptrs);
}
/** Sets the params to the given pointers corresponding to the index 1-26 of
the letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* s, const void* t, const void* u) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;
    const_ptrs[20] = u;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j,
                    const void* k, const void* l, const void* m, const void* n, const void* o,
                    const void* p, const void* q, const void* r, const void* s, const void* t,
                    const void* u, const void* v) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;
    const_ptrs[20] = u;
    const_ptrs[21] = v;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j,
                    const void* k, const void* l, const void* m, const void* n, const void* o,
                    const void* p, const void* q, const void* r, const void* s, const void* t,
                    const void* u, const void* v, const void* w) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;
    const_ptrs[20] = u;
    const_ptrs[21] = v;
    const_ptrs[21] = w;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
    letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* s, const void* t, const void* u, const void* v,
                    const void* w, const void* x) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;
    const_ptrs[20] = u;
    const_ptrs[21] = v;
    const_ptrs[22] = w;
    const_ptrs[23] = x;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* s, const void* t, const void* u, const void* v,
                    const void* w, const void* x, const void* y) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;
    const_ptrs[20] = u;
    const_ptrs[21] = v;
    const_ptrs[22] = w;
    const_ptrs[23] = x;
    const_ptrs[24] = y;

    return const_cast<void**>(const_ptrs);
}

/** Sets the params to the given pointers corresponding to the index 1-26 of the
    letters a-z. */
INLINE void** Args (void** ptrs, const void* a, const void* b,
                    const void* c, const void* d, const void* e, const void* f, const void* g,
                    const void* h, const void* i, const void* j, const void* k, const void* l,
                    const void* m, const void* n, const void* o, const void* p, const void* q,
                    const void* r, const void* s, const void* t, const void* u, const void* v,
                    const void* w, const void* x, const void* y, const void* z) {
    const void** const_ptrs = const_cast<const void**> (ptrs);
    const_ptrs[0] = a;
    const_ptrs[1] = b;
    const_ptrs[2] = c;
    const_ptrs[3] = d;
    const_ptrs[4] = e;
    const_ptrs[5] = f;
    const_ptrs[6] = g;
    const_ptrs[7] = h;
    const_ptrs[8] = i;
    const_ptrs[9] = j;
    const_ptrs[10] = k;
    const_ptrs[11] = l;
    const_ptrs[12] = m;
    const_ptrs[13] = n;
    const_ptrs[14] = o;
    const_ptrs[15] = p;
    const_ptrs[16] = q;
    const_ptrs[17] = r;
    const_ptrs[18] = s;
    const_ptrs[19] = t;
    const_ptrs[20] = u;
    const_ptrs[21] = v;
    const_ptrs[22] = w;
    const_ptrs[23] = x;
    const_ptrs[24] = y;
    const_ptrs[25] = z;

    return const_cast<void**>(const_ptrs);
}

}       //< namespace _
#endif  //< CHINESEROOM_ARGS_H
