/* The Leukocyte Wars
@file       ~/Host.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.Host class.
*/
package leukocytewars;

import java.awt.*;
import javax.swing.*;

public class Host extends JPanel
{
    public Host (int thisWidth, int thisHeight, int numberOfCells, int numberOfViruses)
    {
        setPreferredSize (new Dimension (thisWidth, thisHeight));
        setBorder(BorderFactory.createLineBorder(Color.BLACK));
        
        cells   = new Cell[numberOfCells];
        
        numCells = numberOfCells;
        
        for (int i=0; i < numberOfCells; i++)
            cells[i] = new Cell (this);
        
        viruses = new VirusPopulation (this);
        
        hostColor = DefaultHostColor;
        backgroundColor = DefaultBackgroundColor;
    }
    
    public int getNumCells ()
    {
        return numCells;
    }
    public VirusPopulation virusPopulation ()
    {
        return viruses;
    }
    
    public void update ()
    {
        viruses.update ();
        
        // Check to see if a Virus occupies a cell.
        
        for (int i=0; i < numCells; i++)
        {
            Cell currentCell = cells[i];
            
            if (viruses.contains (currentCell) >= 0)
            {
                //if (currentCell.getMass ())
                //    ;
            }
        }
    }
    
    public void paintComponent (Graphics g)
    {
        super.paintComponent(g);
            
        setBackground(backgroundColor);
        
        Dimension bounds = getPreferredSize ();
        
        g.setColor (hostColor);
        g.fillRect (0, 0, bounds.width, bounds.height);
        
        viruses.draw (g);
    }
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    private int numCells;               //< The number of Cell(s)
                
    private Cell[] cells;               //< The array of Cell(s)
    
    private VirusPopulation viruses;    //< The Virus population array.
    
    private Color hostColor,            //< The background color.
                  backgroundColor;      //< The off screen
    
    private static final Color DefaultHostColor  = Color.gray,          //< The default background color of the 
                               DefaultBackgroundColor = Color.black;    //< The default background color. 
}