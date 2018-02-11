/** Cell Interactive Evolutionary Computation
    @version   1.x
    @file      /.../Cell_IEC/GeneticPolygonPanel.h
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

namespace Cell_IEC {

class GeneticPolygonPanel: public JPanel
{
    static const int defaultMargin = 30;
    
    GeneticPolygonPanel (GeneticPolygon thisShape, String thisShapeLabel, int width, int height);
    
    void paintComponent (Graphics g);
    
    GeneticPolygon getGeneticPolygon ();
    
    void respawn (int numPoints, int width, int height, int color, int lifespan, double angle);
    
    void setGeneticPolygon (GeneticPolygon newPoly);
    
  private:
    
    GeneticPolygon gPoly;
};

}   /< namespace Cell_IEC {
