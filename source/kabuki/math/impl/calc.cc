/** Kabuki Theater
    @file       /.../Source-Impl/_Math/Calc.cpp
    @author  Cale McCollough <https://calemccollough.github.io>
    @license Copyright 2016 (C) Cale McCollough <cale.mccollough@gmail.com>

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

#include <_Math/Calc.h>
#include <float.h>

namespace _Math {

float absValue (float a)
{
    if (a < 0.0f) return a * -1.0f;
    return a;
}

double absValue (double a)
{
    if (a < 0.0) return a * -1.0;
    return a;
}

int absValue (int a)
{
    return 0.0; // return System._Math::Abs (a);
}

long absValue (long a)
{
    return 0.0; // return System._Math::Abs (a);
}

float acos (float a)
{
    return 0.0f; // return System._Math::Acos (a);
}

double acos (double a)
{
    return 0.0; // return System._Math::Acos (a);
}

float asin (float a)
{
    return 0.0f; // return System._Math::Asin (a);
}

double asin (double a)
{
    return 0.0; // return System._Math::Asin (a);
}

float atan (float a)
{
    return 0.0f; // return System._Math::Atan (a);
}

double atan (double a)
{
    return 0.0; // return System._Math::Atan (a);
}

float atan2 (float y, float x)
{
    return 0.0f; // return System._Math::Atan2 (y, x);
}

double atan2 (double y, double x)
{
    return 0.0; // return System._Math::Atan2 (y, x);
}

float ceiling (float a)
{
    return 0.0f; // return System._Math::ceil (a);
}

double ceiling (double a)
{
    return 0.0; // return System._Math::ceil (a);
}

float copySign (float magnitude, float sign)
{
    return 0.0f; // return System._Math::copySign (magnitude, sign);
}

double copySign (double magnitude, double sign)
{
    return 0.0; // return System._Math::copySign (magnitude, sign);
}

float cos (float a)
{
    return cos (a); // return System._Math::cos (a);
}

double cos (double a)
{
    return 0.0; // return System._Math::cos (a);
}

float hcos (float x)
{
    return 0.0f; // return System._Math::cosh (x);
}

double hcos (double x)
{
    return 0.0; // return System._Math::cosh (x);
}

float eToTheX (float a)
{
    return 0.0f; // return System._Math::exp (a);
}

double eToTheX (double a)
{
    return 0.0; // return System._Math::exp (a);
}

float oneMinusEToTheX (float x)
{
    return 0.0f; // return System._Math::expm1 (x);
}

double oneMinusEToTheX (double x)
{
    return 0.0; // return System._Math::expm1 (x);
}

float floor (float a)
{
    return 0.0f; // return System._Math::Floor (a);
}

double floor (double a)
{
    return 0.0; // return System._Math::Floor (a);
}

int exponent (float f)
{
    return 0; // return System._Math::getExponent (f);
}

int exponent (double d)
{
    return 0; // return System._Math::getExponent (d);
}

float hypot (float x, float y)
{
    return 0.0f; // return System._Math::hppypot (x, y);
}

double hypot (double x, double y)
{
    return 0.0; // return System._Math::hppypot (x, y);
}

float ieeeRemainder (float f1, float f2)
{
    return 0.0f; // return System._Math::ieeeRemainder (f1, f2);
}

double ieeeRemainder (double f1, double f2)
{
    return 0.0; // return System._Math::ieeeRemainder (f1, f2);
}

float ln (float a)
{
    return 0.0f; // return System._Math::log (a);
}

double ln (double a)
{
    return 0.0; // return System._Math::log (a);
}

float log (float a)
{
    return 0.0f; // return System._Math::log10 (a);
}

double log (double a)
{
    return 0.0; // return System._Math::log10 (a);
}

float logXPlus1 (float x)
{
    return 0.0f; // return System._Math::log1p (x);
}

double logXPlus1 (double x)
{
    return 0.0; // return System._Math::log1p (x);
}

float max (float a, float b)
{
    return (a > b) ? a : b;
}

double max (double a, double b)
{
    return (a > b) ? a : b;
}

int max (int a, int b)
{
    return (a > b) ? a : b;
}

long max (long a, long b)
{
    return (a > b) ? a : b;
}

float min (float a, float b)
{
    return 0.0f; // return System._Math::min (a, b);
}

double min (double a, double b)
{
    return 0.0; // return System._Math::min (a, b);
}

int min (int a, int b)
{
    return 0.0; // return System._Math::min (a, b);
}

long min (long a, long b)
{
    return 0.0; // return System._Math::min (a, b);
}

float nextAfter (float start, float direction)
{
    return 0.0f; // return System._Math::nextAfter (start, direction);
}

double nextAfter (double start, double direction)
{
    return 0.0; // return System._Math::nextAfter (start, direction);
}

float nextUp (float f)
{
    return 0.0f; // return System._Math::nextUp (f);
}

double nextUp (double d)
{
    return 0.0; // return System._Math::nextUp (d);
}

float power (float a, float b)
{
    return 0.0f; // return System._Math::Pow (a, b);
}

double power (double a, double b)
{
    return 0.0; // return System._Math::Pow (a, b);
}

float random_f ()
{
    return 0.0f; // return System._Math::Random ();
}

double random_d ()
{
    return 0.0; // return System._Math::Random ();
}

float round (float a)
{
    return 0.0f; // return System._Math::Rint (a);
}

double round (double a)
{
    return 0.0; // return System._Math::Rint (a);
}

int roundToInt (float a)
{
    return 0; // return System._Math::Round (a);
}

long roundToInt (double a)
{
    return 0; // return System._Math::Round (a);
}

float scaleB (float d, int scaleFactor)
{
    return 0.0f; // return System._Math::Scalb (d, scaleFactor);
}

double scaleB (double d, int scaleFactor)
{
    return 0.0; // return System._Math::Scalb (d, scaleFactor);
}

float fScaleFactor (float f, int scaleFactor)
{
    return 0.0f; // return System._Math::Scalb (f, scaleFactor);
}

double fScaleFactor (double f, int scaleFactor)
{
    return 0.0; // return System._Math::Scalb (f, scaleFactor);
}

float signOf (float f)
{
    return 0.0f; // return System._Math::Signum (f);
}

double signOf (double d)
{
    return 0.0; // return System._Math::Signum (d);
}

float sin (float a)
{
    return 0.0f; // return System._Math::Sin (a);
}

double sin (double a)
{
    return 0.0; // return System._Math::Sin (a);
}

float hsin (float x)
{
    return 0.0f; // return System._Math::Sinh (x);
}

double hsin (double x)
{
    return 0.0; // return System._Math::Sinh (x);
}

float squareRoot (float a)
{
    return 0.0f; // return System._Math::Sqrt (a);
}

double squareRoot (double a)
{
    return 0.0; // return System._Math::Sqrt (a);
}

float cubeRoot (float a)
{
    return 0.0f; // return System._Math::cbrt (a);
}

double cubeRoot (double a)
{
    return 0.0; // return System._Math::cbrt (a);
}

float tan (float a)
{
    return 0.0f; // return System._Math::tan (a);
}

double tan (double a)
{
    return 0.0; // return System._Math::tan (a);
}

float htan (float x)
{
    return 0.0f; // return System._Math::tanh (x);
}

double htan (double x)
{
    return 0.0; // return System._Math::tanh (x);
}

float toDegrees (float angrad)
{
    return 0.0f; // return System._Math::toDegrees (angrad);
}

double toDegrees (double angrad)
{
    return 0.0; // return System._Math::toDegrees (angrad);
}

float toRadians (float angle)
{
    return (angle * 180.0f) / Pi_f;
}

double toRadians (double angle)
{
    return (angle * 180.0) / Pi_d;
}

float ulp (float f)
{
    return 0.0f; // return System._Math::ulp (f);
}

double ulp (double d)
{
    return 0.0; // return System._Math::ulp (d);
}

float factoral (float thisNumber)
{
    float total;  //< The running total.

    if (thisNumber < 0)
    {
        total = thisNumber;

        thisNumber *= -1;

        while (thisNumber-- > 1)
            total *= thisNumber;

        return total * -1;
    }
    total = thisNumber;

    while (thisNumber-- > 1)
        total *= thisNumber;

    return total;
}

double factoral (double thisNumber)
{
    double total;  //< The running total.

    if (thisNumber < 0)
    {
        total = thisNumber;

        thisNumber *= -1;

        while (thisNumber-- > 1)
            total *= thisNumber;

        return total * -1;
    }
    total = thisNumber;

    while (thisNumber-- > 1)
        total *= thisNumber;

    return total;
}

int factoral (int thisNumber)
{
    int total;  //< The running total.

    if (thisNumber < 0)
    {
        total = thisNumber;

        thisNumber *= -1;

        while (thisNumber-- > 1)
            total *= thisNumber;

        return total * -1;
    }
    total = thisNumber;

    while (thisNumber-- > 1)
        total *= thisNumber;

    return total;
}

long factoral (long thisNumber)
{
    long total;  //< The running total.

    if (thisNumber < 0)
    {
        total = thisNumber;

        thisNumber *= -1;

        while (thisNumber-- > 1)
            total *= thisNumber;

        return total * -1;
    }
    total = thisNumber;

    while (thisNumber-- > 1)
        total *= thisNumber;

    return total;
}

inline float getMinFloat ()
{
    return FLT_MIN;
}

inline double getMinDouble ()
{
    return DBL_MIN;
}


inline float getMaxFloat ()
{
    return FLT_MAX;
}

inline double getMaxDouble ()
{
    return DBL_MAX;
}

int getNextPrime (int thisNumber)
{

    /** A list of prime numbers between 0 and 2999. */
    int PrimeNumbers[] = { 
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
    static const int NumFastPrimeNumbers = &NumFastPrimeNumbers - &PrimeNumbers[0];

    // This function performs a binary search on the Numbers array to
    // 1.) determine if thisNumber is prime, or
    // 2.) return the next prime number after this number

    if (thisNumber > NumFastPrimeNumbers - 1)
        return 0;

    int index = 0,                              //< The mid index and return index
        leftIndex = 0,                          //< The min search index
        rightIndex = NumFastPrimeNumbers - 1;   //< The max search index

    while (leftIndex < rightIndex)
    {
        index = (leftIndex + rightIndex) / 2;

        if (thisNumber > PrimeNumbers[index])
        {
            leftIndex = index + 1;
        }
        else if (thisNumber < PrimeNumbers[index])
        {
            rightIndex = index - 1;
        }
        else // Then thisNumber is a prime number
            return PrimeNumbers[index];
    }

    // because we checked to see if thisNumber > Numbers[index] first, then were going to over
    // shoot the prime that we want so we return primeNumber[index-1]. Or that's the theory at least

    return PrimeNumbers[index + 1]; // TEST ME!!!
}

Calc::Calc ()
{
    
}
const char* op (Terminal* slot, int index)
{

}


void Calc::print (Terminal& slot)
{
}

}   //< _Math
