/* The Leukocyte Wars
@file       ~/VirusPopulation.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the GeneticPolygonTestPanel class.
@desc       This file contains the leukocytewars.VirusPopulation class.
*/
package leukocytewars;

import java.awt.Color;
import java.awt.Graphics;


/*  @class  PathTrail
    @brief  A class that represents a trail of lines.
    @desc   This class is represented by a circular cue of 2 dimensionsal points.
 */
 
 public class PathTrail
 {
    public static final Color DefaultTrailColor = Color.red;        // The default PathTrail color.
    
    public static final int DefaultMaxNumPoints = 300;               // In order to make a line, we need two points.
    
    private Color trailColor;               //< The Color of this PathTrail.
    
    private int numPoints,                  //< The number of points.
                startPoint,                 //< The current startPoint in the circular queue.
                stopPoint,                   //< The current stopPoint in the circular queue.
                maxNumPoints;               //< The maximum number of points.
                
    private int xpoints[],                  //< The x values for the points.
                ypoints[];                  //< The y values for the points.
    
    public PathTrail ()
    {
        trailColor   = DefaultTrailColor;
        maxNumPoints = DefaultMaxNumPoints;
        
        initialize ();
    }
    
    public PathTrail (Color thisTrailColor)
    {
        if (thisTrailColor == null)
            trailColor = DefaultTrailColor;
        else
            trailColor = thisTrailColor;
            
        maxNumPoints = DefaultMaxNumPoints;
            
        initialize ();
    }
    public PathTrail (Color thisTrailColor, int thisMaxNumPoints)
    {
        if (thisTrailColor == null)
            trailColor = DefaultTrailColor;
        else
            trailColor = thisTrailColor;
            
        if (thisMaxNumPoints < 0)
            maxNumPoints = DefaultMaxNumPoints;
        else
            maxNumPoints = thisMaxNumPoints;
        
        initialize ();
    }
    
    private void initialize ()
    {
        startPoint = stopPoint = numPoints = 0;
        
        xpoints = new int[maxNumPoints];
        ypoints = new int[maxNumPoints];
    }
    
    public void setTrailColor (Color newColor)
    {
        if (newColor != null)
            trailColor = newColor;
    }
    
    public Color getTrailColor ()
    {
        return trailColor;
    }
    
    public void draw (Graphics g)
    {
        //System.out.print (toString ());
        g.setColor (trailColor);
        
        for (int i = startPoint; i < stopPoint-1; ++i)
        {
            if (i >= maxNumPoints)
                 i = 0;
            
            g.drawLine (xpoints[i], ypoints[i], xpoints[i+1], ypoints [i+1]);
        }
        for (int i = 0; i <= stopPoint-2; ++i)
        {
            g.drawLine (xpoints[i], ypoints[i], xpoints[i+1], ypoints [i+1]);
        }
    }
    
    public void addPoint (int x, int y)
    {
        //System.out.print ("\nAdding point { x = " + x + ", y = " + y + " }\n");
        
        if (numPoints < maxNumPoints)
            ++numPoints;
        
        // This is a circular que. This means that when we add at first, the stopIndex is going to increment
        // until we hit the maxNumPoints, then the startIndex and stopIndex will increment and loop around such
        // that startIndex = stopIndex + 1.
        
        xpoints[stopPoint] = x;
        ypoints[stopPoint] = y;
        
        if (++stopPoint >= maxNumPoints)
            stopPoint = 0;
        
        if (stopPoint == startPoint)
            startPoint++;
            
        if (startPoint >= maxNumPoints)
            startPoint = 0;
        
        //System.out.print (toString ());
    }
    
    public String toString ()
    {
        String returnString = "PathTrail: numPoints = " + numPoints + ", startPoint = " + startPoint + ", stopPoint = " + stopPoint + "\n";
        
        for (int i = 0; i < numPoints; i++)
            returnString +=  i + ".) x = " + xpoints[i] + ", y = " + ypoints[i] + "\n";
        
        return returnString;
    }
 }