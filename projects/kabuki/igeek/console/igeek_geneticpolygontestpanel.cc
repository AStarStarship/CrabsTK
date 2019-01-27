/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /projects/kabuki/igeek/console/igeek_geneticpolygontestpanel.cc
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#include <pch.h>

namespace kabuki {
namespace igeek {

GeneticPolygonTestPanel::GeneticPolygonTestPanel ()
{
  setPreferredSize (new Dimension (panelWidth, panelHeight));

  timer = new Timer (timerUpdateInterval, new TimerAction ());
  timerOn = false;

  JButton startButton = new JButton ("Start");
  JButton stopButton = new JButton ("Stop");
  JButton mateButton = new JButton ("Mate");
  JButton nextButton = new JButton ("Next");

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
  cell = new GeneticPolygon (5,
    (SI4)((double)GeneticPolygon.defaultSize*1.3),
    (SI4)((double)GeneticPolygon.defaultSize*1.3),
    Color.blue.getRGB (),
    GeneticPolygon.defaultLifespan,
    GeneticPolygon.defaultAngle);

  GeneticPolygon adam = population[0],
    eve = population[1],
    child = new GeneticPolygon (adam, eve);

  motherPanel = new GeneticPolygonPanel (adam, "Father", parentPanelSize, parentPanelSize);
  matGrandPanel = new GeneticPolygonPanel (null, "Maternal Grandparents", parentPanelSize, parentPanelSize);

  fatherPanel = new GeneticPolygonPanel (eve, "Mother", parentPanelSize, parentPanelSize);
  patGrandPanel = new GeneticPolygonPanel (null, "Paternal Grandparents", parentPanelSize, parentPanelSize);

  childPanel = new GeneticPolygonChildPanel (cell, child, "Child", childPanelSize, 2 * parentPanelSize + panelMargin);

  JPanel maternalPanel = new JPanel (new BorderLayout ()),
    paternalPanel = new JPanel (new BorderLayout ());

  maternalPanel.setPreferredSize (new Dimension (parentPanelSize, 2 * parentPanelSize + panelMargin));
  paternalPanel.setPreferredSize (new Dimension (parentPanelSize, 2 * parentPanelSize + panelMargin));

  maternalPanel.add (motherPanel, BorderLayout.NORTH);
  maternalPanel.add (matGrandPanel, BorderLayout.SOUTH);

  paternalPanel.add (fatherPanel, BorderLayout.NORTH);
  paternalPanel.add (patGrandPanel, BorderLayout.SOUTH);

  JPanel polygonsPanel = new JPanel ();
  polygonsPanel.add (maternalPanel);
  polygonsPanel.add (childPanel);
  polygonsPanel.add (paternalPanel);

  setLayout (new BorderLayout ());
  add (polygonsPanel, BorderLayout.CENTER);
  add (buttonPanel, BorderLayout.SOUTH);
}

void GeneticPolygonTestPanel::initializePopulation ()
{
  populationSize = initPopulationSize;

  population = new GeneticPolygon[initPopulationSize];

  for (SI4 i = 0; i < initPopulationSize; i++)
  {
    SI4 numPoints = (SI4)(Math.floor (Math.random () * 5.0 + 3.0));
    population[i] = new GeneticPolygon (numPoints, GeneticPolygon.getDefaultColor (numPoints).getRGB ());
  }
}

void GeneticPolygonTestPanel::paintComponent (Graphics g)
{
  super.paintComponent (g);

  motherPanel.paintComponent (g);
  fatherPanel.paintComponent (g);
  childPanel.paintComponent (g);
}

void GeneticPolygonTestPanel::iterateGeneration ()
{
  SI4 numberToMate = (SI4)(GeneticPolygon.matingPercent * (double)populationSize);

  for (SI4 i = 0; i < numberToMate; i++)
    mateRandom ();

  for (SI4 i = 0; i < populationSize; i++)
  {
    if (population[i] != null)
    {
      if (!population[i].age (10000)) // The defaultLifespan is x number of seconds.
        population[i] = null;
    }
  }

  repaint ();      // Repaint indirectly calls paintComponent.
}

void GeneticPolygonTestPanel::mateRandom ()
{

  SI4 randomIndex,
    numTries = 0,
    maxRandomTries = 2 * populationSize;

  do
  {
    randomIndex = (SI4)(Math.random () * (double)(populationSize - 1));
    numTries++;

    if (numTries == maxRandomTries)
    {
      for (randomIndex = 0; randomIndex < populationSize; ++randomIndex)
        if (population[randomIndex] != null)
          break;

      if (randomIndex == populationSize)
      {
        timerOn = false;
        System.out.print ("Error! The entire population is dead!!!\n");
        return;
      } else
        break;
    }
  } while (population[randomIndex] == null);

  GeneticPolygon newFather = population[randomIndex],
    newMother = childPanel.getChild (),
    newChild = new GeneticPolygon (newMother, newFather);

  fatherPanel.setGeneticPolygon (newFather);
  motherPanel.setGeneticPolygon (newMother);
  childPanel.setChild (newChild);

  addChild (newChild);
}

void GeneticPolygonTestPanel::addChild (GeneticPolygon newChild)
{
  SI4 i;
  for (i = 0; i < populationSize; i++)
  {
    if (population[i] == null)
    {
      population[i] = newChild;
      return;
    }
  }
  // We couldn't find a hole so increase the array size;
  GeneticPolygon[] newPopulation = new GeneticPolygon[populationSize + 1];

  for (i = 0; i < populationSize; i++)
    newPopulation[i] = population[i];
  newPopulation[i] = newChild;
  population = newPopulation;
  populationSize++;
}

void StartAction::actionPerformed (ActionEvent e)
{
  timerOn = true;
}

void StopAction::actionPerformed (ActionEvent e)
{
  timerOn = false;
}

void NextAction::actionPerformed (ActionEvent e)
{
  iterateGeneration ();
}


void MateAction::actionPerformed (ActionEvent e)
{
  mateRandom ();

  repaint ();      // Repaint indirectly calls paintComponent.
}

void TimerAction::actionPerformed (ActionEvent e)
{
  if (timerOn)
  {
    iterateGeneration ();
  }
}

}
}
    
