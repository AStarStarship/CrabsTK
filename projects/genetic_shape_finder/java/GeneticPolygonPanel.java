/* The Leukocyte Wars
@file       ~/GeneticPolygonPanel.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the GeneticPolygonTestPanel class.
@desc       This file contains the leukocytewars.GeneticPolygonPanel class.
This class is basically just a container for a GeneticPolygon so that it can be drawn on the screen.
*/
package leukocytewars;

import java.awt.*;
import javax.swing.*;

public class GeneticPolygonPanel extends JPanel
{
    public static final int defaultMargin = 30;
    
    public GeneticPolygonPanel (GeneticPolygon thisShape, String thisShapeLabel, int width, int height)
    {
        setPreferredSize (new Dimension (width, height));
        setBorder(BorderFactory.createLineBorder(Color.BLACK));
        setLayout(new BorderLayout());
        
        JLabel thisLabel = new JLabel (thisShapeLabel);
        add(thisLabel, BorderLayout.NORTH);
        
        gPoly = thisShape;
    }
    
    public void paintComponent (Graphics g)
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
    
    public GeneticPolygon getGeneticPolygon ()
    {
        return gPoly;
    }
    
    public void respawn (int numPoints, int width, int height, int color, int lifespan, double angle)
    {
        gPoly = new GeneticPolygon (numPoints, width, height, color, lifespan, angle);
    }
    
    public void setGeneticPolygon (GeneticPolygon newPoly)
    {
        gPoly = newPoly;
    }
    
    private GeneticPolygon gPoly;
}
