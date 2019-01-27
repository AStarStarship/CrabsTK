/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/library/igeek_bezierpanel.cc
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

#include "cbezier.h"
 
BezierCurvePanel::BezierCurvePanel (SI4 thisX, SI4 thisY, BezierCurve thisCurve, 
    Color thisBackgroundColor, Color thisCurveColor, Color thisBorderColor)
{
    x = thisX;
    y = thisY;
    
    width  = defaultSize;
    height = defaultSize;
    
    curve = new BezierCurve (thisCurve);
    
    padding = defaultPadding;
    doublePadding = 2*padding;
    
    numPoints = defaultNumPoints;
    pointRadius = 3;
    twicePointRadius = 2*pointRadius;
    
    resolution = defaultResolution;
    deltaT = defaultDeltaT;
    
    backgroundColor = thisBackgroundColor;
    curveColor = thisCurveColor;
    borderColor = thisBorderColor;
    axesColor = Color.gray;
}

SI4 BezierCurvePanel::getNumPoints ()
{
    return numPoints;
}

void BezierCurvePanel::draw (Graphics g)
{
    SI4 centerY = y + height/2 - padding;
    
    g.setColor (backgroundColor);
    g.fillRect (x-padding, y-padding, width+doublePadding, height+doublePadding);
    
    g.setColor (borderColor);
    g.drawRect (x-padding, y-padding, width+doublePadding, height+doublePadding);
    
    g.setColor (axesColor);
    g.drawLine (x, centerY, x+width, centerY);
    g.drawLine (x, y, x, y + height);
    
    if (curve.numPoints == 4)
        drawCubic (g);
     
    g.setColor (pointColor);
    
    for (SI4 i=0; i < numPoints; i++)
    {
        SI4 currentX = x       + (SI4)curve.xpoints[i] * width - pointRadius,
            currentY = centerY + (SI4)curve.ypoints[i] * width - pointRadius;
        
        g.fillOval (currentX, currentY, twicePointRadius, twicePointRadius);
    }
}

void BezierCurvePanel::drawCubic (Graphics g)
{
    g.setColor (curveColor);
    
    DBL centerY = y + height/2 - padding,
           lastX = 0,
           lastY = 0,
           x0 = curve.xpoints[0],
           y0 = curve.ypoints[0],
           x1 = curve.xpoints[1],
           y1 = curve.ypoints[1],
           x2 = curve.xpoints[2],
           y2 = curve.ypoints[2],
           x3 = curve.xpoints[3],
           y3 = curve.ypoints[3];
           
    //Print (curve.toString () + "\n deltaT = " + deltaT + "\n\n");
    
    for (DBL t = deltaT; t < 1.0; t += deltaT)
    {
        DBL oneMinus_t = 1.0 - t,
               oneMinus_t_Squared = Math.pow (oneMinus_t, 2.0),
               oneMinus_t_Cubed   = Math.pow (oneMinus_t, 3.0),
               tSquared = Math.pow (t, 2.0),
               tCubed   = Math.pow (t, 3.0),
               currentX = x       + width * ((oneMinus_t_Cubed*x0 + 3.0*oneMinus_t_Squared*t*x1+3.0*oneMinus_t*tSquared*x2+tCubed*x3)),
               currentY = centerY + width * ((oneMinus_t_Cubed*y0 + 3.0*oneMinus_t_Squared*t*y1+3.0*oneMinus_t*tSquared*y2+tCubed*y3));
        
        lastX = currentX;
        lastY = currentY;
        
        //Print ("currentX = " + currentX + ", currentY " + currentY + "\n");
        
        g.drawLine ((SI4)lastX, (SI4)lastY, (SI4)currentX, (SI4)currentY);
    }
}

}   //< namespace kabuki { namespace igeek {
