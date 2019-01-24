/* The Leukocyte Wars
@file       ~/VirusPopulation.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the GeneticPolygonTestPanel class.
@desc       This file contains the leukocytewars.VirusPopulation class.
*/
package leukocytewars;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

class GeneticPolygonChildPanel extends JPanel
{
    public GeneticPolygonChildPanel (GeneticPolygon fitShape, GeneticPolygon initChild, String thisShapeLabel, int width, int height)
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
    
    public void paintComponent (Graphics g)
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
    
    public GeneticPolygon getChild ()
    {
        return child;
    }
    
    public void setChild (GeneticPolygon newChild)
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
    
    private GeneticPolygon child, cell;
    
    private JLabel infoLabel;
    
    private boolean timerOn;
}
