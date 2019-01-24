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

#include "cviruspopulation.h"

namespace kabuki {
namespace igeek {

VirusPopulation::VirusPopulation (Host thisHost)
{
  host = thisHost;
  viruses = new Virus[DefaultPopulationSize];
  numViruses = DefaultPopulationSize;

  Dimension hostDimensions = host.getPreferredSize ();

  DBL x,
    y,
    hostWidth = hostDimensions.width,
    hostHeight = hostDimensions.height;

  for (SI4 i = 0; i < DefaultPopulationSize; i++)
  {
    x = Math.random () * (DBL)hostWidth;
    y = Math.random () * (DBL)hostHeight;

    viruses[i] = new Virus (x, y);
  }
}

// Adds the virus to the population.
void VirusPopulation::addVirus (Virus virus) {
  SI4 numViruses = viruses.length,
    i;

  Virus[] newArray = new Virus[numViruses + 1];

  for (i = 0; i < numViruses; i++)
    newArray[i] = viruses[i];

  newArray[i] = virus;
}

// Returns the virus at the specified index.
Virus VirusPopulation::getVirus (SI4 index) {
  if (index < 0 || index > numViruses)
    return null;

  return viruses[index];
}


// Selects a random virus from the population.
SI4 VirusPopulation::selectRandomVirus () {
  if (numViruses == 0)
    return -1;

  SI4 n = 0,
    maxTries = 2 * numViruses,
    index;

  do
  {
    index = (SI4)(Math.random () * (numViruses - 1));

    if (viruses[index] != null)
      return index;

    n++;
  } while (n < maxTries);

  // Generate list of non-null indexes.
  SI4 populatedIndecies[] = new SI4[numViruses],
    i = 0;

  for (index = 0; index < numViruses; index++)
  {
    if (viruses[index] != null)
    {
      populatedIndecies[i] = index;
      i++;
    }

  }
  index = (SI4)(Math.random () * (numViruses - 1));

  return index;
}

/* Function returns a value greater that -1 if this population contains thisGeneticPolygon.
@return Returns -1 if the population doesn't contain thisGeneticPolygon. Else it will return the index of the Virus.
*/
SI4 VirusPopulation::contains (GeneticPolygon thisGeneticPolygon)
{
  for (SI4 i = 0; i < numViruses; i++)
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
SI4 VirusPopulation::selectVirus (SI4 mouseX, SI4 mouseY)
{
  for (SI4 i = 0; i < numViruses; i++)
  {
    //Print (i + ", "); // Debug info
    Virus current = viruses[i];
    if (current != null && current.containsPoint (mouseX, mouseY))
      return i;
  }
  return -1;
}

// Returns the number of viruses.
SI4 VirusPopulation::getNumViruses ()
{
  return numViruses;
}

// Updates the virus population with the new .
void VirusPopulation::update ()
{
  Dimension hostSize = host.getPreferredSize ();

  SI4 width = hostSize.width,
    height = hostSize.height;

  for (SI4 i = 0; i < numViruses; i++)
    viruses[i].updatePosition (width, height);

  for (SI4 i = 0; i < numViruses; i++)
  {
    SI4 j;

    for (j = 0; j < i; j++)
      viruses[i].collideWith (viruses[j]);

    for (j++; j < numViruses; j++)
      viruses[i].collideWith (viruses[j]);
  }
}

void VirusPopulation::draw (Graphics g)
{
  Dimension hostDimensions = host.getPreferredSize ();

  for (SI4 i = 0; i < numViruses; i++)
  {
    Virus virus = viruses[i];

    SI4 currentX = (SI4)virus.getX (),
      currentY = (SI4)virus.getY ();

    if (currentX + virus.Width () >= 0
      && currentY + virus.Height () >= 0
      && currentX <= hostDimensions.width
      && currentY <= hostDimensions.height)
    {
      virus.draw (g);
    }
  }

  boxCollidingViruses (g);

  //for (SI4 i=0; i < numViruses; i++)
  //{
  //    for (SI4 j=0; j < numViruses-1 viruses[i]
  //}
}

void VirusPopulation::boxCollidingViruses (Graphics& g)
{
  for (SI4 i = 0; i < numViruses; i++)
  {
    Virus a = viruses[i];
    SI4 j;

    for (j = 0; j < i; j++)
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
  SI4 left = (SI4)((a.getX () < b.getX ()) ? a.getX () : b.getX ()),
    top = (SI4)((a.getY () < b.getY ()) ? a.getY () : b.getY ()),
    right = (SI4)((a.rightEdge () > b.rightEdge ()) ? a.rightEdge () : b.rightEdge ()),
    bottom = (SI4)((a.bottomEdge () > b.bottomEdge ()) ? a.bottomEdge () : b.bottomEdge ());

  g.setColor (new Color (1.0f, 0.0f, 0.0f, 0.5f));

  g.drawRect (left - 2, top - 2, right - left + 2, bottom - top + 2);

  a.drawCircle (g, Color.blue);
  b.drawCircle (g, Color.green);
}

}
}
    
