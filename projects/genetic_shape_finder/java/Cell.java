/* The Leukocyte Wars
@file       ~/Cell.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.Cell class.
*/
package leukocytewars;


import java.awt.*;
import java.awt.Rectangle;
import java.util.Random;
import java.lang.Math;

/* Class represents a cell in the simulation.
*/
public class Cell extends GeneticPolygon
{
    public static final int DefaultWidth  = 50,
                            DefaultHeight = 50,
                            circleBuffer  = 20;
    
    private int numPointsWeakness,
                shapeAngleWeakness,
                colorWeakness;                      //< The color this Cell is weak to.
                                                    /*< Viruses that are of weak to a color will allow a cell to infect it that smaller. */
    
    private double x,                               //< The current x position.
                   y;                               //< The current y position.
    
    private Host host;                              //< A pointer to the host.
    private Virus virus;                            //< The infecting virus.
    private long timeOfInfection;                   //< The time that this cell got infected.
    
    public Cell (Host cellHost)
    {
        super ();
        
        host = cellHost;
    }
    
    public Cell (int xPosition, int yPosition, int thisWidth, int thisHeight, int thisNumPoints, 
                 int thisColorARGB, int thisLifespan, double thisAngle)
    {
        super (thisNumPoints, thisWidth, thisHeight, thisColorARGB, thisLifespan, thisAngle);
        
    }
    
    public void highlight (Graphics g, Color circleColor)
    {
        g.setColor (circleColor);
        g.drawOval (leftEdge () - circleBuffer, topEdge () - circleBuffer,getWidth () + circleBuffer, getHeight () + circleBuffer);
    }
    
    public void update ()
    {
        // We only need to do any logic if the cell is infected. 
        
        if (virus == null)
            return;
    }
    
    public double getX ()
    {
        return x;
    }
    public double getY ()
    {
        return y;
    }
    public int leftEdge ()
    {
        return (int) x;
    }
    public int topEdge ()
    {
        return (int) y;
    }
    public int rightEdge ()
    {
        return (int)x+getWidth ();
    }
    public int bottomEdge ()
    {
        return (int)y+getHeight ();
    }
    
    public void infectCell (Virus thisVirus)
    {
        virus = thisVirus;
        timeOfInfection = System.currentTimeMillis ();
    }
}
