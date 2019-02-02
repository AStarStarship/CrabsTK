/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/math/c_calc.h
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
#ifndef KABUKI_FEATURES_MATH_CALC_C
#define KABUKI_FEATURES_MATH_CALC_C 1

namespace _ {

/* Pi as FP4. */
static const FP4  Pi_f = 3.14159265358979f;

/* Pi as double. */
static const double Pi_d = 3.141592653589793238463;

/* Gets the absolute Value of a FP4 Value. */
inline FP4 AbsValue (FP4 a);

/* Gets the absolute Value of the value. */
inline double AbsValue (double a);

/* Gets the absolute Value of an SI4 Value. */
inline SI4 AbsValue (SI4 a);

/* Gets the absolute Value of a long Value. */
inline long AbsValue (long a);

/* Gets the arc cosine of a Value; the returned angle is in the range 0.0 through pi. */
inline FP4 Acos (FP4 a);

/* Gets the arc cosine of a Value; the returned angle is in the range 0.0 through pi. */
inline double Acos (double a);
    
/* Gets the arc sine of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline FP4 Asin (FP4 a);
    
/* Gets the arc sine of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline double Asin (double a);

/* Gets the arc tangent of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline FP4 Atan (FP4 a);

/* Gets the arc tangent of a Value; the returned angle is in the range -pi/2 through pi/2. */
inline double Atan (double a);
    
/* Gets the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta). */
inline FP4 Atan2 (FP4 y, FP4 x);
    
/* Gets the angle theta from the conversion of rectangular coordinates (x, y) to polar coordinates (r, theta). */
inline double Atan2 (double y, double x);
    
/* Gets the smallest (closest to negative infinity) double Value that is greater than or equal to the argument and is equal to a mathematical integer. */
inline FP4 Ceiling (FP4 a);
    
/* Gets the smallest (closest to negative infinity) double Value that is greater than or equal to the argument and is equal to a mathematical integer. */
inline double Ceiling (double a);
    
/* Gets the first floating-point argument with the sign of the second floating-point argument. */
inline FP4 CopySign (FP4 Magnitude, FP4 sign);
    
/* Gets the first floating-point argument with the sign of the second floating-point argument. */
inline double CopySign (double Magnitude, double sign);
    
/* Gets the trigonometric cosine of an angle. */
inline FP4 Cos (FP4 a);
    
/* Gets the trigonometric cosine of an angle. */
inline double Cos (double a);
    
/* Gets the hyperbolic cosine of the value. */
inline FP4 Hcos (FP4 x);

/* Gets the hyperbolic cosine of the value. */
inline double Hcos (double x);

/* Gets Euler's number e raised to the power of the value. */
inline FP4 eToTheX (FP4 a);

/* Gets Euler's number e raised to the power of the value. */
inline double eToTheX (double a);

/* Gets ex -1. */
inline FP4 OneMinus_eToTheX (FP4 x);

/* Gets ex -1. */
inline double OneMinus_eToTheX (double x);

/* Gets the largest (closest to positive infinity) double Value that is less than or equal to the argument and is equal to a mathematical integer. */
inline FP4 Floor (FP4 a);

/* Gets the largest (closest to positive infinity) double Value that is less than or equal to the argument and is equal to a mathematical integer. */
inline double Floor (double a);

/* Gets the unbiased exponent used in the representation of a FP4. */
inline SI4 Exponent (FP4 d);

inline SI4 Exponent (double d);
/* Gets the unbiased exponent used in the representation of a double. */

/* Gets SquareROot (x2 +y2) without intermediate overflow or underflow. */
inline FP4 Hypot (FP4 x, FP4 y);

/* Gets SquareRoot (x2 +y2) without intermediate overflow or underflow. */
inline double Hypot (double x, double y);

/* Computes the remainder operation on two arguments as prescribed by the IEEE 754 standard. */
inline FP4 IeeeRemainder (FP4 f1, FP4 f2);

/* Computes the remainder operation on two arguments as prescribed by the IEEE 754 standard. */
inline double IeeeRemainder (double f1, double f2);

/* Gets the natural logarithm (base e) of the value. */
inline FP4 Ln (FP4 a);

/* Gets the natural logarithm (base e) of the value. */
inline double Ln (double a);

/* Gets the base 10 logarithm of the value. */
inline FP4 Log (FP4 a);

/* Gets the base 10 logarithm of the value. */
inline double Log (double a);

/* Gets the natural logarithm of the sum of the argument and 1. */
inline FP4 LogXPlus1 (FP4 x);

/* Gets the natural logarithm of the sum of the argument and 1. */
inline double LogXPlus1 (double x);

/* Gets the greater of two FP4 Values. */
inline FP4 Max (FP4 a, FP4 b);

/* Gets the greater of two double Values. */
inline double Max (double a, double b);

/* Gets the greater of two SI4 Values. */
inline SI4 Max (SI4 a, SI4 b);

/* Gets the greater of two long Values. */
inline long Max (long a, long b);

/* Gets the smaller of two FP4 Values. */
inline FP4 Min (FP4 a, FP4 b);

/* Gets the smaller of two double Values. */
inline double Min (double a, double b);

/* Gets the smaller of two SI4 Values. */
inline SI4 Min (SI4 a, SI4 b);

/* Gets the smaller of two long Values. */
inline long Min (long a, long b);

/* Gets the floating-point number adjacent to the first argument in the direction of the second argument. */
inline FP4 NextAfter (FP4 start, FP4 direction);

/* Gets the floating-point number adjacent to the first argument in the direction of the second argument. */
inline double NextAfter (double start, double direction);

/*  Gets the floating-point Value adjacent to f in the direction of positive infinity. */
inline FP4 NextUp (FP4 f);

/* Gets the floating-point Value adjacent to d in the direction of positive infinity. */
inline double NextUp (double d);

/* Gets the Value of the first argument raised to the power of the second argument. */
inline FP4 Power (FP4 a, FP4 b);

/* Gets the Value of the first argument raised to the power of the second argument. */
inline double Power (double a, double b);

/* Gets the value with a positive sign, greater than or equal to 0.0 and less than 1.0. */
inline FP4 Random_f ();

/* Gets the value with a positive sign, greater than or equal to 0.0 and less than 1.0. */
inline double Random_d ();

/* Gets the FP4 Value that is closest in Value to the argument and is equal to a mathematical integer. */
inline FP4 Round (FP4 a);
 
/* Gets the double Value that is closest in Value to the argument and is equal to a mathematical integer. */
inline double Round (double a);

/* Gets the closest SI4 to the argument, with ties Rounding up. */
inline SI4 RoundToInt (FP4 a);

/* Gets the closest long to the argument, with ties Rounding up. */
inline long RoundToInt (double a);

/* Return d × 2scaleFactor Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the double Value set. */
inline FP4 ScaleB (FP4 d, SI4 scaleFactor);

/* Return d × 2scaleFactor Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the double Value set. */
inline double ScaleB (double d, SI4 scaleFactor);

/* Return f × 2scaleFactor.
    Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the FP4 Value set. */
inline FP4 FScaleFactor (FP4 f, SI4 scaleFactor);

/* Return f × 2scaleFactor.
    Rounded as if performed by a single correctly Rounded floating-point multiply to a member of the FP4 Value set. */
inline double FScaleFactor (double f, SI4 scaleFactor);

/* Gets the signum function of the argument
    @return 0.0 if the argument is 0.0, 1.0 if the argument is greater than 0, -1.0 if the argument is less than zero. */
inline FP4 SignOf (FP4 f);

/* Gets the signum function of the argument; zero if the argument is zero, 1.0 if the argument is greater than zero, -1.0 if the argument is less than zero. */
inline double SignOf (double d);
    
/* Gets the trigonometric sine of an angle. */
inline FP4 Sin (FP4 a);
    
/* Gets the trigonometric sine of an angle. */
inline double Sin (double a);
    
/* Gets the hyperbolic sine of the value. */
inline FP4 Hsin (FP4 x);
    
/* Gets the hyperbolic sine of the value. */
inline double Hsin (double x);

/* Gets the correctly Rounded positive square root of the value. */
inline FP4 SquareRoot (FP4 a);

/* Gets the correctly Rounded positive square root of the value. */
inline double SquareRoot (double a);
    
/* Gets the cube root of the value. */
inline FP4 CubeRoot (FP4 a);

/* Gets the cube root of the value. */
inline double CubeRoot (double a);

/* Gets the trigonometric tangent of an angle. */
inline FP4 Tan (FP4 a);

/* Gets the trigonometric tangent of an angle. */
inline double Tan (double a);

/* Gets the hyperbolic tangent of the value. */
inline FP4 Htan (FP4 x);

/* Gets the hyperbolic tangent of the value. */
inline double Htan (double x);

/* Converts an angle measured in radians to an approximately equivalent angle measured in degrees. */
inline FP4 ToDegrees (FP4 angrad);

/* Converts an angle measured in radians to an approximately equivalent angle measured in degrees. */
inline double ToDegrees (double angrad);

/* Converts an angle measured in degrees to an approximately equivalent angle measured in radians. */
inline FP4 ToRadians (FP4 angle);

/* Converts an angle measured in degrees to an approximately equivalent angle measured in radians. */
inline double ToRadians (double angle);

/* Gets the size of an ulp of the argument. */
inline FP4 Ulp (FP4 f);
    
/* Gets the size of an ulp of the argument. */
inline double Ulp (double d);

/* Method calculates the factorial of thisNumber */
inline FP4 Factoral (FP4 thisNumber);

/* Method calculates the factorial of thisNumber */
inline double Factoral (double thisNumber);

/* Method calculates the factorial of thisNumber */
inline SI4 Factoral (SI4 thisNumber);

/* Method calculates the factorial of thisNumber */
inline long Factoral (long thisNumber);

/* Gets the min FP4 value. */
inline FP4 GetMinFloat ();

/* Gets the min double value. */
inline double GetMinDouble ();

/* Gets the max FP4 value. */
inline FP4 GetMaxFloat ();

/* Gets the max double value. */
inline double GetMaxDouble ();

/* Gets the next prime number at or after thisNumber.
    @param thisNumber The number find.
    @return Gets thisNumber if thisNumber is prime.
    @return Gets the next prime number after thisNumber if it is not prime.
    @return Gets 0 if thisNumber is greater then the biggest prime number in the Numbers array.
*/
inline SI4 GetNextPrime (SI4 thisNumber);

/* An Inter-process calculator for doing math on distributed systems.
    @todo Map the above functions to the I2P operations.
*/
class Calc : public Operand {
  public:

  /* Constructs a calculator. */
  Calc () {}

  /* Inter-process operations. */
  virtual const Op* Star (CHN index, Expr* expr) { return nullptr; }

  /* Prints this object to the terminal. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    return o << "\nCalc:";
  }
};

}   //  namespace _
#endif
