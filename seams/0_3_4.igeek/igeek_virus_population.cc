/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/VirusPopulation.cpp
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

#include <Kabuki_Theater/Cell_IGA/VirusPopulation.h>

namespace Kabuki_Theater { namespace Cell_IGA {

VirusPopulation::VirusPopulation (Host thisHost)
{
    host = thisHost;
    viruses = new Virus[DefaultPopulationSize];
    numViruses = DefaultPopulationSize;
    
    Dimension hostDimensions = host.getPreferredSize ();
    
    double x,
           y,
           hostWidth = hostDimensions.width,
           hostHeight = hostDimensions.height;
    
    for (int i=0; i < DefaultPopulationSize; i++)
    {
        x = Math.random () * (double)hostWidth;
        y = Math.random () * (double)hostHeight;
        
        viruses[i] = new Virus (x, y);
    }
}

// Adds the newVirus to the population.
void VirusPopulation::addVirus (Virus newVirus)
{
    int numViruses = viruses.length,
        i;
    
    Virus[] newArray = new Virus[numViruses+1];
    
    for (i=0; i < numViruses; i++)
        newArray[i] = viruses[i];
    
    newArray[i] = newVirus;
}

// Returns the virus at the specified index.
Virus VirusPopulation::getVirus (int index)
{
    if (index < 0 || index > numViruses)
        return null;
    
    return viruses[index];
}


// Selects a random virus from the population.
int VirusPopulation::selectRandomVirus ()
{
    if (numViruses == 0)
        return -1;
    
    int n = 0,
        maxTries = 2*numViruses,
        index;
    
    do
    {
        index = (int)(Math.random () * (numViruses-1));
        
        if (viruses[index] != null)
            return index;
        
        n++;
    } while (n < maxTries);
    
    // Generate list of non-null indexes.
    int populatedIndecies[] = new int[numViruses],
        i=0;
    
    for (index=0; index < numViruses; index++)
    {
        if (viruses[index] != null)
        {
            populatedIndecies[i] = index;
            i++;
        }
        
    }
    index = (int)(Math.random () * (numViruses-1));
    
    return index;
}

/* Function returns a value greater that -1 if this population contains thisGeneticPolygon.
@return Returns -1 if the population doesn't contain thisGeneticPolygon. Else it will return the index of the Virus. 
*/  
int VirusPopulation::contains (GeneticPolygon thisGeneticPolygon)
{
    for (int i=0; i < numViruses; i++)
    {
        Virus currentVirus = viruses[i];
        
        if (currentVirus != null && currentVirus.containsGeneticPolygon (thisGeneticPolygon))
        {
            // Then the virus has occupies the cell
            return i;
        }
            
    }
    return -1;
}

// Function that selects the virus 
int VirusPopulation::selectVirus (int mouseX, int mouseY)
{
    for (int i=0; i < numViruses; i++)
    {
        //System.out.print (i + ", "); // Debug info
        Virus current = viruses[i];
        if (current != null && current.containsPoint (mouseX, mouseY))
            return i;
    }
    return -1;
}

// Returns the number of viruses.
int VirusPopulation::getNumViruses ()
{
    return numViruses;
}

// Updates the virus population with the new .
void VirusPopulation::update ()
{
    Dimension hostSize = host.getPreferredSize ();
    
    int width = hostSize.width,
        height = hostSize.height;
        
    for (int i=0; i < numViruses; i++)
        viruses[i].updatePosition (width, height);
    
    for (int i=0; i < numViruses; i++)
    {
        int j;
        
        for (j=0; j < i; j++)
            viruses[i].collideWith (viruses[j]);
        
        for (j++; j < numViruses; j++)
            viruses[i].collideWith (viruses[j]);
    }
}

void VirusPopulation::draw (Graphics g)
{
    Dimension hostDimensions = host.getPreferredSize ();
    
    for (int i=0; i < numViruses; i++)
    {
        Virus virus = viruses[i];
        
        int currentX = (int) virus.getX (),
            currentY = (int) virus.getY ();
        
        if (   currentX + virus.getWidth  () >= 0
            && currentY + virus.getHeight () >= 0
            && currentX <= hostDimensions.width
            && currentY <= hostDimensions.height)
        {
            virus.draw (g);
        }
    }
    
    boxCollidingViruses (g);
    
    //for (int i=0; i < numViruses; i++)
    //{
    //    for (int j=0; j < numViruses-1 viruses[i]
    //}
}

void VirusPopulation::boxCollidingViruses (Graphics& g)
{
    for (int i=0; i < numViruses; i++)
    {
        Virus a = viruses[i];
        int j;
        
        for (j=0; j < i; j++)
        {   
            Virus b = viruses[j];
            
            if (a.intersects (b))
                boxTwoViruses (g, a, b);
        }
        for (j++; j < numViruses; j++)
        {
            Virus b = viruses[j];
            
            if (a.intersects (b))
                boxTwoViruses (g, a, b);
        }   
    }
}

void VirusPopulation::boxTwoViruses (Graphics& g, Virus& a, Virus& b)
{
    int left   = (int)((a.getX ()       < b.getX ()      ) ? a.getX ()       : b.getX       ()),
        top    = (int)((a.getY ()       < b.getY ()      ) ? a.getY ()       : b.getY       ()),
        right  = (int)((a.rightEdge  () > b.rightEdge  ()) ? a.rightEdge  () : b.rightEdge  ()),
        bottom = (int)((a.bottomEdge () > b.bottomEdge ()) ? a.bottomEdge () : b.bottomEdge ());
    
    g.setColor (new Color (1.0f, 0.0f, 0.0f, 0.5f));
    
    g.drawRect (left-2, top-2, right-left+2, bottom-top+2);
    
    a.drawCircle (g, Color.blue);
    b.drawCircle (g, Color.green);
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
    