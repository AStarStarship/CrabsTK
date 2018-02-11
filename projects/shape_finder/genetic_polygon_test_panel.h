/** Cell Interactive Evolutionary Computation
    @version   1.x
    @file      /.../Cell_IEC/GeneticPolygonTestPanel.h
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

class GeneticPolygonTestPanel: public JPanel
{
  private:
    
    enum {
        panelWidth          = 1024,
        panelHeight         = 700,
        panelMargin         = 10,
        parentPanelSize     = 300,
        childPanelSize      = 400,
        initPopulationSize  = 100,
    };
    
    GeneticPolygonTestPanel ();
    
    void initializePopulation ();
    
    void paintComponent (Graphics g);
    
    void iterateGeneration ();
    
    void mateRandom ();
    
    void addChild (GeneticPolygon newChild);
    
  private:
    
    GeneticPolygonPanel motherPanel, fatherPanel, matGrandPanel, patGrandPanel;
    GeneticPolygonChildPanel childPanel;
    
    GeneticPolygon cell;
    
    GeneticPolygon population[];            /< The population of GeneticPolygon(s)
    
    int timerUpdateInterval  = 1000,        /< The durration of time tha the timer is updated
                populationSize;             /< The size of the pupulation.
    Timer timer;                            /< The timer for the 
    bool timerOn;                           /< Represents if the timer is on or off.
    
    class StartAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
    
    class StopAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
    
    class NextAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
    
    class MateAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
    
    class TimerAction: public ActionListener
    {
        void actionPerformed (ActionEvent e);
    }
};

}   /< namespace Cell_IEC {
