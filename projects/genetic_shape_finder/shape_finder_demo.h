/** Cell Interactive Evolutionary Computation
    @version   1.x
    @file      /.../Cell_IEC/LeukocyteWar.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 Cale McCollough <calemccollough.github.io>

                      All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

            http:/www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

namespace Cell_IEC {


class LeukocyteWar: public JPanel, public MouseListener, KeyListener /, ActionListener 
{
  public:
    
    enum {
        defaultWidth        = 1300, /< The default window width.
        defaultHeight       = 700,  /< The default window height.
        defaultNumCells     = 0,    /< The default number of Viruses for the simulation
        defaultNumViruses   = 20,   /< The default number of Viruses for the simulation
        timerUpdateInterval = 100,  /< The durration of time tha the timer is updated.
    };
    
    LeukocyteWar ();
    
    void initialize ();
    
    void paintComponent (Graphics g);
    
  private:
    
    Host host;                      /< The simulated host.
    VirusPopulation viruses;        /< The Virus population array.
    
    Timer timer;                    /< The timer for the program.
    bool timerOn,                   /< The time on status.
        drawCurrentBezierCurve,     /< The the Bezier curve for the current selected virus.
        drawCurrentTrail,           /< Draws the current selectred virus's trail.
        kill,                       /< Flag that determines if program is in "Kill Mode"
        breed;
    
    Virus currentVirus;             /< The currently selected virus.
    
    BezierCurvePanel curvePanel;    /< The graphic representation of the currentVirus's Bezier movement curve.
    
    int curvePanelX,
        curvePanelY,
        currentVirusIndex;          /< The index of the currently selected virus.
    
    Toolbar toolbar;
    
    class Toolbar: public JPanel
    {
        Toolbar ();
    }
    
    class TimerAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
    
    class StartAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
    
    class StopAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }

    void mousePressed(MouseEvent e);
    
    void mouseReleased(MouseEvent e);
     
    void mouseEntered(MouseEvent e);
     
    void mouseExited(MouseEvent e);
     
    void mouseClicked(MouseEvent e);
    
    void keyTyped (KeyEvent e);

    void keyPressed (KeyEvent e);

    void keyReleased (KeyEvent e);
}
