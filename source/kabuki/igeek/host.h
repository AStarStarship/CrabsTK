/** Cell Evolutionary Interface
    @version   0.x
    @file      /.../CellEvolutionaryInterface/Host.hpp
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
    
class Host: public JPanel
{
  public:
    
    Host (int thisWidth, int thisHeight, int numberOfCells, int numberOfViruses);
    
    int getNumCells ();
    
    VirusPopulation virusPopulation ();
    
    void update ();
    
    void paintComponent (Graphics g);
    
  private:
    
    int numCells;               /< The number of Cell(s)
                
    Cell[] cells;               /< The array of Cell(s)
    
    VirusPopulation viruses;    /< The Virus population array.
    
    Color hostColor,            /< The background color.
        backgroundColor;        /< The off screen
    
    static const Color DefaultHostColor  = Color.gray,  /< The default background color of the 
        DefaultBackgroundColor = Color.black;           /< The default background color. 
}