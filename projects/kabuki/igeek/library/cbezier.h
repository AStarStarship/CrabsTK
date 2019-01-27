/* Cell Evolutionary Interface
    @version   0.x
    @file      /.../CellEvolutionaryInterface/BezierCurve.hpp
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

#pragma once

namespace CellEvolutionaryInterface {
    
/* A fourth order BezierCurve.
*/
class BezierCurve
{
  public:
    
    static const SI4 DefaultNumPoints = 4;
    
    SI4 numPoints;
               
    DBL* xpoints,    //< An array of the 
          * ypoints;
    
    /* Constructor initializes curve with the given points.
    @warning the length of xPoints and yPoints must equal nPoints or curve will not be created correctly.
    */ 
    BezierCurve (DBL xPoints[], DBL yPoints[], SI4 nPoints);

    /* Constructor initializes curve with the given points.
    */
    BezierCurve (DBL x0, DBL y0, DBL x1, DBL y1, DBL x2, DBL y2, DBL x3, DBL y3)
    {
        numPoints = 4;
        xpoints = new DBL[4];
        ypoints = new DBL[4];
        
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
    BezierCurve (BezierCurve initState);
    
    / Returns a Point2D along the path of the of curve at time t.
    Point2D getPoint (DBL t);
    
    static SI4 factoral (SI4 value);
    
    SI4 getNumPoints ();
    
    String toString ();
};

}   //< CellEvolutionaryInterface

