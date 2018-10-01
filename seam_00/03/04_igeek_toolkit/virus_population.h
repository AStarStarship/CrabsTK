/** Cell Evolutionary Interface
    @version   0.x
    @file      /.../CellEvolutionaryInterface/VirusPopulation.hpp
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

#include "Virus.h"
#include "Host.h"

namespace CellEvolutionaryInterface {

class VirusPopulation
{
    static const int DefaultPopulationSize = 100;
    
    VirusPopulation (Host thisHost);
    /*< Constuctor */
    
    void addVirus (Virus newVirus);
    /*< Adds the newVirus to the population. */
    
    Virus getVirus (int index);
    /*< Returns the virus at the specified index. */
    
    int selectRandomVirus ();
    /*< Selects a random virus from the population. */
    
    int contains (GeneticPolygon thisGeneticPolygon);
    /*< Function returns a value greater that -1 if this population contains thisGeneticPolygon.
        @return Returns -1 if the population doesn't contain thisGeneticPolygon. Else it will return the index of the Virus. 
    */  
    
    int selectVirus (int mouseX, int mouseY);
    /*< Function that selects the virus  */
    
    int getNumViruses ();
    /*< Returns the number of viruses. */
    
    void update ();
    /*< Updates the virus population with the new . */
    
    void draw (Graphics g);
    
    void boxCollidingViruses (Graphics g);
    
    void boxTwoViruses (Graphics g, Virus a, Virus b);
    
  private:
    
    int numViruses;             /< The number of Viruses.
    Virus[] viruses;            /< The array of Virus.
    Host host;                  /< A pointer to the Host this VirusPopulation lives on.
}