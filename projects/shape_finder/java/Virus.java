/* The Leukocyte Wars
@file       ~/Virus.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.Virus class.
*/
package leukocytewars;

/* This class represents a GeneticPolygon Virus.
@desc  A virus is a Genetic Polygon

Movement:

Drift Speed: This is a constant speed that is added onto a Virus's movement vector. This allows Viruses.

Movement of Viruses is define by Bezier curves. There are two distinct patterns of movement:

1.) Linear Movement - This is defined where the movement occurs primarily 

2.) Circular movement - This is where a a Virus moves back and forth through the curve waveform.    
*/

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.util.Random;
import java.lang.Math;
 
public class Virus extends GeneticPolygon
{
    public static final int defaultWidth        = 50,                           //< The default width.
                            DefaultHeight       = 50,                           //< The default height.
                            defaultSpeed        = 20,                           //< The default speed.
                            CircleMargin        = 20,                           //< The margin around the 
                            CircleMarginX2      = CircleMargin * 2,             //< Twice the margin used to drawOval (...);
                            numTrailSegments    = 2;                            //< The number of segments in the trail.
                            
    public static final long DefaultReproductionTime = 10000;                   //< The default time to reproduce.
    
    public static final double minSpeed             = 10.0,                    
                               maxSpeed             = 20.0,                     //< This is the maximum speed in pixels/frame.
                               defaultPathLength    = 500,                      //< The default length the movement path in seconds
                               defaultDeltaT        = 0.01,                     //< The default deltaT???
                               trailDeltaT_accuracy = 1000,                     //< This number represents the number of digits of accuracy.
                                                                                /*< Note: Must be an even power of 10 */
                               defaultTrailDeltaT   = 1/(2 * numTrailSegments); //< 

    
    private int velocityDirection,          //< Variable that indicates which direction the.
                numTailSegments,
                currentIndex;
    
    private double x,                       //< The current x position.
                   y,                       //< The current y position.
                   lastX,                   //< The last x position.
                                            /*< Used to calculate collision angles. */
                   lastY,                   //< The last y position.
                                            /*< Used to calculate collision angles. */
                   lastPathX,               //< The last path in the Bezier Curve.
                   lastPathY,               //< 
                   pathLength,              //< The scale of the Bezier path.
                                            /*< The Bezier Curve path is a scalar path bounced from 0<x<1 and -1<y<1. This determines how far 1 is. */
                   trailDelta,              //< The period during the transition from 0-1 on a Bezier Curve,
                                            /*< The trail that is drawn behind a Polygon is a series of dots that are connected */
                   velocity,                //< The velocity of this Virus in pixels per second.
                   pathAngle,               //< The degree at which this Virus is moving.
                   t,                       //< The current time t in the Bezier curve path.
                   deltaT,                  //< The change in time per frame
                   x1,                      //< The second x point in the Bezier Curve.
                   y1,                      //< The second y point in the Bezier Curve.
                   x2,                      //< The third x point in the Bezier Curve.
                   y2,                      //< The third y point in the Bezier Curve.
                   trailDeltaT,             //< The distance in the time t from 0-1 between the first marker point,
                   trailMarkerT;            //< The next value of t to add a point to the trail.
    
    private Color trailColor,               //< The Color of the trail to draw.
                  outlineColor,             //< The outline color.
                  selectedColor;            //< The color of the fill circle.
                  
    private boolean drawTrail,              //< Determines whether to draw the trail or not.
                    ciricleVirus,           //< Draws a circle a round this Virus.
                    selected;               //< Oh shit! I need to stop programming in class!!!!
    
    private PathTrail trail;                //< The PathTrail
    
    public Virus (double initX, double initY)
    {
        super ();
        
        x = initX;
        y = initY;
        
        initialize ();
    }
    
    public Virus (int thisNumPoints, int thisWidth, int thisHeight, int thisColorARGB, 
                  int thisLifespan, double thisAngle)
    {
        super (thisNumPoints, thisWidth, thisHeight, thisColorARGB, thisLifespan, thisAngle);
        
        x = y = 0;
        
        initialize ();
    }
    
