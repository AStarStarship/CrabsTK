/** Cell Evolutionary Interface
    @version   0.x
    @file      /.../CellEvolutionaryInterface/Cell.hpp
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

class Cell: public GeneticPolygon
/* Class represents a cell in the simulation.
*/
{
    static const int DefaultWidth  = 50,
        DefaultHeight = 50,
        circleBuffer  = 20;
    
    int numPointsWeakness,
        shapeAngleWeakness,
        colorWeakness;                      /< The color this Cell is weak to.
                                            /*< Viruses that are of weak to a color will allow a cell to infect it that smaller. */
    
    double x,                               /< The current x position.
        y;                                  /< The current y position.
    
    Host host;                              /< A pointer to the host.
    Virus virus;                            /< The infecting virus.
    long timeOfInfection;                   /< The time that this cell got infected.
    
    Cell (Host cellHost);
    
    Cell (int xPosition, int yPosition, int thisWidth, int thisHeight, int thisNumPoints, int thisColorARGB, 
        int thisLifespan, double thisAngle);
    
    void highlight (Graphics g, Color circleColor);
    
    void update ();
    
    double getX ();
    
    double getY ();
    
    int leftEdge ();
    
    int topEdge ();
    
    int rightEdge ();
    
    void infectCell (Virus thisVirus);
};

}   //< CellEvolutionaryInterface
