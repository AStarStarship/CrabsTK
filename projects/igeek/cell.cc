/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/Cell.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                      All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

            http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <Kabuki_Theater/Cell_IGA/Cell.h>

namespace Kabuki_Theater { namespace Cell_IGA {
    
Cell::Cell (Host cellHost)
{
    super ();
    
    host = cellHost;
}

Cell::Cell (int xPosition, int yPosition, int thisWidth, int thisHeight, int thisNumPoints, 
    int thisColorARGB, int thisLifespan, double thisAngle)
{
    super (thisNumPoints, thisWidth, thisHeight, thisColorARGB, thisLifespan, thisAngle);
    
}

void Cell::highlight (Graphics g, Color circleColor)
{
    g.setColor (circleColor);
    g.drawOval (leftEdge () - circleBuffer, topEdge () - circleBuffer,getWidth () + circleBuffer, getHeight () + circleBuffer);
}

void Cell::update ()
{
    // We only need to do any logic if the cell is infected. 
    
    if (virus == null)
        return;
}

double Cell::getX ()
{
    return x;
}

double Cell::getY ()
{
    return y;
}
int Cell::leftEdge ()
{
    return (int) x;
}

int Cell::topEdge ()
{
    return (int) y;
}

int Cell::rightEdge ()
{
    return (int)x+getWidth ();
}

int Cell::bottomEdge ()
{
    return (int)y+getHeight ();
}

void Cell::infectCell (Virus thisVirus)
{
    virus = thisVirus;
    timeOfInfection = System.currentTimeMillis ();
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
