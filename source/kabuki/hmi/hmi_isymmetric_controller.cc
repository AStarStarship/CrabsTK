/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/isymmetric_controller.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version 
             2.0 (the "License"); you may not use this file except in 
             compliance with the License. You may obtain a copy of the License 
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless 
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or 
             implied. See the License for the specific language governing 
             permissions and limitations under the License.
*/
 
#include "isymmetric_controller.h"
//#include "ButtonColumn.h"
#include "control_matrix.h"
#include "parameter_set.h"
#include "macro_cutton.h"
#include "device.h"
#include "control_surface.h"

namespace kabuki { namespace hmi {

IsymmetricController::IsymmetricController ()
:   a  (new ParameterSet  (ParameterSet::A)),
    b  (new ParameterSet  (ParameterSet::B)),
    c  (new ParameterSet  (ParameterSet::C)),
    d  (new ParameterSet  (ParameterSet::D)),
    swappedFlippedState  (false),
    topControlsA  (BlankControlGroup),
    topControlsB  (BlankControlGroup),
    bottomControlsA  (BlankControlGroup),
    bottomControlsB  (BlankControlGroup),
    numDevices  (0),
    encoderAcceleration  (DefaultEncoderAcceleration),
    commonChannel  (0),
    keyboardChannel  (1),
    drumsChannel  (7)
{
    modes[MixMode]  = new Device ("Mixer");
    modes[AuxMode]  = new Device ("Aux Sends");
    modes[HostMode] = new Device ("Live!");
    modes[ModeA]    = new Device ("A");
    modes[ModeB]    = new Device ("B");

    mode = modes[MixMode];

    templatesArray = new Device * [MaxTemplates];

    for  (int i=0; i < MaxTemplates; ++i)
        templatesArray[i] = nullptr;
}

IsymmetricController::~IsymmetricController ()
{
    delete mixerMacros;
    delete auxMacros;
    delete deviceMacros;

    delete layerA;
    delete layerB;
    delete [] templatesArray;
}

void IsymmetricController::InitMIDIDevices ()
{
    //numMIDIInDevices = midiInGetNumDevs ();
    //numMIDIOutDevices = midiOutGetNumDevs ();
}

void IsymmetricController::Flip ()
{
    /*
    if  (a->mode () == b->mode ()) // Then we want to flip the pages on the mode
    {
        Device *modeA;
        modeA = a->mode ();
        a->setMode  (b->mode ())
        b->setMode  (pageA)
        return;
    }

    ControlGroup *swapPages;

    if  (a->swappedFlippedState) // then unswap pages
    {
        swapPages = a->mode->pages1to4;
        a->mode->pages1to4 = a->mode->pages5to8;
        a->mode->pages5to8 = swapPages;
    }
    if  (b->swappedFlippedState) // then unswap pages
    {
        swapPages = b->mode->pages1to4;
        b->mode->pages1to4 = b->mode->pages5to8;
        b->mode->pages5to8 = swapPages;
    }

    swapPages = a->mode->pages5to8;
    a->mode->pages5to8 = b->mode->pages1to4;
    b->mode->pages1to4 = swapPages;

    if  (isFlipped)
        isFlipped = false;
    else
        isFlipped = true;

 // updateLEDs  (sl);
 */
}

void IsymmetricController::swap ()
{
    // This function swaps the pots and the encoders.

    ParameterSet *temp = layerA;
    layerA = layerB;
    layerB = temp;

    update ();
}

void IsymmetricController::switchMode  (ParameterSet *thisLayer, Mode newMode)
{
    ;
}

void IsymmetricController::pressModeButton  (ParameterSet *thisLayer, Mode thisMode)
{
    // Nothing happens until you release the mode button

    thisLayer->pressModeButton ();
}

void IsymmetricController::depressModeButton  (ParameterSet *thisLayer, Mode thisMode)
{
    // this function switches thisLayer's current mode

    time_t currentTime = time  (nullptr),
           doubleClickVector;

    doubleClickVector = currentTime - thisLayer->LastModePress ();

    if  (doubleClickVector > _doubleClickTime) // then swap the layer controls back
    {
        holdSwap ();
    }
    else
        switchMode  (thisLayer, thisMode);

    /*
    if  (thisMode == thisLayer->mode->type ()) // same mode pressed
    {//
        if ((time  (nullptr) - thisLayer->LastModePress ()) <= _doubleClickTime) // Then its a double-click
        {// and the swap sticks
            thisLayer->modeBttnPressed = false;
        }
        else // Then hold down button
        {
            thisLayer->modeBttnPressed = true;
            thisLayer->lastSwapPress = time  (nullptr);
            swap  (sl, thisLayer);
        }
    }
    else // just change the mode
    {
        if  (thisLayer->layer == LAYER_A)
            a->mode = modes[thisMode];
        else// then its Layer B
            b->mode = modes[thisMode];
    }
    */
}

void IsymmetricController::holdSwap ()
{
    if (!swappedFlippedState)
        return;

}

int IsymmetricController::compare  (const Device& that)
{
    //First, find template with same name
    String thatsName = that.label ();

    for  (int i=0; i < numDevices; ++i)
    {
        if  (templatesArray[i]->getLabel () == thatsName)
        {
        }

    }
    return false;
}

Device *IsymmetricController::GetTemplates  (int index)
{
    if  (index < 0)
        return templatesArray[0];
    if  (index > numDevices)
        return templatesArray[numDevices-1];
    return templatesArray[index-1];
}

Device *IsymmetricController::findDevice  (char *device)
{
    for  (int i=0; i < numDevices; ++i)
        if  (strcmp  (device, templatesArray[i]->getLabel ()->getText ()))
            return templatesArray[i];
    return nullptr;
}

bool IsymmetricController::containsDevice  (const char thisName[])
{
    for  (int i=0; i < numDevices; ++i)
        if  (strcmp  (templatesArray[i]->getLabel ()->getText (), thisName) == 0)
            return true;
    return false;
}

int IsymmetricController::GetNumDevices ()
{
    return numDevices;
}

bool IsymmetricController::AddDevice  (Device *newDevice)
{
    // Check precondiditions
    if  (newDevice == nullptr || numDevices == MaxTemplates)
        return false;

    char *newDeviceName = newDevice->getLabel ()->getText ();

    for  (int i=0; i < numDevices; ++i)
        if  (strcmp  (templatesArray[i]->getLabel ()->getText (), newDeviceName) == 0)
            return false;

    //Done checking pre conditions

    // Add template

    templatesArray[numDevices] = newDevice;
    ++numDevices;

    return true;
}

int IsymmetricController::GetCommonChannel ()
{
    return commonChannel;
}

int IsymmetricController::GetKeyboardChannel ()
{
    return keyboardChannel;
}

void IsymmetricController::SetKeyboardChannel  (int value)
{
    if  (value < 1)
        keyboardChannel = 1;
    else
    if  (value >= 16)
        keyboardChannel = 16;
    else
        keyboardChannel = value;
}

int IsymmetricController::GetDrumsChannel ()
{
    return drumsChannel;
}
void IsymmetricController::SetDrumsChannel  (int value)
{
    if  (value < 1)
        drumsChannel = 1;
    else
    if  (value >= 16)
        drumsChannel = 16;
    else
        drumsChannel = value;
}


void IsymmetricController::SetTicksPerBeat  (int newNumTicksPerBeat)
{
    if  (newNumTicksPerBeat <= MinTicksPerBeat ||
        newNumTicksPerBeat >= MaxTicksPerBeat)
        return;

    numTicksPerBeat = newNumTicksPerBeat;
}

int IsymmetricController::GetTicksPerBeat ()
{
    return numTicksPerBeat;
}

void IsymmetricController::SetTempo  (double newTempo)
{
    if  (theTempo < 0 || theTempo > MaxTempo)
        return;
    //long microsecondsPerSecond = 60000000/newTempo;
    theTempo = newTempo;
}



void IsymmetricController::updateMIDIClock ()
{
    ++tickCount;

    if  (tickCount >= numTicksPerBeat)
    {
        processMIDIQuaterNote ();
        tickCount = 0;
    }
}



void IsymmetricController::processMIDIQuaterNote ()
{
    // Write me!!!
}

const Operation* IsymmetricController::op (_::Expression* io, byte index)
{
    switch (index)
    {
        case '?': return _::deviceHeader<"IsymmetricController", 0, "_HMI"> ();
    }
    
    return _::invalidMember ();
}

/** Returns a text representation of this object. */
void IsymmetricController::print ()
{
    _::printLine ('~');
    printf ("Symmetrical Control Surface:\n");
    _::printLine ('~');

    a->print ();
    _::printNL ();
    _::printLine ('-');
    b->print ();
    _::printNL (); 
    _::printLine ('-') +
    c->print ();
    _::printNL (); 
    _::printLine ('-') +
    d->print ();
    _::printNL (); 
    _::printLine ('-');
}

}   //< namespace hmi
}   //< namespace kabuki
