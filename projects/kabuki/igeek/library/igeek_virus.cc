/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/library/igeek_.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "cvirus.h"

namespace kabuki {
namespace igeek {

Virus::Virus (DBL initX, DBL initY)
{
  super ();

  x = initX;
  y = initY;

  initialize ();
}

Virus::Virus (SI4 point_count, SI4 width, SI4 height, SI4 color,
  SI4 lifespan, DBL angle)
  : GeneticPolygon (point_count, width, height, color, lifespan, angle)
{
  x = y = 0;

  initialize ();
}

void Virus::initialize ()
{
  Random generator = new Random ();

  t = lastPathX = lastPathY = 0.0;

  deltaT = randomizeDouble (defaultDeltaT);
  pathLength = randomizeDouble (defaultPathLength);

  trailDeltaT = defaultTrailDeltaT;
  trailMarkerT = 0;

  x1 = generator.nextDouble ();
  y1 = generator.nextDouble ()*2.0 - 1.0;

  x2 = generator.nextDouble ();
  y2 = generator.nextDouble ()*2.0 - 1.0;

  velocity = (maxSpeed - minSpeed)*Math.random () + minSpeed;

  pathAngle = generator.nextDouble () * 2 * Math.PI;

  velocityDirection = 0;

  drawTrail = false;
  ciricleVirus = false;

  trailColor = Color.red;
  outlineColor = Color.blue;
  selectedColor = Color.green;

  trail = new PathTrail (trailColor);
  //< These variables are currently unused
}

DBL Virus::velocityX ()
{
  return x - lastX;
}

DBL Virus::velocityY ()
{
  return y - lastY;
}

DBL Virus::centerX ()
{
  return x + Width () / 2;
}
DBL Virus::centerY ()
{
  return y + Height () / 2;
}

void Virus::collideWith (Virus thatVirus)
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

