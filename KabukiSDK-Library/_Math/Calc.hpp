/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_Math/Calc.hpp
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

#pragma once

#include <KabukiSDK-Config.hpp>
#include <FreeI2P.hpp>

namespace _Math {

static const float  Pi_f = 3.14159265358979f;
/*< Pi as float. */

static const double Pi_d = 3.141592653589793238463;
/*< Pi as double. */

__Inline float absValue (float a);
/*< Gets the absolute Value of a float Value. */

__Inline double absValue (double a);
/*< Gets the absolute Value of the value. */
    
__Inline int absValue (int a);
/*< Gets the absolute Value of an int Value. */
    
__Inline long absValue (long a);
/*< Gets the absolute Value of a long Value. */
    
__Inline float acos (float a);
/*< Gets the arc cosine of a Value; the returned angle is in the range 0.0 through pi. */
    
__Inline double acos (double a);
/*< Gets the arc cosine of a Value; the returned angle is in the range 0.0 through pi. */
    
__Inline float asin (float a);
/*< Gets the arc sine of a Value; the returned angle is in the range -pi/2 through pi/2. */
    
__Inline double asin (double a);
/*< Gets the arc sine of a Value; the returned angle is in the range -pi/2 through pi/2. */

__Inline float atan (float a);
/*< Gets the arc tangent of a Value; the returned angle is in the range -pi/2 through pi/2. */

__Inline double atan (double a);
/*< Gets the arc tangent of a Value; the returned angle is in the range -pi/2 through pi/2. */
    
__Inline float atan2 (float y, float x);
/*< Gets the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta). */
    
__Inline double atan2 (double y, double x);
/*< Gets the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta). */
    
__Inline float ceiling (float a);
/*< Gets the smallest (closest to negative infinity) double Value that is greater than or equal to the argument and is equal to a mathematical integer. */
    
__Inline double ceiling (double a);
/*< Gets the smallest (closest to negative infinity) double Value that is greater than or equal to the argument and is equal to a mathematical integer. */
    
__Inline float copySign (float magnitude, float sign);
/*< Gets the first floating-point argument with the sign of the second floating-point argument. */
    
__Inline double copySign (double magnitude, double sign);
/*< Gets the first floating-point argument with the sign of the second floating-point argument. */
    
__Inline float cos (float a);
/*< Gets the trigonometric cosine of an angle. */
    
__Inline double cos (double a);
/*< Gets the trigonometric cosine of an angle. */
    
__Inline float hcos (float x);
/*< Gets the hyperbolic cosine of the value. */
    
__Inline double hcos (double x);
/*< Gets the hyperbolic cosine of the value. */

__Inline float eToTheX (float a);
/*< Gets Euler's number e raised to the power of the value. */

__Inline double eToTheX (double a);
/*< Gets Euler's number e raised to the power of the value. */

__Inline float oneMinusEToTheX (float x);
/*< Gets ex -1. */

__Inline double oneMinusEToTheX (double x);
/*< Gets ex -1. */

__Inline float floor (float a);
/*< Gets the largest (closest to positive infinity) double Value that is less than or equal to the argument and is equal to a mathematical integer. */

__Inline double floor (double a);
/*< Gets the largest (closest to positive infinity) double Value that is less than or equal to the argument and is equal to a mathematical integer. */

__Inline int exponent (float d);
/*< Gets the unbiased exponent used in the representation of a float. */

__Inline int exponent (double d);
/*< Gets the unbiased exponent used in the representation of a double. */

__Inline float hypot (float x, float y);
/*< Gets SquareROot (x2 +y2) without intermediate overflow or underflow. */

__Inline double hypot (double x, double y);
/*< Gets SquareRoot (x2 +y2) without intermediate overflow or underflow. */

__Inline float ieeeRemainder (float f1, float f2);
/*< Computes the remainder operation on two arguments as prescribed by the IEEE 754 standard. */

__Inline double ieeeRemainder (double f1, double f2);
/*< Computes the remainder operation on two arguments as prescribed by the IEEE 754 standard. */

__Inline float ln (float a);
/*< Gets the natural logarithm (base e) of the value. */

__Inline double ln (double a);
/*< Gets the natural logarithm (base e) of the value. */

__Inline float log (float a);
/*< Gets the base 10 logarithm of the value. */

__Inline double log (double a);
/*< Gets the base 10 logarithm of the value. */

__Inline float logXPlus1 (float x);
/*< Gets the natural logarithm of the sum of the argument and 1. */

__Inline double logXPlus1 (double x);
/*< Gets the natural logarithm of the sum of the argument and 1. */

__Inline float max (float a, float b);
/*< Gets the greater of two float Values. */

__Inline double max (double a, double b);
/*< Gets the greater of two double Values. */

__Inline int max (int a, int b);
/*< Gets the greater of two int Values. */

__Inline long max (long a, long b);
/*< Gets the greater of two long Values. */

__Inline float min (float a, float b);
/*< Gets the smaller of two float Values. */

__Inline double min (double a, double b);
/*< Gets the smaller of two double Values. */

__Inline int min (int a, int b);
/*< Gets the smaller of two int Values. */

__Inline long min (long a, long b);
/*< Gets the smaller of two long Values. */

__Inline float nextAfter (float start, float direction);
/*< Gets the floating-point number adjacent to the first argument in the direction of the second argument. */

__Inline double nextAfter (double start, double direction);
/*< Gets the floating-point number adjacent to the first argument in the direction of the second argument. */

__Inline float nextUp (float f);
/*<  Gets the floating-point Value adjacent to f in the direction of positive infinity. */

__Inline double nextUp (double d);
/*< Gets the floating-point Value adjacent to d in the direction of positive infinity. */

__Inline float power (float a, float b);
/*< Gets the Value of the first argument raised to the power of the second argument. */

__Inline double power (double a, double b);
/*< Gets the Value of the first argument raised to the power of the second argument. */

__Inline float random_f ();
/*< Gets the value with a positive sign, greater than or equal to 0.0 and less than 1.0. */

__Inline double random_d ();
/*< Gets the value with a positive sign, greater than or equal to 0.0 and less than 1.0. */

__Inline float round (float a);
/*< Gets the float Value that is closest in Value to the argument and is equal to a mathematical integer. */

__Inline double round (double a);
/*< Gets the double Value that is closest in Value to the argument and is equal to a mathematical integer. */

__Inline int roundToInt (float a);
/*< Gets the closest int to the argument, with ties Rounding up. */

__Inline long roundToInt (double a);
/*< Gets the closest long to the argument, with ties Rounding up. */

__Inline float scaleB (float d, int scaleFactor);
/*< Return d × 2scaleFactor Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the double Value set. */

__Inline double scaleB (double d, int scaleFactor);
/*< Return d × 2scaleFactor Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the double Value set. */

__Inline float fScaleFactor (float f, int scaleFactor);
/** Return f × 2scaleFactor.
    Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the float Value set. */

__Inline double fScaleFactor (double f, int scaleFactor);
/** Return f × 2scaleFactor.
    Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the float Value set. */

__Inline float signOf (float f);
/** Gets the signum function of the argument
    @return 0.0 if the argument is 0.0, 1.0 if the argument is greater than 0, -1.0 if the argument is less than zero. */

__Inline double signOf (double d);
/*< Gets the signum function of the argument; zero if the argument is zero, 1.0 if the argument is greater than zero, -1.0 if the argument is less than zero. */
    
__Inline float sin (float a);
/*< Gets the trigonometric sine of an angle. */
    
__Inline double sin (double a);
/*< Gets the trigonometric sine of an angle. */
    
__Inline float hsin (float x);
/*< Gets the hyperbolic sine of the value. */
    
__Inline double hsin (double x);
/*< Gets the hyperbolic sine of the value. */

__Inline float squareRoot (float a);
/*< Gets the correctly Rounded positive square root of the value. */

__Inline double squareRoot (double a);
/*< Gets the correctly Rounded positive square root of the value. */
    
__Inline float cubeRoot (float a);
/*< Gets the cube root of the value. */
    
__Inline double cubeRoot (double a);
/*< Gets the cube root of the value. */
    
__Inline float tan (float a);
/*< Gets the trigonometric tangent of an angle. */
    
__Inline double tan (double a);
/*< Gets the trigonometric tangent of an angle. */
    
__Inline float htan (float x);
/*< Gets the hyperbolic tangent of the value. */
    
__Inline double htan (double x);
/*< Gets the hyperbolic tangent of the value. */

__Inline float toDegrees (float angrad);
/*< Converts an angle measured in radians to an approximately equivalent angle measured in degrees. */

__Inline double toDegrees (double angrad);
/*< Converts an angle measured in radians to an approximately equivalent angle measured in degrees. */

__Inline float toRadians (float angle);
/*< Converts an angle measured in degrees to an approximately equivalent angle measured in radians. */

__Inline double toRadians (double angle);
/*< Converts an angle measured in degrees to an approximately equivalent angle measured in radians. */

__Inline float ulp (float f);
/*< Gets the size of an ulp of the argument. */
    
__Inline double ulp (double d);
/*< Gets the size of an ulp of the argument. */

__Inline float factoral (float thisNumber);
/*< Method calculates the factorial of thisNumber */

__Inline double factoral (double thisNumber);
/*< Method calculates the factorial of thisNumber */

__Inline int factoral (int thisNumber);
/*< Method calculates the factorial of thisNumber */

__Inline long factoral (long thisNumber);
/*< Method calculates the factorial of thisNumber */

__Inline int getNextPrime (int thisNumber);
/*< Gets the next prime number at or after thisNumber.
    @param thisNumber The number find.
    @return Gets thisNumber if thisNumber is prime.
    @return Gets the next prime number after thisNumber if it is not prime.
    @return Gets 0 if thisNumber is greater then the biggest prime number in the Numbers array.
*/

class _KabukiSDK Calc
/*< An Inter-process calculator for doing math on distributed systems.
    
*/
{
    public:
    
    Calc ();
    /*< Constructs a calculator. */

    virtual const char* op (I2P::Terminal* slot, int index);
    /*< Inter-process operations. */

    inline void print (I2P::Terminal& slot);
    /*< Prints this object to the terminal. */
};

}   //  namespace _Math