    private void initialize ()
    {
        Random generator = new Random ();
        
        t = lastPathX = lastPathY = 0.0;
        
        deltaT = randomizeDouble (defaultDeltaT);
        pathLength = randomizeDouble (defaultPathLength);
        
        trailDeltaT = defaultTrailDeltaT;
        trailMarkerT = 0;
        
        x1 = generator.nextDouble ();
        y1 = generator.nextDouble ()*2.0-1.0;
        
        x2 = generator.nextDouble ();
        y2 = generator.nextDouble ()*2.0-1.0;
        
        velocity = (maxSpeed-minSpeed)*Math.random ()+ minSpeed;
        
        pathAngle = generator.nextDouble ()*2*Math.PI;
        
        velocityDirection = 0;
        
        drawTrail = false;
        ciricleVirus = false;
        
        trailColor = Color.red;
        outlineColor = Color.blue;
        selectedColor = Color.green;
        
        trail = new PathTrail (trailColor);
        //< These variables are currently unused
    }
    
    // Returns the instantaneous velocity in the x axis of this object in relation to the prior frame.
    public double velocityX ()
    {
        return x-lastX;
    }
    
    // Returns the instantaneous velocity in the y axis of this object in relation to the prior frame.
    public double velocityY ()
    {
        return y-lastY;
    }
    
    public double centerX ()
    {
        return x + getWidth ()/2;
    }
    public double centerY ()
    {
        return y + getHeight ()/2;
    }
    
    public void collideWith (Virus thatVirus)
    {
        if (!intersects (thatVirus))
            return;
        
        // Remember this formula from physics?
        // m1v1 + m2v2 = m1v1' + m2v2'
        // Well guess what, were going to use it!
        
        // Research more about elastic collisions on Wikipedia 
        // http://en.wikipedia.org/wiki/Elastic_collision
        
        // First we need to find the angle betwen the two movement vectors.
        
        // This is defined as Cos(theta) = dotProduct(a, b)/(magnitude(a)*magnitude(b))
        
        double m1 = (double) getNumPixels(),
               m2 = (double) thatVirus.getNumPixels(),
               sumOfMasses = m1 + m2,
               m1_minus_m2 = m1 - m2,
               m2_minus_m1 = m2 - m1,
               ux1 = velocityX (),
               uy1 = velocityY (),
               ux2 = thatVirus.velocityX (),
               uy2 = thatVirus.velocityY (),
               vx1 = (ux1*m1_minus_m2 + 2*m2*ux2)/sumOfMasses,
               vy1 = (uy1*m1_minus_m2 + 2*m2*uy2)/sumOfMasses,
               vx2 = (ux2*m2_minus_m1 + 2*m1*ux1)/sumOfMasses, //  |   /|
               vy2 = (uy2*m2_minus_m1 + 2*m1*uy1)/sumOfMasses, //  |  / |
               newAngle1 = Math.atan (vy1/vx1), // SOH CAH TOA     | /T | vy
               newAngle2 = Math.atan (vy2/vx2); // O=vy A=vx    ___|/___|
                                           // T=Theta                 vx
        pathAngle = newAngle1;
        thatVirus.pathAngle = newAngle2;
    }
        
