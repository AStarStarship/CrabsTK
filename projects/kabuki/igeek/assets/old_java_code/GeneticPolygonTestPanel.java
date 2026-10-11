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

public class GeneticPolygonTestPanel extends JPanel
{
    public static final int panelWidth  = 1024,
                            panelHeight = 700,
                            panelMargin = 10,
                            parentPanelSize = 300,
                            childPanelSize = 400,
                            initPopulationSize = 100;
    
    public GeneticPolygonTestPanel ()
    {
        setPreferredSize (new Dimension (panelWidth, panelHeight));
        
        timer = new Timer (timerUpdateInterval, new TimerAction ());
        timerOn = false;
        
        JButton startButton = new JButton ("Start");
        JButton stopButton  = new JButton ("Stop");
        JButton mateButton  = new JButton ("Mate");
        JButton nextButton  = new JButton ("Next");
        
        startButton.addActionListener (new StartAction ());
        stopButton.addActionListener (new StopAction ());
        mateButton.addActionListener (new MateAction ());
        nextButton.addActionListener (new NextAction ());
        
        JPanel buttonPanel = new JPanel ();
        buttonPanel.add (startButton);
        buttonPanel.add (stopButton);
        buttonPanel.add (mateButton);
        buttonPanel.add (nextButton);
        
        initializePopulation ();
        cell  = new GeneticPolygon (5, 
                                    (int)((double)GeneticPolygon.defaultSize*1.3),
                                    (int)((double)GeneticPolygon.defaultSize*1.3), 
                                    Color.blue.getRGB (),
                                    GeneticPolygon.defaultLifespan,
                                    GeneticPolygon.defaultAngle);
        
        GeneticPolygon adam = population[0],
                       eve  = population[1],
                       child = new GeneticPolygon(adam, eve);
        
        motherPanel   = new GeneticPolygonPanel (adam, "Father", parentPanelSize, parentPanelSize);
        matGrandPanel = new GeneticPolygonPanel (null, "Maternal Grandparents", parentPanelSize, parentPanelSize);
        
        fatherPanel   = new GeneticPolygonPanel (eve, "Mother", parentPanelSize, parentPanelSize);
        patGrandPanel = new GeneticPolygonPanel (null, "Paternal Grandparents", parentPanelSize, parentPanelSize);
        
        childPanel  = new GeneticPolygonChildPanel (cell, child, "Child", childPanelSize, 2*parentPanelSize+panelMargin);
            
        JPanel maternalPanel = new JPanel (new BorderLayout ()),
               paternalPanel = new JPanel (new BorderLayout ());
        
        maternalPanel.setPreferredSize (new Dimension (parentPanelSize, 2*parentPanelSize+panelMargin));
        paternalPanel.setPreferredSize (new Dimension (parentPanelSize, 2*parentPanelSize+panelMargin));
        
        maternalPanel.add (motherPanel, BorderLayout.NORTH);
        maternalPanel.add (matGrandPanel, BorderLayout.SOUTH);
        
        paternalPanel.add (fatherPanel, BorderLayout.NORTH);
        paternalPanel.add (patGrandPanel, BorderLayout.SOUTH);
        
        JPanel polygonsPanel = new JPanel ();
        polygonsPanel.add (maternalPanel);
        polygonsPanel.add (childPanel);
        polygonsPanel.add (paternalPanel);
        
        setLayout (new BorderLayout ());
        add (polygonsPanel , BorderLayout.CENTER);
        add (buttonPanel   , BorderLayout.SOUTH);
    }
    
    public void initializePopulation ()
    {
        populationSize = initPopulationSize;
        
        population = new GeneticPolygon[initPopulationSize];
        
        for (int i=0; i < initPopulationSize; i++)
        {
            int numPoints = (int)(Math.floor(Math.random () * 5.0 + 3.0));
            population[i] = new GeneticPolygon (numPoints, GeneticPolygon.getDefaultColor(numPoints).getRGB ());
        }
    }
    
    public void paintComponent (Graphics g)
    {
        super.paintComponent (g);
        
        motherPanel.paintComponent (g);
        fatherPanel.paintComponent (g);
        childPanel.paintComponent (g);
    }
    
    private void iterateGeneration ()
    {
        int numberToMate = (int)(GeneticPolygon.matingPercent * (double)populationSize);
        
        for (int i=0; i < numberToMate; i++)
            mateRandom ();
        
        for (int i=0; i < populationSize; i++)
        {
            if (population[i] != null)
            {
                if (!population[i].age (10000)) // The defaultLifespan is x number of seconds.
                    population[i] = null;
            }
        }
        
        repaint ();      // Repaint indirectly calls paintComponent.
    }
    
    private void mateRandom ()
    {
    
        int randomIndex,
            numTries = 0,
            maxRandomTries = 2*populationSize;
        
        do
        {
            randomIndex = (int)(Math.random () * (double)(populationSize-1));
            numTries++;
            
            if (numTries == maxRandomTries)
            {
                for (randomIndex=0; randomIndex < populationSize; ++randomIndex)
                    if (population[randomIndex] != null)
                        break;
                
                if (randomIndex == populationSize)
                {
                    timerOn = false;
                    System.out.print ("Error! The entire population is dead!!!\n");
                    return;
                }
                else
                    break;
            }
        }
        while (population[randomIndex] == null);
        
        GeneticPolygon newFather = population[randomIndex],
                       newMother = childPanel.getChild (),
                       newChild  = new GeneticPolygon (newMother, newFather);
        
        fatherPanel.setGeneticPolygon (newFather);
        motherPanel.setGeneticPolygon (newMother);
        childPanel.setChild (newChild);
        
        addChild (newChild);
    }
    
    private void addChild (GeneticPolygon newChild)
    {
        int i;
        for (i=0; i < populationSize; i++)
        {
            if (population[i] == null)
            {
                population[i] = newChild;
                return;
            }
        }
        // We couldn't find a hole so increase the array size;
        GeneticPolygon[] newPopulation = new GeneticPolygon[populationSize+1];
        
        for(i=0; i < populationSize; i++)
            newPopulation[i] = population[i];
        newPopulation[i] = newChild;
        population = newPopulation;
        populationSize++;
    }
    
    private GeneticPolygonPanel motherPanel, fatherPanel, matGrandPanel, patGrandPanel;
    private GeneticPolygonChildPanel childPanel;
    
    private GeneticPolygon cell;
    
    private GeneticPolygon population[];            //< The population of GeneticPolygon(s)
    
    private int timerUpdateInterval  = 1000,        //< The durration of time tha the timer is updated
                populationSize;                     //< The size of the pupulation.
    private Timer timer;                            //< The timer for the 
    private boolean timerOn;                        //< Represents if the timer is on or off.
    
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
    
    private class NextAction implements ActionListener
    {
        public void actionPerformed (ActionEvent e)
        {
            iterateGeneration ();
        }
    }
    
    private class MateAction implements ActionListener
    {
        public void actionPerformed (ActionEvent e)
        {
            mateRandom ();
        
            repaint ();      // Repaint indirectly calls paintComponent.
        }
    }
    
    private class TimerAction implements ActionListener
    {
        public void actionPerformed (ActionEvent e)
        {
            if (timerOn)
            {
                iterateGeneration ();
            }
        }
    }
}