/** The Chinese Room
    @version 0.x
    @file    /.../Params.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                            All right reserved(R).

        Licensed under the Apache License, Version 2.0(the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#ifndef CHINESEROOM_ARGS_H
#define CHINESEROOM_ARGS_H

#include "Config.h"
#include "Types.h"

namespace _ {

template<uint_t... N>
struct Params
/*< C++11 constexpr struct for creating parameter headers with less typing. */
{
    //static constexpr size_t length = sizeof...(N);
    static const uint_t Header[sizeof...(N)];
    static const uint_t terminator = 0;
};

inline void printParams(const uint_t* header)
/*<  Prints out the parameters to the debug console. */
{
    uint_t numParams = *header,
        i;

    printf ("Params<");
    if (numParams > MaxNumParams)
    {
        printf ("Invalid numParams: %u>\n", numParams);
        return;
    }
    printf ("%u: ", *header);
    ++header;
    for (i = 1; i < numParams; ++i)
    {
        printf ("%s, ", typeString (*header));
        ++header;
    }
    printf ("%s>\n", typeString (*header));
}

template<byte numArgs>
struct RxArgs
{
    enum { NumArgs = numArgs };

    void* ptrs[numArgs];
};

template<byte numArgs>
struct TxArgs
{
    enum { NumArgs = numArgs };

    const void* ptrs[numArgs];
};
 
inline void** args(void** ptrs, void* a)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	ptrs[0] = a;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	ptrs[0] = a;
	ptrs[1] = b;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b, void* c)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
 	letters a-z. */
{
	ptrs[0] = a;
	ptrs[1] = b;
	ptrs[2] = c;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b, void* c, void* d)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
 	letters a-z. */
{
	ptrs[0] = a;
	ptrs[1] = b;
	ptrs[2] = c;
	ptrs[3] = d;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	ptrs[0] = a;
	ptrs[1] = b;
	ptrs[2] = c;
	ptrs[3] = d;
	ptrs[4] = e;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	ptrs[0] = a;
	ptrs[1] = b;
	ptrs[2] = c;
	ptrs[3] = d;
	ptrs[4] = e;
	ptrs[5] = f;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	ptrs[0] = a;
	ptrs[1] = b;
	ptrs[2] = c;
	ptrs[3] = d;
	ptrs[4] = e;
	ptrs[5] = f;
	ptrs[6] = g;

	return ptrs;
}

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
		void* f, void* g, void* h, void* i, void* j)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
 	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
 	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
 	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s, void* t)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j,
    void* k, void* l, void* m, void* n, void* o, void* p, void* q, void* r,
	void* s, void* t, void* u)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
	void* v)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
	void* v, void* w)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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


inline void** args (void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
	void* v, void* w, void* x)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
	void* v, void* w, void* x, void* y)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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

inline void** args(void** ptrs, void* a, void* b, void* c, void* d, void* e,
	void* f, void* g, void* h, void* i, void* j, void* k, void* l, void* m,
	void* n, void* o, void* p, void* q, void* r, void* s, void* t, void* u,
	void* v, void* w, void* x, void* y, void* z)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
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
 
inline const void** args(void** ptrs, const void* a)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;
	const_ptrs[3] = d;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;
	const_ptrs[3] = d;
	const_ptrs[4] = e;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;
	const_ptrs[3] = d;
	const_ptrs[4] = e;
	const_ptrs[5] = f;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;
	const_ptrs[3] = d;
	const_ptrs[4] = e;
	const_ptrs[5] = f;
	const_ptrs[6] = g;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;
	const_ptrs[3] = d;
	const_ptrs[4] = e;
	const_ptrs[5] = f;
	const_ptrs[6] = g;
	const_ptrs[7] = h;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
	const_ptrs[0] = a;
	const_ptrs[1] = b;
	const_ptrs[2] = c;
	const_ptrs[3] = d;
	const_ptrs[4] = e;
	const_ptrs[5] = f;
	const_ptrs[6] = g;
	const_ptrs[7] = h;
	const_ptrs[8] = i;

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m,
	const void* n)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m,
	const void* n, const void* o)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m,
	const void* n, const void* o, const void* p)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m,
	const void* n, const void* o, const void* p, const void* q, const void* r)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m,
	const void* n, const void* o, const void* p, const void* q, const void* r,
	const void* s)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b, const void* c,
	const void* d, const void* e, const void* f, const void* g, const void* h,
	const void* i, const void* j, const void* k, const void* l, const void* m,
	const void* n, const void* o, const void* p, const void* q, const void* r,
	const void* s, const void* t)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b,
	const void* c, const void* d, const void* e, const void* f, const void* g, 
	const void* h, const void* i, const void* j, const void* k, const void* l,
	const void* m, const void* n, const void* o, const void* p, const void* q,
	const void* r, const void* s, const void* t, const void* u)
/*< Sets the params to the given pointers corresponding to the index 1-26 of
	the letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args (void** ptrs, const void* a, const void* b,
	const void* c, const void* d, const void* e, const void* f, const void* g,
	const void* h, const void* i, const void* j,
	const void* k, const void* l, const void* m, const void* n, const void* o,
	const void* p, const void* q, const void* r, const void* s, const void* t,
	const void* u, const void* v)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args (void** ptrs, const void* a, const void* b,
	const void* c, const void* d, const void* e, const void* f, const void* g,
	const void* h, const void* i, const void* j,
	const void* k, const void* l, const void* m, const void* n, const void* o,
	const void* p, const void* q, const void* r, const void* s, const void* t,
	const void* u, const void* v, const void* w)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b,
	const void* c, const void* d, const void* e, const void* f, const void* g, 
	const void* h, const void* i, const void* j, const void* k, const void* l, 
	const void* m, const void* n, const void* o, const void* p, const void* q, 
	const void* r, const void* s, const void* t, const void* u, const void* v, 
	const void* w, const void* x)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b,
	const void* c, const void* d, const void* e, const void* f, const void* g, 
	const void* h, const void* i, const void* j, const void* k, const void* l, 
	const void* m, const void* n, const void* o, const void* p, const void* q, 
	const void* r, const void* s, const void* t, const void* u, const void* v, 
    const void* w, const void* x, const void* y)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

inline const void** args(void** ptrs, const void* a, const void* b,
	const void* c, const void* d, const void* e, const void* f, const void* g, 
	const void* h, const void* i, const void* j, const void* k, const void* l, 
	const void* m, const void* n, const void* o, const void* p, const void* q, 
	const void* r, const void* s, const void* t, const void* u, const void* v, 
    const void* w, const void* x, const void* y, const void* z)
/*< Sets the params to the given pointers corresponding to the index 1-26 of the
	letters a-z. */
{
	const void** const_ptrs = const_cast<const void**>(ptrs);
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

	return const_ptrs;
}

}       //< namespace _

#endif  //< CHINESEROOM_ARGS_H
