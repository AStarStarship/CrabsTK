/** Kabuki Toolkit
    @file    ~/kt/math/calc.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
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
#include "config.h"


namespace math {

/** Pi as float. */
static const float  Pi_f = 3.14159265358979f;

/** Pi as double. */
static const double Pi_d = 3.141592653589793238463;

/** Gets the absolute Value of a float Value. */
inline float AbsValue (float a);

/** Gets the absolute Value of the value. */
inline double AbsValue (double a);

/** Gets the absolute Value of an int Value. */
inline int AbsValue (int a);

/** Gets the absolute Value of a long Value. */
inline long AbsValue (long a);

/** Gets the arc cosine of a Value; the returned angle is in the range 0.0 through pi. */
inline float Acos (float a);

/** Gets the arc cosine of a Value; the returned angle is in the range 0.0 through pi. */
inline double Acos (double a);
    
/** Gets the arc sine of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline float Asin (float a);
    
/** Gets the arc sine of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline double Asin (double a);

/** Gets the arc tangent of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline float Atan (float a);

/** Gets the arc tangent of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline double Atan (double a);
    
/** Gets the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta). */
inline float Atan2 (float y, float x);
    
/** Gets the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta). */
inline double Atan2 (double y, double x);
    
/** Gets the smallest (closest to negative infinity) double Value that is greater than or equal to the argument and is equal to a mathematical integer. */
inline float Ceiling (float a);
    
/** Gets the smallest (closest to negative infinity) double Value that is greater than or equal to the argument and is equal to a mathematical integer. */
inline double Ceiling (double a);
    
/** Gets the first floating-point argument with the sign of the second floating-point argument. */
inline float CopySign (float magnitude, float sign);
    
/** Gets the first floating-point argument with the sign of the second floating-point argument. */
inline double CopySign (double magnitude, double sign);
    
/** Gets the trigonometric cosine of an angle. */
inline float Cos (float a);
    
/** Gets the trigonometric cosine of an angle. */
inline double Cos (double a);
    
/** Gets the hyperbolic cosine of the value. */
inline float Hcos (float x);

/** Gets the hyperbolic cosine of the value. */
inline double Hcos (double x);

/** Gets Euler's number e raised to the power of the value. */
inline float eToTheX (float a);

/** Gets Euler's number e raised to the power of the value. */
inline double eToTheX (double a);

/** Gets ex -1. */
inline float OneMinus_eToTheX (float x);

/** Gets ex -1. */
inline double OneMinus_eToTheX (double x);

/** Gets the largest (closest to positive infinity) double Value that is less than or equal to the argument and is equal to a mathematical integer. */
inline float Floor (float a);

/** Gets the largest (closest to positive infinity) double Value that is less than or equal to the argument and is equal to a mathematical integer. */
inline double Floor (double a);

/** Gets the unbiased exponent used in the representation of a float. */
inline int Exponent (float d);

inline int Exponent (double d);
/** Gets the unbiased exponent used in the representation of a double. */

/** Gets SquareROot (x2 +y2) without intermediate overflow or underflow. */
inline float Hypot (float x, float y);

/** Gets SquareRoot (x2 +y2) without intermediate overflow or underflow. */
inline double Hypot (double x, double y);

/** Computes the remainder operation on two arguments as prescribed by the IEEE 754 standard. */
inline float IeeeRemainder (float f1, float f2);

/** Computes the remainder operation on two arguments as prescribed by the IEEE 754 standard. */
inline double IeeeRemainder (double f1, double f2);

/** Gets the natural logarithm (base e) of the value. */
inline float Ln (float a);

/** Gets the natural logarithm (base e) of the value. */
inline double Ln (double a);

/** Gets the base 10 logarithm of the value. */
inline float Log (float a);

/** Gets the base 10 logarithm of the value. */
inline double Log (double a);

/** Gets the natural logarithm of the sum of the argument and 1. */
inline float LogXPlus1 (float x);

/** Gets the natural logarithm of the sum of the argument and 1. */
inline double LogXPlus1 (double x);

/** Gets the greater of two float Values. */
inline float Max (float a, float b);

/** Gets the greater of two double Values. */
inline double Max (double a, double b);

/** Gets the greater of two int Values. */
inline int Max (int a, int b);

/** Gets the greater of two long Values. */
inline long Max (long a, long b);

/** Gets the smaller of two float Values. */
inline float Min (float a, float b);

/** Gets the smaller of two double Values. */
inline double Min (double a, double b);

/** Gets the smaller of two int Values. */
inline int Min (int a, int b);

/** Gets the smaller of two long Values. */
inline long Min (long a, long b);

/** Gets the floating-point number adjacent to the first argument in the direction of the second argument. */
inline float NextAfter (float start, float direction);

/** Gets the floating-point number adjacent to the first argument in the direction of the second argument. */
inline double NextAfter (double start, double direction);

