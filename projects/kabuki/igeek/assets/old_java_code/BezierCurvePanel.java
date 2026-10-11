/* The Leukocyte Wars
@file       ~/BezierCurvePanel.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.BezierCurvePanel class.
*/
package leukocytewars;

import java.awt.Color;
import java.awt.Graphics;

/* This class draws a BezierCurve drawn on a JPanel.
@desc  
*/

public class BezierCurvePanel
{
    public static final int defaultNumPoints  = 4,                      //< The default number of points.
                            defaultResolution = 1000,                   //< The default resolution.
                            defaultPadding    = 20,                     //< The default padding.
                            defaultSize      = 200;                     //< The default size of the
    
    public static final double defaultDeltaT = 1/(double)defaultResolution;     //< The default increment in the path to iterate when drawing.
    
    
    public static final Color defaultCurveColor   = Color.red,          //< The default of of the curve.
                              defaultBorderColor  = Color.white;        //< The border color.
    
    
    public BezierCurvePanel (int thisX, int thisY, BezierCurve thisCurve, 
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
    
    public int getNumPoints ()
    {
        return numPoints;
    }
    
    public void draw (Graphics g)
    {
        int centerY = y + height/2 - padding;
        
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
        
        for (int i=0; i < numPoints; i++)
        {
            int currentX = x       + (int)curve.xpoints[i] * width - pointRadius,
                currentY = centerY + (int)curve.ypoints[i] * width - pointRadius;
            
            g.fillOval (currentX, currentY, twicePointRadius, twicePointRadius);
        }
    }
    
    private void drawCubic (Graphics g)
    {
        g.setColor (curveColor);
        
        double centerY = y + height/2 - padding,
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
               
        //System.out.print (curve.toString () + "\n deltaT = " + deltaT + "\n\n");
        
        for (double t = deltaT; t < 1.0; t += deltaT)
        {
            double oneMinus_t = 1.0 - t,
                   oneMinus_t_Squared = Math.pow (oneMinus_t, 2.0),
                   oneMinus_t_Cubed   = Math.pow (oneMinus_t, 3.0),
                   tSquared = Math.pow (t, 2.0),
                   tCubed   = Math.pow (t, 3.0),
                   currentX = x       + width * ((oneMinus_t_Cubed*x0 + 3.0*oneMinus_t_Squared*t*x1+3.0*oneMinus_t*tSquared*x2+tCubed*x3)),
                   currentY = centerY + width * ((oneMinus_t_Cubed*y0 + 3.0*oneMinus_t_Squared*t*y1+3.0*oneMinus_t*tSquared*y2+tCubed*y3));
            
            lastX = currentX;
            lastY = currentY;
            
            //System.out.print ("currentX = " + currentX + ", currentY " + currentY + "\n");
            
            g.drawLine ((int)lastX, (int)lastY, (int)currentX, (int)currentY);
        }
    }
    
    public int numPoints,               //< The number of bezier points.
               resolution,              //< The path resolution.
               padding,                 //< The panel padding.
               doublePadding,           //< Double to padding for calculation acceleration.
               x,                       //< The x position of this panel on the screen.
               y,                       //< The y position of this panel on the screen.
               width,                   //< The width of this panel.
               height,                  //< The height of this panel.
               pointRadius,             //< The radius of the point to draw. 
               twicePointRadius;        //< Twice the pointRaidus.
    
    public double deltaT,               //< The increment in the path to iterate when drawing.
                  curveScale,           //< The scale of the Bezier path.
                                        /*< The Bezier Curve path is a scalar path bounced from 0<x<1 and -1<y<1. This determines how far 1 is. */
                  x1,                   //< The second x point.
                  y1,                   //< The second y point. 
                  x2,                   //<  The third x point.
                  y2;                   //<  The third y point.
    
    public BezierCurve curve;           //< The bezier curve.
    
    public Color backgroundColor,       //< The background color.
                 curveColor,            //< The color of the curve.
                 borderColor,           //< The color of the border.
                 axesColor,             //< The color to draw the axes.
                 pointColor;            //< The color of the points to draw.
    
    public double[] points;             //< The bazier points.
}
