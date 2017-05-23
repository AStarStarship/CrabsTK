/** Kabuki Theater
    @file    /.../Source-Impl/_Theater/HMI/WidgetPage.cpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

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

#include <_HMI/WidgetPage.hpp>
#include <_HMI/Label.hpp>
#include <_HMI/MIDIControl.hpp>
#include <_HMI/AButton.hpp>
#include <_HMI/ControlMatrix.hpp>

namespace _Theater { namespace HMI {

const string WidgetPage::TypeText = "Controls Page";

WidgetPage::WidgetPage (const char* initName, int initNumControlPairs)
:   pageLabel (String (initName) ), 
    numControlPairs (initNumControlPairs),
    mstrControlsEnabled (false)
{
    uint32_t numControlGroupControlPairs;

    if (initNumControlPairs < minControlPairs)
        numControlPairs = minControlPairs;
    else if (initNumControlPairs > maxControlPairs)
        numControlPairs = maxControlPairs;
    else
        numControlPairs = initNumControlPairs;

    numControlGroupControlPairs = initNumControlPairs >> 1; // The floor of initNumControlPairs/2
    
    if (! (numControlPairs & 0x01))
    {
        knobsArray = new MIDIControl*[numControlPairs+1];
        bttnsArray = new AButton*[numControlPairs+1];
    }

    uint32_t i;

    for (i=0; i < numControlPairs; ++i)
    {
        knobsArray[i] = new MIDIControl ((String ("Knob ") += i));
        bttnsArray[i] = new DummyButton ((String ("AButton ") += i));
    }
    
    cntrlGroup1 = new ControlGroup (LAYER_A, numControlGroupControlPairs);
    cntrlGroup2 = new ControlGroup (LAYER_B, numControlGroupControlPairs);
}


WidgetPage::WidgetPage (const WidgetPage &thisPage):
    pageLabel (String (thisPage.)),
    cntrlGroup1 (new ControlGroup (*thisPage.cntrlGroup1)),
    cntrlGroup2 (new ControlGroup (*thisPage.cntrlGroup2)),
    mstrControlsEnabled (thisPage.mstrControlsEnabled)
{
    // Nothing to do here
}



WidgetPage::~WidgetPage ()
{
    delete [] knobsArray;
    delete [] bttnsArray;

    delete cntrlGroup1;
    delete cntrlGroup2;
}

string WidgetPage::getType () {    return typeText;    }

int WidgetPage::getNumControlPairs ()
{
    return numControlPairs;
}

WidgetPage& WidgetPage::operator= (const WidgetPage &thisPage)
{   
    int i;

    delete knobsArray;
    delete bttnsArray;
    
    numControlPairs = thisPage.numControlPairs;
    mstrControlsEnabled = thisPage.mstrControlsEnabled;

    knobsArray = new MIDIControl*[numControlPairs];
    bttnsArray = new AButton*[numControlPairs];

    for (i = 0; i < numControlPairs; ++i)
    {
        knobsArray[i] = thisPage.knobsArray[i];
        bttnsArray[i] = thisPage.bttnsArray[i];
    }

    cntrlGroup1 = thisPage.cntrlGroup1;
    cntrlGroup2 = thisPage.cntrlGroup2;

    return *this;
}

const char* WidgetPage::getLabel ()
{
    return pageLabel;
}
void WidgetPage::setLabel (const char* newName)
{
    pageLabel = newName;
}

ControlGroup *WidgetPage::getControlGroup1 ()
{
    return cntrlGroup1;
}
ControlGroup *WidgetPage::getControlGroup2 ()
{
    return cntrlGroup2;
}

MIDIControl* WidgetPage::getKnob (int index)
{
    if (index >= numControlPairs)
        return nullptr;

    return knobsArray[index];
}

Controls::AButton* WidgetPage::getButton (int index)
{
    if (index >= numControlPairs)
        return nullptr;

    return bttnsArray[index];
}

int WidgetPage::compare (const WidgetPage& thisPage)
{
    int i;

    if (pageLabel != thisPage.label || numControlPairs != thisPage.numControlPairs) 
       return -1;

    for (i=0; i < numControlPairs; ++i)
    {
        int comparisonValue;

        comparisonValue = knobsArray[i]->compare (*thisPage.knobsArray[i]);
        if (!comparisonValue)
            return comparisonValue;

        comparisonValue = bttnsArray[i]->compare (*thisPage.bttnsArray[i]);
        if (!comparisonValue)
            return comparisonValue;
    }
    return 0;
}

bool WidgetPage::masterControlsEnabled ()
{
    return mstrControlsEnabled;
}
void WidgetPage::enableMasterControls ()
{
    mstrControlsEnabled = true;
}
void WidgetPage::disableMasterControls ()
{
    mstrControlsEnabled = false;
}

void WidgetPage::print(_::Log& log)
{
    io << "Page: " << pageLabel << "\n";
    
    io.printLine ('~');
    
    io << "\nMaster Controls:\n";
        
    cntrlGroup1->print (io) + cntrlGroup2->print (io) + 
    io.printLine ('~');
}

}   //< HMI
}   //< _Theater
