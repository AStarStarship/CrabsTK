/* The Leukocyte Wars
@file       ~/BezierCurve.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http:// opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.BezierCurve class.
*/
package leukocytewars;

import java.awt.geom.Point2D;
 
/* Class that represents a fourth order BezierCurve.  
*/  
public class BezierCurve
{
    public static final int DefaultNumPoints = 4;
    
    public int numPoints;
               
    public double[] xpoints,    //< An array of the 
                    ypoints;
    
    /* Constructor initializes curve with the given points.
    @warning the length of xPoints and yPoints must equal nPoints or curve will not be created correctly.
    */ 
    public BezierCurve (double xPoints[], double yPoints[], int nPoints)
    {
        assert (yPoints != null): "Error creating BezierCurve. xPoings is null.";
        assert (yPoints != null): "Error creating BezierCurve. yPoints is null.";
        
        int tempX = xPoints.length;
        
        assert (tempX != nPoints): "Error creating BezierCurve. nPoints doesn't match the length of the point arrays.";
        
        
        if (yPoints == null || xPoints == null || tempX != yPoints.length || tempX != nPoints)
        {  
            numPoints = 4;
            
            xpoints = new double[4];
            ypoints = new double[4];
            
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
            xpoints = new double[nPoints];
            ypoints = new double[nPoints];
            
            for (int i=0; i < nPoints; i++)
            {
                xpoints[i] = xPoints[i];
                ypoints[i] = yPoints[i];
            }
        }
    }

    /* Constructor initializes curve with the given points.
    */
    public BezierCurve (double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
    {
        numPoints = 4;
        xpoints = new double[4];
        ypoints = new double[4];
        
        xpoints[0] = x0;
        ypoints[0] = y0;
        xpoints[1] = x1;
        ypoints[1] = y1;
        xpoints[2] = x2;
        ypoints[2] = y2;
        xpoints[3] = x3;
        ypoints[3] = y3;
    }

    /* Copy constructor initializes curve with the given points.
    */
    public BezierCurve (BezierCurve initState)
    {
        assert (initState != null): "Error creating BezierCurve using copy constructor. Input was null.";
        
        if (initState != null)
        {
            numPoints = 4;
            
            xpoints = new double[4];
            ypoints = new double[4];
            
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
            
            xpoints = new double[numPoints];
            ypoints = new double[numPoints];
            
            for (int i = 0; i < numPoints; i++)
            {
                xpoints[i] = initState.xpoints[i];
                ypoints[i] = initState.ypoints[i];
            }
        }
    }
    
    // Returns a Point2D along the path of the of curve at time t.
    public Point2D getPoint (double t)
    {
        double x = 0,                           //< The x position to return.
               y = 0,                           //< The y position to return.
               factn = factoral (numPoints);    //< Local copy of n!.
        
        int n = numPoints;                      //< Local copy of numPoints as a double.
        
        for (int i = 0; i <= n; i++)
        {
            double b = factn / (factoral (i) * factoral (n-i)), //< Calculate binomial coefficient
                   k = Math.pow (1-t, n-i) * Math.pow (t, i);   //< Calculate powers
            
            // Add weighted points to totals
            x += b*k*xpoints[i];
            y += b*k*ypoints[i];
        }
        
        return new Point2D.Double (x, y);
    }
    
    private static int factoral (int value)
    {
        // return special case
        if (value==0)
            return 1;
        
        if (value < 0)
            value *= -1;
        
        // Calculate factoral of value
        
        int total = value;
        while (--value > 1)
            total *= value;
        
        return total;
    }
    
    public int getNumPoints ()
    {
        return numPoints;
    }
    
    public String toString ()
    {
        String returnString = new String ("numPoints = " + numPoints + "\n");
        
        for (int i=0; i < numPoints; i++)
        {
            returnString += ("x[" + i + "] = " + xpoints[i] + ", y[" + i + "] = " + ypoints[i] + "\n");
        }
        returnString += ("\n");
        
        return returnString;
    }
}
