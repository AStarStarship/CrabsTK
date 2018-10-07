/** Kabuki Theater - Cell Interactive Genetic Algorihm
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/GeneticPolygonChildPanel.cpp
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

#include <Kabuki_Theater/Cell_IGA/GeneticPolygonChildPanel.h>

namespace Kabuki_Theater { namespace Cell_IGA {

GeneticPolygonChildPanel::GeneticPolygonChildPanel (GeneticPolygon fitShape, GeneticPolygon initChild, String thisShapeLabel, int width, int height)
{
    setPreferredSize (new Dimension (width, height));
    setBorder(BorderFactory.createLineBorder(Color.BLACK));
    setLayout(new BorderLayout());
    
    JLabel thisLabel = new JLabel (thisShapeLabel);
    infoLabel = new JLabel ("");
    
    add(thisLabel, BorderLayout.NORTH);
    add(infoLabel, BorderLayout.SOUTH);
    
    cell = fitShape;
    setChild (initChild);
}

void GeneticPolygonChildPanel::paintComponent (Graphics g)
{
    super.paintComponent(g);
    
    int offsetX, offsetY;
    
    // We want to center the GeneticPolygon
    
    Dimension panelDimensions = getPreferredSize ();

    // Draw the cell
    
    if (cell != null)
    {
        offsetX = (panelDimensions.width  - cell.getWidth  ()) / 2;
        offsetY = (panelDimensions.height - cell.getHeight ()) / 2;
        
        g.drawImage(cell.getBitmap (), offsetX, offsetY, null);
    }
    // Draw the child
    
    if (child != null)
    {
        offsetX = (panelDimensions.width  - child.getWidth  ()) / 2;
        offsetY = (panelDimensions.height - child.getHeight ()) / 2;
        
        g.drawImage(child.getBitmap (), offsetX, offsetY, null);
    }
}

GeneticPolygon GeneticPolygonChildPanel::getChild ()
{
    return child;
}

void GeneticPolygonChildPanel::setChild (GeneticPolygon newChild)
{
    if (newChild == null)
        return;
    child = newChild;
    
    // Update childLabel
    
    String infoString;
    
    if (cell.containsGeneticPolygon (newChild))
    {
        infoString = "Cell contains child\n"
                   + "Cell mass = " + cell.getNumPixels () + "\n"
                   + "Child mass = " + newChild.getNumPixels () + "\n";
                   //+ "Child is " + cell.getNumPixels ()/child.getNumPixels () + "% of the mass of the cell.\n";
    }
    else
    {
        infoString = "Cell does not contain child.";
    }
    
    infoLabel.setText (infoString);
}

}   //< namespace Kabuki_Theater { namespace Cell_IGA {
