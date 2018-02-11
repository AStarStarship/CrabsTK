/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/GeneticPolygonPanel.cpp
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

#include <Kabuki_Theater/Cell_IGA/GeneticPolygonPanel.h>

namespace Kabuki_Theater { namespace Cell_IGA {
    
GeneticPolygonPanel::GeneticPolygonPanel (GeneticPolygon thisShape, String thisShapeLabel, int width, int height)
{
    setPreferredSize (new Dimension (width, height));
    setBorder(BorderFactory.createLineBorder(Color.BLACK));
    setLayout(new BorderLayout());
    
    JLabel thisLabel = new JLabel (thisShapeLabel);
    add(thisLabel, BorderLayout.NORTH);
    
    gPoly = thisShape;
}

void GeneticPolygonPanel::paintComponent (Graphics g)
{
    super.paintComponent(g);
    
    int offsetX, offsetY;
    
    // We want to center the GeneticPolygon
    
    Dimension panelDimensions = getPreferredSize ();
    
    if (gPoly != null)
    {
        offsetX = (panelDimensions.width  - gPoly.getWidth  ()) /2;
        offsetY = (panelDimensions.height - gPoly.getHeight ()) /2;
        
        g.drawImage(gPoly.getBitmap (), offsetX, offsetY, null);
    }
}

GeneticPolygonPanel::GeneticPolygon getGeneticPolygon ()
{
    return gPoly;
}

void GeneticPolygonPanel::respawn (int numPoints, int width, int height, int color, int lifespan, double angle)
{
    gPoly = new GeneticPolygon (numPoints, width, height, color, lifespan, angle);
}

void GeneticPolygonPanel::setGeneticPolygon (GeneticPolygon newPoly)
{
    gPoly = newPoly;
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
