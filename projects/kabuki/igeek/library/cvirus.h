/** Cell Evolutionary Interface
    @version   0.x
    @file      /.../CellEvolutionaryInterface/Virus.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                      All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

            http:/www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include "GeneticPolygon.h"
#include "PathTrail.h"

namespace CellEvolutionaryInterface {
 
class Virus: public GeneticPolygon
/* This class represents a GeneticPolygon Virus.
@desc  A virus is a Genetic Polygon

Movement:

Drift Speed: This is a constant speed that is added onto a Virus's movement TArray. This allows Viruses.

Movement of Viruses is define by Bezier curves. There are two distinct patterns of movement:

1.) Linear Movement - This is defined where the movement occurs primarily 

2.) Circular movement - This is where a a Virus moves back and forth through the curve waveform.    
*/
{
  public:
    
    static const SI4 defaultWidth   = 50,                   /< The default width.
        DefaultHeight               = 50,                   /< The default height.
        defaultSpeed                = 20,                   /< The default speed.
        CircleMargin                = 20,                   /< The margin around the 
        CircleMarginX2              = CircleMargin * 2,     /< Twice the margin used to drawOval (...);
        numTrailSegments            = 2;                    /< The number of segments in the trail.
                            
    static const long DefaultReproductionTime   = 10000;    /< The default time to reproduce.
    
    static const DBL minSpeed                = 10.0,     /< The minimum speed in pixels/frame.
        maxSpeed             = 20.0,                        /< This is the maximum speed in pixels/frame.
        defaultPathLength    = 500,                         /< The default length the movement path in seconds
        defaultDeltaT        = 0.01,                        /< The default deltaT???
        trailDeltaT_accuracy = 1000,                        /*< This number represents the number of digits of accuracy.
                                                                Note: Must be an even power of 10 */
        defaultTrailDeltaT   = 1/(2 * numTrailSegments);    /< 
    
    Virus (DBL initX, DBL initY);
    
    Virus (SI4 point_count, SI4 width, SI4 height, SI4 color, 
                  SI4 lifespan, DBL angle);
    
    void initialize ();
    
    DBL velocityX ();
    /*< Returns the instantaneous velocity in the x axis of this object in relation to the prior frame. */
    
    DBL velocityY ();
    /*< Returns the instantaneous velocity in the y axis of this object in relation to the prior frame. */
    
    DBL centerX ();
    
    DBL centerY ();
    
    void collideWith (Virus thatVirus);
    
    void updatePosition (SI4 hostWidth, SI4 hostHeight);
    
    void updateTrail ();
    
    BOL checkHostBoundsCollision (SI4 hostWidth, SI4 hostHeight);
    
    void select ();
    
    void deselect ();

    BOL containsPoint (SI4 thisX, SI4 thisY);
    
    BOL containsPoint (DBL thisX, DBL thisY);
    
    BOL contains (Rectangle thisRectangle);
    
    BOL intersects (Virus thatVirus);
    
    DBL getX ();
    
    DBL getY ();
    
    SI4 Left ();
    
    SI4 Top ();
    
    SI4 rightEdge ();
    
    SI4 bottomEdge ();
        
    BezierCurve getBezierCurve ();
    
    void fillCircle (Graphics g);
    
    void drawCircle (Graphics g);
    
    void drawCircle (Graphics g, Color thisColor);
    
    void draw (Graphics g);
    
    void setTrailColor (Color newColor);
    
    void setDrawTrail (BOL newState);
    
    BOL getDrawTrail ();
    
    void setCircleColor (Color newColor);
    
    void setDrawCircle (BOL newState);
    
    void setCircleFillColor (Color newColor);
    
    void setFillCircle (BOL newState);
    
  private:

    
    SI4 velocityDirection,          /< Variable that indicates which direction the.
        numTailSegments,
        currentIndex;
    
    DBL x,                       /< The current x position.
        y,                       /< The current y position.
        lastX,                   /< The last x position.
                                    /*< Used to calculate collision angles. */
        lastY,                   /< The last y position.
                                    /*< Used to calculate collision angles. */
        lastPathX,               /< The last path in the Bezier Curve.
        lastPathY,               /< 
        pathLength,              /< The scale of the Bezier path.
                                    /*< The Bezier Curve path is a scalar path bounced from 0<x<1 and -1<y<1. This determines how far 1 is. */
        trailDelta,              /< The period during the transition from 0-1 on a Bezier Curve,
                                    /*< The trail that is drawn behind a Polygon is a series of dots that are connected */
        velocity,                /< The velocity of this Virus in pixels per second.
        pathAngle,               /< The degree at which this Virus is moving.
        t,                       /< The current time t in the Bezier curve path.
        deltaT,                  /< The change in time per frame
        x1,                      /< The second x point in the Bezier Curve.
        y1,                      /< The second y point in the Bezier Curve.
        x2,                      /< The third x point in the Bezier Curve.
        y2,                      /< The third y point in the Bezier Curve.
        trailDeltaT,             /< The distance in the time t from 0-1 between the first marker point,
        trailMarkerT;            /< The next value of t to add a point to the trail.
    
    Color trailColor,               /< The Color of the trail to draw.
        outlineColor,             /< The outline color.
        selectedColor;            /< The color of the fill circle.
                  
    BOL drawTrail,              /< Determines whether to draw the trail or not.
        ciricleVirus,           /< Draws a circle a round this Virus.
        selected;               /< Oh shit! I need to stop programming in class!!!!
    
    PathTrail trail;                /< The PathTrail
};

}   //< CellEvolutionaryInterface

