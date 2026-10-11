/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/library/igeek_geneticpolygonchildpanel.cc
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

#include "cgeneticpolygonchildpanel.h"

namespace kabuki {
namespace igeek {

GeneticPolygonChildPanel::GeneticPolygonChildPanel (GeneticPolygon fitShape, GeneticPolygon initChild, String thisShapeLabel, SI4 width, SI4 height)
{
  setPreferredSize (new Dimension (width, height));
  setBorder (BorderFactory.createLineBorder (Color.BLACK));
  setLayout (new BorderLayout ());

  JLabel thisLabel = new JLabel (thisShapeLabel);
  infoLabel = new JLabel ("");

  add (thisLabel, BorderLayout.NORTH);
  add (infoLabel, BorderLayout.SOUTH);

  cell = fitShape;
  setChild (initChild);
}

void GeneticPolygonChildPanel::paintComponent (Graphics g)
{
  super.paintComponent (g);

  SI4 offsetX, offsetY;

  // We want to center the GeneticPolygon

  Dimension panelDimensions = getPreferredSize ();

  // Draw the cell

  if (cell != null)
  {
    offsetX = (panelDimensions.width - cell.Width ()) / 2;
    offsetY = (panelDimensions.height - cell.Height ()) / 2;

    g.drawImage (cell.getBitmap (), offsetX, offsetY, null);
  }
  // Draw the child

  if (child != null)
  {
    offsetX = (panelDimensions.width - child.Width ()) / 2;
    offsetY = (panelDimensions.height - child.Height ()) / 2;

    g.drawImage (child.getBitmap (), offsetX, offsetY, null);
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
  } else
  {
    infoString = "Cell does not contain child.";
  }

  infoLabel.setText (infoString);
}

}
}
