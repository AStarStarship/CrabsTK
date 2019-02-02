/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/library/igeek_bezier.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "cbezier.h"

namespace kabuki {
namespace igeek {

BezierCurve::BezierCurve (FP8 xPoints[], FP8 yPoints[], SI4 nPoints)
{
  DASSERT (yPoints != null) : "Error creating BezierCurve. xPoings is null.";
  DASSERT (yPoints != null) : "Error creating BezierCurve. yPoints is null.";

  SI4 tempX = xPoints.length;

  DASSERT (tempX != nPoints) : "Error creating BezierCurve. nPoints doesn't match the length of the point arrays.";


  if (yPoints == null || xPoints == null || tempX != yPoints.length || tempX != nPoints)
  {
    numPoints = 4;

    xpoints = new FP8[4];
    ypoints = new FP8[4];

    xpoints[0] = 0;
    ypoints[0] = 0;
    xpoints[1] = 0;
    ypoints[1] = 1;
    xpoints[2] = 1;
    ypoints[2] = -1;
    xpoints[3] = 1;
    ypoints[3] = 0;
  }
  else
  {
    numPoints = nPoints;
    xpoints = new FP8[nPoints];
    ypoints = new FP8[nPoints];

    for (SI4 i = 0; i < nPoints; i++)
    {
      xpoints[i] = xPoints[i];
      ypoints[i] = yPoints[i];
    }
  }
}

BezierCurve::BezierCurve (FP8 x0, FP8 y0, FP8 x1, FP8 y1, FP8 x2, FP8 y2, FP8 x3, FP8 y3)
{
  numPoints = 4;
  xpoints = new FP8[4];
  ypoints = new FP8[4];

  xpoints[0] = x0;
  ypoints[0] = y0;
  xpoints[1] = x1;
  ypoints[1] = y1;
  xpoints[2] = x2;
  ypoints[2] = y2;
  xpoints[3] = x3;
  ypoints[3] = y3;
}

BezierCurve::BezierCurve (BezierCurve initState)
{
  DASSERT (initState != null) : "Error creating BezierCurve using copy constructor. Input was null.";

  if (initState != null)
  {
    numPoints = 4;

    xpoints = new FP8[4];
    ypoints = new FP8[4];

    xpoints[0] = 0;
    ypoints[0] = 0;
    xpoints[1] = 0;
    ypoints[1] = 1;
    xpoints[2] = 1;
    ypoints[2] = -1;
    xpoints[3] = 1;
    ypoints[3] = 0;
  }
  else
  {
    numPoints = initState.numPoints;

    xpoints = new FP8[numPoints];
    ypoints = new FP8[numPoints];

    for (SI4 i = 0; i < numPoints; i++)
    {
      xpoints[i] = initState.xpoints[i];
      ypoints[i] = initState.ypoints[i];
    }
  }
}

Point2D BezierCurve::getPoint (FP8 t)
{
  FP8 x = 0,                           //< The x position to return.
    y = 0,                           //< The y position to return.
    factn = factoral (numPoints);    //< Local copy of n!.

  SI4 n = numPoints;                      //< Local copy of numPoints as a FP8.

  for (SI4 i = 0; i <= n; i++)
  {
    FP8 b = factn / (factoral (i) * factoral (n - i)), //< Calculate binomial coefficient
      k = Math.pow (1 - t, n - i) * Math.pow (t, i);   //< Calculate powers

// Add weighted points to totals
    x += b * k*xpoints[i];
    y += b * k*ypoints[i];
  }

  return new Point2D.Double (x, y);
}

static SI4 BezierCurve::factoral (SI4 value)
{
  // return special case
  if (value == 0)
    return 1;

  if (value < 0)
    value *= -1;

  // Calculate factoral of value

  SI4 total = value;
  while (--value > 1)
    total *= value;

  return total;
}

SI4 BezierCurve::getNumPoints ()
{
  return numPoints;
}

String BezierCurve::toString ()
{
  String returnString = new String ("numPoints = " + numPoints + "\n");

  for (SI4 i = 0; i < numPoints; i++)
  {
    returnString += ("x[" + i + "] = " + xpoints[i] + ", y[" + i + "] = " + ypoints[i] + "\n");
  }
  returnString += ("\n");

  return returnString;
}
}   //< namespace igeek
}   //< namespace kabuki
