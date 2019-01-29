/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/math_calc.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_FEATURES_MATH_1

#include "c_calc.h"

#include <cmath>
#include <climits>

namespace _ {

FLT AbsValue (FLT a)
{
    if (a < 0.0f) return a * -1.0f;
    return a;
}

double AbsValue (double a)
{
    if (a < 0.0) return a * -1.0;
    return a;
}

SI4 AbsValue (SI4 a)
{
    return 0.0; // return System.Abs (a);
}

long AbsValue (long a)
{
    return 0.0; // return System.Abs (a);
}

FLT Acos (FLT a)
{
    return 0.0f; // return System.Acos (a);
}

double Acos (double a)
{
    return 0.0; // return System.Acos (a);
}

FLT Asin (FLT a)
{
    return 0.0f; // return System.Asin (a);
}

double Asin (double a)
{
    return 0.0; // return System.Asin (a);
}

FLT Atan (FLT a)
{
    return 0.0f; // return System.Atan (a);
}

double Atan (double a)
{
    return 0.0; // return System.Atan (a);
}

FLT Atan2 (FLT y, FLT x)
{
    return 0.0f; // return System.Atan2 (y, x);
}

double Atan2 (double y, double x)
{
    return 0.0; // return System.Atan2 (y, x);
}

FLT Ceiling (FLT a)
{
    return 0.0f; // return System.ceil (a);
}

double Ceiling (double a)
{
    return 0.0; // return System.ceil (a);
}

FLT CopySign (FLT Magnitude, FLT sign)
{
    return 0.0f; // return System.copySign (Magnitude, sign);
}

double CopySign (double Magnitude, double sign)
{
    return 0.0; // return System.copySign (Magnitude, sign);
}

FLT Cos (FLT a)
{
    return cos (a); // return System.cos (a);
}

double Cos (double a)
{
    return 0.0; // return System.cos (a);
}

FLT Hcos (FLT x)
{
    return 0.0f; // return System.cosh (x);
}

double Hcos (double x)
{
    return 0.0; // return System.cosh (x);
}

FLT eToTheX (FLT a)
{
    return 0.0f; // return System.exp (a);
}

double eToTheX (double a)
{
    return 0.0; // return System.exp (a);
}

FLT OneMinus_eToTheX (FLT x)
{
    return 0.0f; // return System.expm1 (x);
}

double OneMinus_eToTheX (double x)
{
    return 0.0; // return System.expm1 (x);
}

FLT Floor (FLT a)
{
    return 0.0f; // return System.Floor (a);
}

double Floor (double a)
{
    return 0.0; // return System.Floor (a);
}

SI4 Exponent (FLT f)
{
    return 0; // return System.getExponent (f);
}

SI4 Exponent (double d)
{
    return 0; // return System.getExponent (d);
}

FLT Hypot (FLT x, FLT y)
{
    return 0.0f; // return System.hppypot (x, y);
}

double Hypot (double x, double y)
{
    return 0.0; // return System.hppypot (x, y);
}

FLT IeeeRemainder (FLT f1, FLT f2)
{
    return 0.0f; // return System.ieeeRemainder (f1, f2);
}

double IeeeRemainder (double f1, double f2)
{
    return 0.0; // return System.ieeeRemainder (f1, f2);
}

FLT Ln (FLT a)
{
    return 0.0f; // return System.log (a);
}

double Ln (double a)
{
    return 0.0; // return System.log (a);
}

FLT Log (FLT a)
{
    return 0.0f; // return System.log10 (a);
}

double Log (double a)
{
    return 0.0; // return System.log10 (a);
}

FLT LogXPlus1 (FLT x)
{
    return 0.0f; // return System.log1p (x);
}

double LogXPlus1 (double x)
{
    return 0.0; // return System.log1p (x);
}

FLT Max (FLT a, FLT b)
{
    return (a > b) ? a : b;
}

double Max (double a, double b)
{
    return (a > b) ? a : b;
}

SI4 Max (SI4 a, SI4 b)
{
    return (a > b) ? a : b;
}

long Max (long a, long b)
{
    return (a > b) ? a : b;
}

FLT Min (FLT a, FLT b)
{
    return 0.0f; // return System.min (a, b);
}

double Min (double a, double b)
{
    return 0.0; // return System.min (a, b);
}

SI4 Min (SI4 a, SI4 b)
{
    return 0.0; // return System.min (a, b);
}

long Min (long a, long b)
{
    return 0.0; // return System.min (a, b);
}

FLT NextAfter (FLT start, FLT direction)
{
    return 0.0f; // return System.nextAfter (start, direction);
}

double NextAfter (double start, double direction)
{
    return 0.0; // return System.nextAfter (start, direction);
}

FLT NextUp (FLT f)
{
    return 0.0f; // return System.nextUp (f);
}

double NextUp (double d)
{
    return 0.0; // return System.nextUp (d);
}

FLT Power (FLT a, FLT b)
{
    return 0.0f; // return System.Pow (a, b);
}

double Power (double a, double b)
{
    return 0.0; // return System.Pow (a, b);
}

FLT Random_f ()
{
    return 0.0f; // return System.Random ();
}

double Random_d ()
{
    return 0.0; // return System.Random ();
}

FLT Round (FLT a)
{
    return 0.0f; // return System.Rint (a);
}

double Round (double a)
{
    return 0.0; // return System.Rint (a);
}

SI4 RoundToInt (FLT a)
{
    return 0; // return System.Round (a);
}

long RoundToInt (double a)
{
    return 0; // return System.Round (a);
}

FLT ScaleB (FLT d, SI4 scaleFactor)
{
    return 0.0f; // return System.Scalb (d, scaleFactor);
}

double ScaleB (double d, SI4 scaleFactor)
{
    return 0.0; // return System.Scalb (d, scaleFactor);
}

FLT FScaleFactor (FLT f, SI4 scaleFactor)
{
    return 0.0f; // return System.Scalb (f, scaleFactor);
}

double FScaleFactor (double f, SI4 scaleFactor)
{
    return 0.0; // return System.Scalb (f, scaleFactor);
}

FLT SignOf (FLT f)
{
    return 0.0f; // return System.Signum (f);
}

double SignOf (double d)
{
    return 0.0; // return System.Signum (d);
}

FLT Sin (FLT a)
{
    return 0.0f; // return System.Sin (a);
}

double Sin (double a)
{
    return 0.0; // return System.Sin (a);
}

FLT Hsin (FLT x)
{
    return 0.0f; // return System.Sinh (x);
}

double Hsin (double x)
{
    return 0.0; // return System.Sinh (x);
}

FLT SquareRoot (FLT a)
{
    return 0.0f; // return System.Sqrt (a);
}

double SquareRoot (double a)
{
    return 0.0; // return System.Sqrt (a);
}

FLT CubeRoot (FLT a)
{
    return 0.0f; // return System.cbrt (a);
}

double CubeRoot (double a)
{
    return 0.0; // return System.cbrt (a);
}

FLT Tan (FLT a)
{
    return 0.0f; // return System.tan (a);
}

double Tan (double a)
{
    return 0.0; // return System.tan (a);
}

FLT Htan (FLT x)
{
    return 0.0f; // return System.tanh (x);
}

double Htan (double x)
{
    return 0.0; // return System.tanh (x);
}

FLT ToDegrees (FLT angrad)
{
    return 0.0f; // return System.toDegrees (angrad);
}

double ToDegrees (double angrad)
{
    return 0.0; // return System.toDegrees (angrad);
}

FLT ToRadians (FLT angle)
{
    return (angle * 180.0f) / Pi_f;
}

double ToRadians (double angle)
{
    return (angle * 180.0) / Pi_d;
}

FLT Ulp (FLT f)
{
    return 0.0f; // return System.ulp (f);
}

double Ulp (double d)
{
    return 0.0; // return System.ulp (d);
}

FLT Factoral (FLT value)
{
    FLT total;  //< The running total.

    if (value < 0)
    {
        total = value;

        value *= -1;

        while (value-- > 1)
            total *= value;

        return total * -1;
    }
    total = value;

    while (value-- > 1)
        total *= value;

    return total;
}

double Factoral (double value)
{
    double total;  //< The running total.

    if (value < 0)
    {
        total = value;

        value *= -1;

        while (value-- > 1)
            total *= value;

        return total * -1;
    }
    total = value;

    while (value-- > 1)
        total *= value;

    return total;
}

SI4 Factoral (SI4 value)
{
    SI4 total;  //< The running total.

    if (value < 0)
    {
        total = value;

        value *= -1;

        while (value-- > 1)
            total *= value;

        return total * -1;
    }
    total = value;

    while (value-- > 1)
        total *= value;

    return total;
}

long Factoral (long value)
{
    long total;  //< The running total.

    if (value < 0)
    {
        total = value;

        value *= -1;

        while (value-- > 1)
            total *= value;

        return total * -1;
    }
    total = value;

    while (value-- > 1)
        total *= value;

    return total;
}

inline FLT GetMinFloat ()
{
    return FLT_MIN;
}

inline double GetMinDouble ()
{
    return DBL_MIN;
}


inline FLT GetMaxFloat ()
{
    return FLT_MAX;
}

inline double GetMaxDouble ()
{
    return DBL_MAX;
}

SI4 GetNextPrime (SI4 value)
{

    /* A list of prime numbers between 0 and 2999. */
    SI4 PrimeNumbers[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
        53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107,
        109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167,
        173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283,
        293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
        367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431,
        433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491,
        499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
        577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641,
        643, 647, 653, 659, 66, 673, 677, 683, 691, 701, 709,
        719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787,
        797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859,
        863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
        947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013,
        1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063,
        1069, 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123,
        1129, 1151, 1153, 1163, 117, 1181, 1187, 1193, 1201,
        1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277,
        1279, 1283, 1289, 1291, 1297, 1301, 1303, 1307, 1319,
        1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409, 1423,
        1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471,
        1481, 1483, 1487, 1489, 1493, 1499, 1511, 1523, 1531,
        1543, 1549, 1553, 1559,1567, 1571, 1579, 1583, 1597,
        1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657,
        1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723,
        1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789,
        1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871,1873,
        1877, 1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949,
        1951, 1973, 1979, 1987, 1993, 1997, 1999, 2003, 2011,
        2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083,
        2087, 2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141,
        2143, 2153, 2161,2179, 2203, 2207, 2213, 2221, 2237,
        2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287, 2293,
        2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357,
        2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417,
        2423, 2437, 2441, 2447, 2459, 2467, 2473,2477, 2503,
        2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591,
        2593, 2609, 2617, 2621, 2633, 2647, 2657, 2659, 2663,
        2671, 2677, 2683, 2687, 2689, 2693, 2699, 2707, 2711,
        2713, 2719, 2729, 2731, 2741, 2749, 2753, 2767, 2777,
        2789, 2791,2797, 2801, 2803, 2819, 2833, 2837, 2843,
        2851, 2857, 2861, 2879, 2887, 2897, 2903, 2909, 2917,
        2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999
    };
    static const SI4 NumFastPrimeNumbers = &NumFastPrimeNumbers - &PrimeNumbers[0];

    // This function performs a binary search on the Numbers array to
    // 1.) determine if value is prime, or
    // 2.) return the next prime number after this number

    if (value > NumFastPrimeNumbers - 1)
        return 0;

    SI4 index = 0,                              //< The mid index and return index
        left_index = 0,                          //< The min search index
        right_index = NumFastPrimeNumbers - 1;   //< The max search index

    while (left_index < right_index)
    {
        index = (left_index + right_index) / 2;

        if (value > PrimeNumbers[index])
        {
            left_index = index + 1;
        }
        else if (value < PrimeNumbers[index])
        {
            right_index = index - 1;
        }
        else // Then value is a prime number
            return PrimeNumbers[index];
    }

    // because we checked to see if value > Numbers[index] first, then were going to over
    // shoot the prime that we want so we return primeNumber[index-1]. Or that's the theory at least

    return PrimeNumbers[index + 1]; // TEST ME!!!
}

}       //< namespace _
#endif