    public void updatePosition (int hostWidth, int hostHeight)
    {
        // The movement of the virus is based off of Bezier curves. The curve has two given points, a start at 
        // coordinates (0,0), and an end at coordinates (1,0). Points two and three are bounded by 
        // { 0 <= x <= 1 } and { 1 <= y >= -1}.
        
        //System.out.print ("t=" + t + "\n");
        t += deltaT;                // Increment the current time in the path.
        
        if (t < 0) // This shouldn't happen.
        {
            t = 0;
            lastPathX = lastPathY = 0;
        }
        else if (t > 1.0)
        {
            t = t-1.0;
            lastPathX -= 1.0;
        }
        
        double oneMinus_t = 1.0-t,
               oneMinus_t_Squared = Math.pow (oneMinus_t, 2.0),
               oneMinus_t_Cubed = Math.pow (oneMinus_t, 3.0),
               tSquared = Math.pow (t, 2.0),
               tCubed = Math.pow (t, 3.0),
               newPathX = 3.0*oneMinus_t_Squared*t*x1 + 3.0*oneMinus_t*tSquared*x2 + tCubed,
               newPathY = 3.0*oneMinus_t_Squared*t*y1 + 3.0*oneMinus_t*tSquared*y2,
               // Note the above funtions are optimized for this vunction because x0, y0, y3 will always 0 and 
               // x3 will always be 1. The following two lines are th eoriginal funtions.
               //newPathX = oneMinus_t_Cubed*x0 + 3.0*oneMinus_t_Squared*t*x1 + 3.0*oneMinus_t*tSquared*x2 + tCubed*x3,
               //newPathY = oneMinus_t_Cubed*y0 + 3.0*oneMinus_t_Squared*t*y1 + 3.0*oneMinus_t*tSquared*y2 + tCubed*y3, // Origital Function
               deltaX = (newPathX - lastPathX)*pathLength,
               deltaY = (newPathY - lastPathY)*pathLength,
               sinTheta = Math.sin (pathAngle),
               cosTheta = Math.cos (pathAngle);
        
        lastPathX = newPathX;
        lastPathY = newPathY;
        
        lastX = x;
        lastY = y;
        
        // Translate the movement vector deltaX and deltaY around the circle theta degrees and add to the x and y positions.
        
        x += deltaX*cosTheta - deltaY*sinTheta;
        y += deltaX*sinTheta + deltaY*cosTheta;
        
        //System.out.print ("\nt = " + t + ", velocity = " + velocity + ", pathLength = " + pathLength + "\n");
        //System.out.print ("newPathX = " + newPathX + ", newPathY = " + newPathY + "\n");
        //System.out.print ("lastPathX = " + lastPathX + ", lastPathY = " + lastPathY + "\n");
        //System.out.print ("deltaX = " + deltaX + ", deltaY = " + deltaY + "\n");
        //System.out.print ("lastX = " + lastX + ", lastY = " + lastY + "\n");
        //System.out.print ("x = " + x + ", y = " + y + "\n");
        
        checkHostBoundsCollision (hostWidth, hostHeight);
        
        if (drawTrail)
            updateTrail ();
    }
    
    public void updateTrail ()
    {
        if (t < trailMarkerT)
            return;
     // else
        trailMarkerT += trailDeltaT;
        
        if (trailMarkerT > 1)
            trailMarkerT -= 1;
        
        trail.addPoint ((int) centerX (), (int) centerY ());
    }
    
    public boolean checkHostBoundsCollision (int hostWidth, int hostHeight)
    {
        double dx = x - lastX,
               dy = y - lastY;
        
        boolean collisionDetected;
        
        if (x < 0 || (int)x + getWidth () > hostWidth)
        {
            dx *= -1;
            collisionDetected = true;
        }
        else if (y < 0 || (int)y + getHeight () > hostHeight)
        {
            dy *= -1;
            collisionDetected = true;
        }
        else
            collisionDetected = false;
        
        if (collisionDetected)
        {
            // Because we are using the Bezier Curve paths, if the Virus collides with the wall, the pathAngle
            // needs to be adjustest so that the two collision results in a smooth transition of angles.
            // We need to find out what is the initial angle for the first movement in the Bezier curve.
            
            double t_plus_deltaT = t + deltaT,
                   oneMinus_t = 1.0-t_plus_deltaT,
                   oneMinus_t_Squared = Math.pow (oneMinus_t, 2.0),
                   oneMinus_t_Cubed = Math.pow (oneMinus_t, 3.0),
                   tSquared = Math.pow (t_plus_deltaT, 2.0),
                   tCubed = Math.pow (t_plus_deltaT, 3.0),
                   newPathX = 3.0*oneMinus_t_Squared*t_plus_deltaT*x1 + 3.0*oneMinus_t*tSquared*x2 + tCubed,
                   newPathY = 3.0*oneMinus_t_Squared*t_plus_deltaT*y1 + 3.0*oneMinus_t*tSquared*y2,
                   nextPathDX = newPathX - lastPathX,
                   nextPathDY = newPathY - lastPathY;
            
            pathAngle = Math.atan (dy/dx) + Math.atan (nextPathDX/nextPathDY); // TOA -> y/x
        }
        
        return collisionDetected;
    }
    
