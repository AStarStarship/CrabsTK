/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/Host.cpp
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

#include <Kabuki_Theater/Cell_IGA/Host.h>

namespace Kabuki_Theater { namespace Cell_IGA {

Host::Host (int thisWidth, int thisHeight, int numberOfCells, int numberOfViruses)
{
    setPreferredSize (new Dimension (thisWidth, thisHeight));
    setBorder(BorderFactory.createLineBorder(Color.BLACK));
    
    cells   = new Cell[numberOfCells];
    
    numCells = numberOfCells;
    
    for (int i=0; i < numberOfCells; i++)
        cells[i] = new Cell (this);
    
    viruses = new VirusPopulation (this);
    
    hostColor = DefaultHostColor;
    backgroundColor = DefaultBackgroundColor;
}

int Host::getNumCells ()
{
    return numCells;
}
VirusPopulation Host::virusPopulation ()
{
    return viruses;
}

void Host::update ()
{
    viruses.update ();
    
    // Check to see if a Virus occupies a cell.
    
    for (int i=0; i < numCells; i++)
    {
        Cell currentCell = cells[i];
        
        if (viruses.contains (currentCell) >= 0)
        {
            //if (currentCell.getMass ())
            //    ;
        }
    }
}

void Host::paintComponent (Graphics g)
{
    super.paintComponent(g);
        
    setBackground(backgroundColor);
    
    Dimension bounds = getPreferredSize ();
    
    g.setColor (hostColor);
    g.fillRect (0, 0, bounds.width, bounds.height);
    
    viruses.draw (g);
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
    