/* The Leukocyte Wars
@file       ~/VirusPopulation.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.VirusPopulation class.
*/
package leukocytewars;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

public class VirusPopulation
{
    public static final int DefaultPopulationSize = 100;
    
    // Constuctor
    public VirusPopulation (Host thisHost)
    {
        host = thisHost;
        viruses = new Virus[DefaultPopulationSize];
        numViruses = DefaultPopulationSize;
        
        Dimension hostDimensions = host.getPreferredSize ();
        
        double x,
               y,
               hostWidth = hostDimensions.width,
               hostHeight = hostDimensions.height;
        
        for (int i=0; i < DefaultPopulationSize; i++)
        {
            x = Math.random () * (double)hostWidth;
            y = Math.random () * (double)hostHeight;
            
            viruses[i] = new Virus (x, y);
        }
    }
    
    // Adds the newVirus to the population.
    public void addVirus (Virus newVirus)
    {
        int numViruses = viruses.length,
            i;
        
        Virus[] newArray = new Virus[numViruses+1];
        
        for (i=0; i < numViruses; i++)
            newArray[i] = viruses[i];
        
        newArray[i] = newVirus;
    }
    
    // Returns the virus at the specified index.
    public Virus getVirus (int index)
    {
        if (index < 0 || index > numViruses)
            return null;
        
        return viruses[index];
    }
    
    
    // Selects a random virus from the population.
    public int selectRandomVirus ()
    {
        if (numViruses == 0)
            return -1;
        
        int n = 0,
            maxTries = 2*numViruses,
            index;
        
        do
        {
            index = (int)(Math.random () * (numViruses-1));
            
            if (viruses[index] != null)
                return index;
            
            n++;
        } while (n < maxTries);
        
        // Generate list of non-null indexes.
        int populatedIndecies[] = new int[numViruses],
            i=0;
        
        for (index=0; index < numViruses; index++)
        {
            if (viruses[index] != null)
            {
                populatedIndecies[i] = index;
                i++;
            }
            
        }
        index = (int)(Math.random () * (numViruses-1));
        
        return index;
    }
    
    /* Function returns a value greater that -1 if this population contains thisGeneticPolygon.
    @return Returns -1 if the population doesn't contain thisGeneticPolygon. Else it will return the index of the Virus. 
    */  
    public int contains (GeneticPolygon thisGeneticPolygon)
    {
        for (int i=0; i < numViruses; i++)
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
    public int selectVirus (int mouseX, int mouseY)
    {
        for (int i=0; i < numViruses; i++)
        {
            //System.out.print (i + ", "); // Debug info
            Virus current = viruses[i];
            if (current != null && current.containsPoint (mouseX, mouseY))
                return i;
        }
        return -1;
    }
    
    // Returns the number of viruses.
    public int getNumViruses ()
    {
        return numViruses;
    }
    
    // Updates the virus population with the new .
    public void update ()
    {
        Dimension hostSize = host.getPreferredSize ();
        
        int width = hostSize.width,
            height = hostSize.height;
            
        for (int i=0; i < numViruses; i++)
            viruses[i].updatePosition (width, height);
        
        for (int i=0; i < numViruses; i++)
        {
            int j;
            
            for (j=0; j < i; j++)
                viruses[i].collideWith (viruses[j]);
            
            for (j++; j < numViruses; j++)
                viruses[i].collideWith (viruses[j]);
        }
    }
    
    public void draw (Graphics g)
    {
        Dimension hostDimensions = host.getPreferredSize ();
        
        for (int i=0; i < numViruses; i++)
        {
            Virus virus = viruses[i];
            
            int currentX = (int) virus.getX (),
                currentY = (int) virus.getY ();
            
            if (   currentX + virus.getWidth  () >= 0
                && currentY + virus.getHeight () >= 0
                && currentX <= hostDimensions.width
                && currentY <= hostDimensions.height)
            {
                virus.draw (g);
            }
        }
        
        boxCollidingViruses (g);
        
        //for (int i=0; i < numViruses; i++)
        //{
        //    for (int j=0; j < numViruses-1 viruses[i]
        //}
    }
    
    public void boxCollidingViruses (Graphics g)
    {
        for (int i=0; i < numViruses; i++)
        {
            Virus a = viruses[i];
            int j;
            
            for (j=0; j < i; j++)
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
    
    public void boxTwoViruses (Graphics g, Virus a, Virus b)
    {
        int left   = (int)((a.getX ()       < b.getX ()      ) ? a.getX ()       : b.getX       ()),
            top    = (int)((a.getY ()       < b.getY ()      ) ? a.getY ()       : b.getY       ()),
            right  = (int)((a.rightEdge  () > b.rightEdge  ()) ? a.rightEdge  () : b.rightEdge  ()),
            bottom = (int)((a.bottomEdge () > b.bottomEdge ()) ? a.bottomEdge () : b.bottomEdge ());
        
        g.setColor (new Color (1.0f, 0.0f, 0.0f, 0.5f));
        
        g.drawRect (left-2, top-2, right-left+2, bottom-top+2);
        
        a.drawCircle (g, Color.blue);
        b.drawCircle (g, Color.green);
    }
    
    private int numViruses;             //< The number of Viruses.
    private Virus[] viruses;            //< The array of Virus.
    private Host host;                  //< A pointer to the Host this VirusPopulation lives on.
}