/* Cell Evolutionary Interface
    @version   0.x
    @file      /.../CellEvolutionaryInterface/PathTrail.hpp
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

namespace CellEvolutionaryInterface {
 
class PathTrail
/* A class that represents a trail of lines.
    This class is represented by a circular cue of 2 dimensionsal points.
 */
 {
  public:
  
    static const Color DefaultTrailColor = Color.red;        
    /< The default PathTrail color.
    
    static const SI4 DefaultMaxNumPoints = 300;
    /< In order to make a line, we need two points.
    
    PathTrail ();
    
    PathTrail (Color thisTrailColor);
    
    PathTrail (Color thisTrailColor, SI4 thisMaxNumPoints);
    
    void initialize ();
    
    void setTrailColor (Color newColor);
    
    Color getTrailColor ();
    
    void draw (Graphics g);
    
    void addPoint (SI4 x, SI4 y);
    
    String toString ();
    
  private:
    
    Color trailColor;           /< The Color of this PathTrail.
    
    SI4 numPoints,              /< The number of points.
        startPoint,             /< The current startPoint in the circular queue.
        stopPoint,              /< The current stopPoint in the circular queue.
        maxNumPoints;           /< The maximum number of points.
                
    SI4 xpoints[],              /< The x values for the points.
        ypoints[];              /< The y values for the points.
 };
 
}   //< CellEvolutionaryInterface