    public void select ()
    {
        selected = true;
    }
    
    public void deselect ()
    {
        selected = false;
    }

    public boolean containsPoint (int thisX, int thisY)
    {
        return containsPoint ((double)thisX, (double)thisY);
    }
    
    public boolean containsPoint (double thisX, double thisY)
    {
        double polyX = thisX - x,
               polyY = thisY - y;
        
        if (   thisX < x || thisX > rightEdge  ()
            || thisY < y || thisY > bottomEdge ())
            return false;
        
        //System.out.print ("\nx = " + x + ", y = " + y + "\n");
        //System.out.print ("thisX = " + thisX + ", thisY = " + thisY + "\n");
        //System.out.print ("polyX = " + polyX + ", polyY = " + polyY + "\n");
        //System.out.print ("left = " + x + ", top = " + y + ", right = " + rightEdge () + ", bottom = " + bottomEdge () + "\n\n");
        
        return super.contains (polyX, polyY);
    }
    
    public boolean contains (Rectangle thisRectangle)
    {
        if (   thisRectangle.x + getWidth  () < x || thisRectangle.x > x + getWidth ()
            || thisRectangle.y + getHeight () < y || thisRectangle.y > y + getHeight ())
            return false;
        return true;
    }
    
    public boolean intersects (Virus thatVirus)
    {
        if (   thatVirus == null 
            || x > thatVirus.rightEdge ()
            || y > thatVirus.bottomEdge ()
            || rightEdge  () < thatVirus.x
            || bottomEdge () < thatVirus.y)
            return false;
        
        // else the rectangles occlude eachother.
        
        return super.intersects (thatVirus, thatVirus.x-x, thatVirus.y-y);
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
    
    public BezierCurve getBezierCurve ()
    {
        return new BezierCurve (0, 0, x1, y1, x2, y2, 1, 0);
    }
    
    public void fillCircle (Graphics g)
    {   
        g.setColor (selectedColor);
        g.fillOval (leftEdge () - CircleMargin  , topEdge   () - CircleMargin,
                    getWidth () + CircleMarginX2, getHeight () + CircleMarginX2);
    }
    
    public void drawCircle (Graphics g)
    {
        g.setColor (outlineColor);
        g.drawOval (leftEdge () - CircleMargin, topEdge () - CircleMargin,
                    getWidth () + CircleMargin, getHeight () + CircleMargin);
    }
    
    public void drawCircle (Graphics g, Color thisColor)
    {
        g.setColor (thisColor);
        g.drawOval (leftEdge () - CircleMargin, topEdge ()   - CircleMargin, 
                    getWidth () + CircleMargin, getHeight () + CircleMargin);
    }
    
    public void draw (Graphics g)
    {
        if (drawTrail)
            trail.draw (g);
        
        if (selected)
            fillCircle (g);
            
        g.drawImage(getBitmap (), leftEdge (), topEdge (), null);
        
        if (ciricleVirus)
            drawCircle (g);
    }
    
    public void setTrailColor (Color newColor)
    {
        trailColor = newColor;
    }
    public void setDrawTrail (boolean newState)
    {
        drawTrail = newState;
    }
    public boolean getDrawTrail ()
    {
        return drawTrail;
    }
    
    public void setCircleColor (Color newColor)
    {
        selectedColor = newColor;
    }
    public void setDrawCircle (boolean newState)
    {
        ciricleVirus = newState;
    }
    
    public void setCircleFillColor (Color newColor)
    {
        outlineColor = newColor;
    }
    public void setFillCircle (boolean newState)
    {
        selected = newState;
    }
}