  DBL m1 = (DBL)getNumPixels (),
    m2 = (DBL)thatVirus.getNumPixels (),
    sumOfMasses = m1 + m2,
    m1_minus_m2 = m1 - m2,
    m2_minus_m1 = m2 - m1,
    ux1 = velocityX (),
    uy1 = velocityY (),
    ux2 = thatVirus.velocityX (),
    uy2 = thatVirus.velocityY (),
    vx1 = (ux1*m1_minus_m2 + 2 * m2*ux2) / sumOfMasses,
    vy1 = (uy1*m1_minus_m2 + 2 * m2*uy2) / sumOfMasses,
    vx2 = (ux2*m2_minus_m1 + 2 * m1*ux1) / sumOfMasses, //  |   /|
    vy2 = (uy2*m2_minus_m1 + 2 * m1*uy1) / sumOfMasses, //  |  / |
    newAngle1 = Math.atan (vy1 / vx1), // SOH CAH TOA     | /T | vy
    newAngle2 = Math.atan (vy2 / vx2); // O=vy A=vx    ___|/___|
                                // T=Theta                 vx
  pathAngle = newAngle1;
  thatVirus.pathAngle = newAngle2;
}

void Virus::updatePosition (SI4 hostWidth, SI4 hostHeight)
{
  // The movement of the virus is based off of Bezier curves. The curve has two given points, a start at 
  // coordinates (0,0), and an end at coordinates (1,0). Points two and three are bounded by 
  // { 0 <= x <= 1 } and { 1 <= y >= -1}.

  //Print ("t=" + t + "\n");
  t += deltaT;                // Increment the current time in the path.

  if (t < 0) // This shouldn't happen.
  {
    t = 0;
    lastPathX = lastPathY = 0;
  } else if (t > 1.0)
  {
    t = t - 1.0;
    lastPathX -= 1.0;
  }

  DBL oneMinus_t = 1.0 - t,
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

  x += deltaX * cosTheta - deltaY * sinTheta;
  y += deltaX * sinTheta + deltaY * cosTheta;

  //Print ("\nt = " + t + ", velocity = " + velocity + ", pathLength = " + pathLength + "\n");
  //Print ("newPathX = " + newPathX + ", newPathY = " + newPathY + "\n");
  //Print ("lastPathX = " + lastPathX + ", lastPathY = " + lastPathY + "\n");
  //Print ("deltaX = " + deltaX + ", deltaY = " + deltaY + "\n");
  //Print ("lastX = " + lastX + ", lastY = " + lastY + "\n");
  //Print ("x = " + x + ", y = " + y + "\n");

  checkHostBoundsCollision (hostWidth, hostHeight);

  if (drawTrail)
    updateTrail ();
}

void Virus::updateTrail ()
{
  if (t < trailMarkerT)
    return;
  // else
  trailMarkerT += trailDeltaT;

  if (trailMarkerT > 1)
    trailMarkerT -= 1;

  trail.addPoint ((SI4)centerX (), (SI4)centerY ());
}

bool Virus::checkHostBoundsCollision (SI4 hostWidth, SI4 hostHeight)
{
  DBL dx = x - lastX,
    dy = y - lastY;

  bool collisionDetected;

  if (x < 0 || (SI4)x + Width () > hostWidth)
  {
    dx *= -1;
    collisionDetected = true;
  } else if (y < 0 || (SI4)y + Height () > hostHeight)
  {
    dy *= -1;
    collisionDetected = true;
  } else
    collisionDetected = false;

  if (collisionDetected)
  {
    // Because we are using the Bezier Curve paths, if the Virus collides with the wall, the pathAngle
    // needs to be adjustest so that the two collision results in a smooth transition of angles.
    // We need to find out what is the initial angle for the first movement in the Bezier curve.

    DBL t_plus_deltaT = t + deltaT,
      oneMinus_t = 1.0 - t_plus_deltaT,
      oneMinus_t_Squared = Math.pow (oneMinus_t, 2.0),
      oneMinus_t_Cubed = Math.pow (oneMinus_t, 3.0),
      tSquared = Math.pow (t_plus_deltaT, 2.0),
      tCubed = Math.pow (t_plus_deltaT, 3.0),
      newPathX = 3.0*oneMinus_t_Squared*t_plus_deltaT*x1 + 3.0*oneMinus_t*tSquared*x2 + tCubed,
      newPathY = 3.0*oneMinus_t_Squared*t_plus_deltaT*y1 + 3.0*oneMinus_t*tSquared*y2,
      nextPathDX = newPathX - lastPathX,
      nextPathDY = newPathY - lastPathY;

    pathAngle = Math.atan (dy / dx) + Math.atan (nextPathDX / nextPathDY); // TOA -> y/x
  }

  return collisionDetected;
}

void Virus::select ()
{
  selected = true;
}

void Virus::deselect ()
{
  selected = false;
}

bool Virus::containsPoint (SI4 thisX, SI4 thisY)
{
  return containsPoint ((DBL)thisX, (DBL)thisY);
}

bool Virus::containsPoint (DBL thisX, DBL thisY)
{
  DBL polyX = thisX - x,
    polyY = thisY - y;

  if (thisX < x || thisX > rightEdge ()
    || thisY < y || thisY > bottomEdge ())
    return false;

  //Print ("\nx = " + x + ", y = " + y + "\n");
  //Print ("thisX = " + thisX + ", thisY = " + thisY + "\n");
  //Print ("polyX = " + polyX + ", polyY = " + polyY + "\n");
  //Print ("left = " + x + ", top = " + y + ", right = " + rightEdge () + ", bottom = " + bottomEdge () + "\n\n");

  return super.contains (polyX, polyY);
}

bool Virus::contains (Rectangle thisRectangle)
{
  if (thisRectangle.x + Width () < x || thisRectangle.x > x + Width ()
    || thisRectangle.y + Height () < y || thisRectangle.y > y + Height ())
    return false;
  return true;
}

bool Virus::intersects (Virus thatVirus)
{
  if (thatVirus == null
    || x > thatVirus.rightEdge ()
    || y > thatVirus.bottomEdge ()
    || rightEdge () < thatVirus.x
    || bottomEdge () < thatVirus.y)
    return false;

  // else the rectangles occlude eachother.

  return super.intersects (thatVirus, thatVirus.x - x, thatVirus.y - y);
}

DBL Virus::getX ()
{
  return x;
}
DBL Virus::getY ()
{
  return y;
}
SI4 Virus::Left ()
{
  return (SI4)x;
}
SI4 Virus::Top ()
{
  return (SI4)y;
}
SI4 Virus::rightEdge ()
{
  return (SI4)x + Width ();
}
SI4 Virus::bottomEdge ()
{
  return (SI4)y + Height ();
}

BezierCurve Virus::getBezierCurve ()
{
  return new BezierCurve (0, 0, x1, y1, x2, y2, 1, 0);
}

void Virus::fillCircle (Graphics g)
{
  g.setColor (selectedColor);
  g.fillOval (Left () - CircleMargin, Top () - CircleMargin,
    Width () + CircleMarginX2, Height () + CircleMarginX2);
}

void Virus::drawCircle (Graphics g)
{
  g.setColor (outlineColor);
  g.drawOval (Left () - CircleMargin, Top () - CircleMargin,
    Width () + CircleMargin, Height () + CircleMargin);
}

void Virus::drawCircle (Graphics g, Color thisColor)
{
  g.setColor (thisColor);
  g.drawOval (Left () - CircleMargin, Top () - CircleMargin,
    Width () + CircleMargin, Height () + CircleMargin);
}

void Virus::draw (Graphics g)
{
  if (drawTrail)
    trail.draw (g);

  if (selected)
    fillCircle (g);

  g.drawImage (getBitmap (), Left (), Top (), null);

  if (ciricleVirus)
    drawCircle (g);
}

void Virus::setTrailColor (Color newColor)
{
  trailColor = newColor;
}
void Virus::setDrawTrail (bool newState)
{
  drawTrail = newState;
}
bool Virus::getDrawTrail ()
{
  return drawTrail;
}

void Virus::setCircleColor (Color newColor)
{
  selectedColor = newColor;
}
void Virus::setDrawCircle (bool newState)
{
  ciricleVirus = newState;
}

void Virus::setCircleFillColor (Color newColor)
{
  outlineColor = newColor;
}
void Virus::setFillCircle (bool newState)
{
  selected = newState;
}

}
}
