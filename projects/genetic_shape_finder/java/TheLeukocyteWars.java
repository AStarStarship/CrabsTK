/* The Leukocyte Wars
@file       ~/TheLeukocyteWars.java
@date       June 2, 2014
@author     Cale Jamison McCollough
@copyright  Cale Jamison McCollough
@license    http://opensource.org/licenses/MIT
@brief      This file contains the leukocytewars.TheLeukocyteWars class.
This is the main code for the 
*/
package leukocytewars;

import javax.swing.*;

public class TheLeukocyteWars extends JApplet
{
    public TheLeukocyteWars ()
    {
        add (new LeukocyteWar ());
    }
    
    public static void main (String[] args)
    {
        JFrame window = new JFrame ("The Leukocyte Wars");
        window.setDefaultCloseOperation (JFrame.EXIT_ON_CLOSE);
        
        JScrollPane scrollableArea = new JScrollPane (new LeukocyteWar ());
        
        window.setContentPane (scrollableArea);
        
        window.pack ();
        window.setVisible (true);
    }
}
