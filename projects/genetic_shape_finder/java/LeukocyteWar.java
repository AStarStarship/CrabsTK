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


public class LeukocyteWar extends JPanel implements MouseListener, KeyListener //, ActionListener 
{
    public static final int defaultWidth        = 1300,         //< The default window width.
                            defaultHeight       = 700,          //< The default window height.
                            defaultNumCells     = 0,            //< The default number of Viruses for the simulation
                            defaultNumViruses   = 20,           //< The default number of Viruses for the simulation
                            timerUpdateInterval = 100;          //< The durration of time tha the timer is updated.
    
    public LeukocyteWar ()
    {
        setPreferredSize (new Dimension (defaultWidth, defaultHeight));
        setLayout (new BorderLayout ());
        addMouseListener(this);           
        addKeyListener (this);
        setFocusable(true);
        
        timer = new Timer (timerUpdateInterval, new TimerAction ());
        
        toolbar = new Toolbar ();
        
        add (toolbar, BorderLayout.SOUTH);
        
        initialize ();
        
        currentVirus = viruses.getVirus (viruses.selectRandomVirus ());
        currentVirus.setDrawTrail (true);
        currentVirus.setFillCircle (true);
        
        curvePanelX = 1000;
        curvePanelY = 30;
        
        drawCurrentBezierCurve = false;
        drawCurrentTrail       = false;
        
        curvePanel = new BezierCurvePanel (curvePanelX, curvePanelY, currentVirus.getBezierCurve (),
                                           Color.white, Color.blue, Color.red);
    }
    
    public void initialize ()
    {
        
        host = new Host (defaultWidth, defaultHeight, defaultNumCells, defaultNumViruses);
        viruses = host.virusPopulation ();
        
        timerOn = true;
        timer.start();
    }
    
    public void paintComponent (Graphics g)
    {
        super.paintComponent (g);
        
        host.paintComponent (g);
        
        if (curvePanel != null && drawCurrentBezierCurve)
            curvePanel.draw (g);
    }
    
    private Host host;                              //< The simulated host.
    private VirusPopulation viruses;                //< The Virus population array.
    
    private Timer timer;                            //< The timer for the program.
    private boolean timerOn,                        //< The time on status.
                    drawCurrentBezierCurve,         //< The the Bezier curve for the current selected virus.
                    drawCurrentTrail,               //< Draws the current selectred virus's trail.
                    kill,                           //< Flag that determines if program is in "Kill Mode"
                    breed;
    
    private Virus currentVirus;                     //< The currently selected virus.
    
    private BezierCurvePanel curvePanel;            //< The graphic representation of the currentVirus's Bezier movement curve.
    private int curvePanelX,
                curvePanelY,
                currentVirusIndex;                  //< The index of the currently selected virus.
    
    private Toolbar toolbar;
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    private class Toolbar extends JPanel
    {
        public Toolbar ()
        {
            JButton startButton = new JButton ("Start");
            JButton stopButton  = new JButton ("Stop");
            
            startButton.addActionListener(new StartAction ());
            stopButton.addActionListener(new StopAction ());
            
            //JButton nextButton = new JButton ("Next");
            
            //nextButton.setMnemonic(KeyEvent.VK_N);
            //nextButton.addActionListener(new NextAction ());
            //nextButton.setPreferredSize (new Dimension(150, 40));
            //nextButton.setAlignmentX (Component.CENTER_ALIGNMENT);
            //nextButton.setToolTipText("Click to iterate to the next generation.");
            
            add (startButton);
            add (stopButton);
        }
    }
    
    private class TimerAction implements ActionListener
    {
        public void actionPerformed (ActionEvent e)
        {
            if (timerOn)
            {
                host.update ();
            }
            repaint ();
        }
    }
    
    private class StartAction implements ActionListener
    {
        public void actionPerformed (ActionEvent e)
        {
            timerOn = true;
        }
    }
    
    private class StopAction implements ActionListener
    {
        public void actionPerformed (ActionEvent e)
        {
            timerOn = false;
        }
    }

    public void mousePressed(MouseEvent e)
    {
        currentVirusIndex = viruses.selectVirus (e.getX (), e.getY ());
        
        if (currentVirusIndex < 0) // Then there was not virus under the mouse cursor.
            return;
        
        Virus newSelectedVirus = viruses.getVirus (currentVirusIndex);
        
        if (newSelectedVirus == null)
            return;
        
        if (currentVirus != null)
            currentVirus.deselect ();
        
        currentVirus = newSelectedVirus;
        currentVirus.select ();
        
        curvePanel = new BezierCurvePanel (curvePanelX, curvePanelY, currentVirus.getBezierCurve (),
                                           Color.white, Color.blue, Color.red);
    }
    
    public void mouseReleased(MouseEvent e)
    {
        ;
    }
     
    public void mouseEntered(MouseEvent e)
    {
        ;
    }
     
    public void mouseExited(MouseEvent e)
    {
        ;
    }
     
    public void mouseClicked(MouseEvent e)
    {
        ;
    }
    
    public void keyTyped (KeyEvent e)
    {
        //System.out.print ("keyTyped = " + e.getKeyChar() + "\n");
    }

    public void keyPressed (KeyEvent e)
    {
        char key = e.getKeyChar();
        
        if (key == 'c'  || key == 'C')
        {
            if (drawCurrentBezierCurve)
                drawCurrentBezierCurve = false;
            else
                drawCurrentBezierCurve = true;
        }
        
        if (key == 't'  || key == 'T' || currentVirus == null)
        {
            if (drawCurrentTrail)
            {
                drawCurrentTrail = false;
                currentVirus.setDrawTrail (false);
            }
            else
            {
                drawCurrentTrail = true;
                currentVirus.setDrawTrail (true);
            }
        }
        
        if (key == 'k'  || key == 'K')
        {
            kill = true;
        }
    }

    public void keyReleased (KeyEvent e)
    {
        char key = e.getKeyChar();
        
        if (key == 'k'  || key == 'K')
        {
            kill = false;
        }
    }
}
