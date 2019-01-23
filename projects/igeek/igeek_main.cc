/** Cell Evolutionary Interface
    @version   0.x
    @file      /.../Source/Kabuki_Theater/Cell_IGA-Impl/CellEvolutionaryInterface.cpp
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

#include <Kabuki_Theater/Cell_IGA/CellEvolutionaryInterface.hpp>

namespace project { namespace igeek {

CellEvolutionaryInterface::CellEvolutionaryInterface ()
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

void CellEvolutionaryInterface::initialize ()
{
    
    host = new Host (defaultWidth, defaultHeight, defaultNumCells, defaultNumViruses);
    viruses = host.virusPopulation ();
    
    timerOn = true;
    timer.start();
}

void CellEvolutionaryInterface::paintComponent (Graphics g)
{
    super.paintComponent (g);
    
    host.paintComponent (g);
    
    if (curvePanel != null && drawCurrentBezierCurve)
        curvePanel.draw (g);
}

class Toolbar: public JPanel
{
    Toolbar ()
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

class TimerAction: public ActionListener
{
    void actionPerformed (ActionEvent e)
    {
        if (timerOn)
        {
            host.update ();
        }
        repaint ();
    }
}

class StartAction: public ActionListener
{
    void actionPerformed (ActionEvent e)
    {
        timerOn = true;
    }
}

class StopAction: public ActionListener
{
    void actionPerformed (ActionEvent e)
    {
        timerOn = false;
    }
}

void CellEvolutionaryInterface::mousePressed(MouseEvent e)
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

void CellEvolutionaryInterface::mouseReleased(MouseEvent e)
{
    ;
}
 
void CellEvolutionaryInterface::mouseEntered(MouseEvent e)
{
    ;
}
 
void CellEvolutionaryInterface::mouseExited(MouseEvent e)
{
    ;
}
 
void CellEvolutionaryInterface::mouseClicked(MouseEvent e)
{
    ;
}

void CellEvolutionaryInterface::keyTyped (KeyEvent e)
{
    //Print ("keyTyped = " + e.getKeyChar() + "\n");
}

void CellEvolutionaryInterface::keyPressed (KeyEvent e)
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

void CellEvolutionaryInterface::keyReleased (KeyEvent e)
{
    char key = e.getKeyChar();
    
    if (key == 'k'  || key == 'K')
    {
        kill = false;
    }
}

}