/**  Gets the floating-point Value adjacent to f in the direction of positive infinity. */
inline float NextUp (float f);

/** Gets the floating-point Value adjacent to d in the direction of positive infinity. */
inline double NextUp (double d);

/** Gets the Value of the first argument raised to the power of the second argument. */
inline float Power (float a, float b);

/** Gets the Value of the first argument raised to the power of the second argument. */
inline double Power (double a, double b);

/** Gets the value with a positive sign, greater than or equal to 0.0 and less than 1.0. */
inline float Random_f ();

/** Gets the value with a positive sign, greater than or equal to 0.0 and less than 1.0. */
inline double Random_d ();

/** Gets the float Value that is closest in Value to the argument and is equal to a mathematical integer. */
inline float Round (float a);
 
/** Gets the double Value that is closest in Value to the argument and is equal to a mathematical integer. */
inline double Round (double a);

/** Gets the closest int to the argument, with ties Rounding up. */
inline int RoundToInt (float a);

/** Gets the closest long to the argument, with ties Rounding up. */
inline long RoundToInt (double a);

/** Return d × 2scaleFactor Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the double Value set. */
inline float ScaleB (float d, int scaleFactor);

/** Return d × 2scaleFactor Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the double Value set. */
inline double ScaleB (double d, int scaleFactor);

/** Return f × 2scaleFactor.
    Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the float Value set. */
inline float FScaleFactor (float f, int scaleFactor);

/** Return f × 2scaleFactor.
    Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the float Value set. */
inline double FScaleFactor (double f, int scaleFactor);

/** Gets the signum function of the argument
    @return 0.0 if the argument is 0.0, 1.0 if the argument is greater than 0, -1.0 if the argument is less than zero. */
inline float SignOf (float f);

/** Gets the signum function of the argument; zero if the argument is zero, 1.0 if the argument is greater than zero, -1.0 if the argument is less than zero. */
inline double SignOf (double d);
    
/** Gets the trigonometric sine of an angle. */
inline float Sin (float a);
    
/** Gets the trigonometric sine of an angle. */
inline double Sin (double a);
    
/** Gets the hyperbolic sine of the value. */
inline float Hsin (float x);
    
/** Gets the hyperbolic sine of the value. */
inline double Hsin (double x);

/** Gets the correctly Rounded positive square root of the value. */
inline float SquareRoot (float a);

/** Gets the correctly Rounded positive square root of the value. */
inline double SquareRoot (double a);
    
/** Gets the cube root of the value. */
inline float CubeRoot (float a);

/** Gets the cube root of the value. */
inline double CubeRoot (double a);

/** Gets the trigonometric tangent of an angle. */
inline float Tan (float a);

/** Gets the trigonometric tangent of an angle. */
inline double Tan (double a);

/** Gets the hyperbolic tangent of the value. */
inline float Htan (float x);

/** Gets the hyperbolic tangent of the value. */
inline double Htan (double x);

/** Converts an angle measured in radians to an approximately equivalent angle measured in degrees. */
inline float ToDegrees (float angrad);

/** Converts an angle measured in radians to an approximately equivalent angle measured in degrees. */
inline double ToDegrees (double angrad);

/** Converts an angle measured in degrees to an approximately equivalent angle measured in radians. */
inline float ToRadians (float angle);

/** Converts an angle measured in degrees to an approximately equivalent angle measured in radians. */
inline double ToRadians (double angle);

/** Gets the size of an ulp of the argument. */
inline float Ulp (float f);
    
/** Gets the size of an ulp of the argument. */
inline double Ulp (double d);

/** Method calculates the factorial of thisNumber */
inline float Factoral (float thisNumber);

/** Method calculates the factorial of thisNumber */
inline double Factoral (double thisNumber);

/** Method calculates the factorial of thisNumber */
inline int Factoral (int thisNumber);

/** Method calculates the factorial of thisNumber */
inline long Factoral (long thisNumber);

/** Gets the min float value. */
inline float GetMinFloat ();

/** Gets the min double value. */
inline double GetMinDouble ();

/** Gets the max float value. */
inline float GetMaxFloat ();

/** Gets the max double value. */
inline double GetMaxDouble ();

/** Gets the next prime number at or after thisNumber.
    @param thisNumber The number find.
    @return Gets thisNumber if thisNumber is prime.
    @return Gets the next prime number after thisNumber if it is not prime.
    @return Gets 0 if thisNumber is greater then the biggest prime number in the Numbers array.
*/
inline int GetNextPrime (int thisNumber);

/** An Inter-process calculator for doing math on distributed systems.
    @todo Map the above functions to the I2P operations.
*/
class KABUKI_TOOLKIT Calc : public _::Operand {
    public:

    /** Constructs a calculator. */
    Calc ();

    /** Inter-process operations. */
    virtual const _::Op* Star (wchar_t index, _::Expr* expr);

    /** Prints this object to the terminal. */
    _::Printer& Print (_::Printer& printer);
};

}   //  namespace math
