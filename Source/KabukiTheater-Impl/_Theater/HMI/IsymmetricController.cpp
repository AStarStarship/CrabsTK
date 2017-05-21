/** Kabuki Theater
    @file    /.../Source/KabukiTheater-Impl/_Theater/HMI/IsymmetricController.cpp
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
 

#include <_Theater/HMI/IsymmetricController.hpp>
//#include <_Theater/HMI/ButtonColumn.hpp>
#include <_Theater/HMI/ControlMatrix.hpp>
#include <_Theater/HMI/ControlLayer.hpp>
#include <_Theater/HMI/MacroButton.hpp>
#include <_Theater/HMI/Device.hpp>
#include <_Theater/HMI/ControlSurface.hpp>

namespace _Theater { namespace HMI {

IsymmetricController::IsymmetricController ()
:   a  (new ControlLayer  (ControlLayer::A)),
    b  (new ControlLayer  (ControlLayer::B)),
    c  (new ControlLayer  (ControlLayer::C)),
    d  (new ControlLayer  (ControlLayer::D)),
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

void IsymmetricController::Swap ()
{
    // This function swaps the pots and the encoders.

    ControlLayer *temp = layerA;
    layerA = layerB;
    layerB = temp;

    update ();
}

void IsymmetricController::SwitchMode  (ControlLayer *thisLayer, Mode newMode)
{
    ;
}

void IsymmetricController::PressModeButton  (ControlLayer *thisLayer, Mode thisMode)
{
    // Nothing happens until you release the mode button

    thisLayer->PressModeButton ();
}

void IsymmetricController::DepressModeButton  (ControlLayer *thisLayer, Mode thisMode)
{
    // this function switches thisLayer's current mode

    time_t currentTime = time  (nullptr),
           doubleClickVector;

    doubleClickVector = currentTime - thisLayer->LastModePress ();

    if  (doubleClickVector > _doubleClickTime) // then swap the layer controls back
    {
        HoldSwap ();
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

void IsymmetricController::HoldSwap ()
{
    if (!swappedFlippedState)
        return;

}

int IsymmetricController::Compare  (const Device& that)
{
    //First, find template with same name
    String thatsName = that.label ();

    for  (int i=0; i < numDevices; ++i)
    {
        if  (templatesArray[i]->GetLabel () == thatsName)
        {
        }

    }
    return false;
}

Device *IsymmetricController::GetTemplates  (int thisIndex)
{
    if  (thisIndex < 0)
        return templatesArray[0];
    if  (thisIndex > numDevices)
        return templatesArray[numDevices-1];
    return templatesArray[thisIndex-1];
}

Device *IsymmetricController::FindDevice  (char *thisDevice)
{
    for  (int i=0; i < numDevices; ++i)
        if  (strcmp  (thisDevice, templatesArray[i]->GetLabel ()->GetText ()))
            return templatesArray[i];
    return nullptr;
}

bool IsymmetricController::ContainsDevice  (const char thisName[])
{
    for  (int i=0; i < numDevices; ++i)
        if  (strcmp  (templatesArray[i]->GetLabel ()->GetText (), thisName) == 0)
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

    char *newDeviceName = newDevice->GetLabel ()->GetText ();

    for  (int i=0; i < numDevices; ++i)
        if  (strcmp  (templatesArray[i]->GetLabel ()->GetText (), newDeviceName) == 0)
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



void IsymmetricController::UpdateMIDIClock ()
{
    ++tickCount;

    if  (tickCount >= numTicksPerBeat)
    {
        processMIDIQuaterNote ();
        tickCount = 0;
    }
}



void IsymmetricController::ProcessMIDIQuaterNote ()
{
    // Write me!!!
}

byte IsymmetricController::GetState ()
{
    return 0;
}

const char* IsymmetricController::SetState (byte Value)
{
    return 0;
}

const char* IsymmetricController::Do (const char* Query, byte Index, Roombot* Bot)
{
    switch (Index)
    {
        case 0: return I2P::NumMembers (0);
    }
    
    return Query ? Enquery ("IsymmetricController", "_Theater/HMI"): InvalidIndex ();
}

/** Returns a text representation of this object. */
string IsymmetricController::toString ()
{
    string text = ConsoleLine ('~') + "Symmetrical Control Surface:\n" + ConsoleLine ('~');

    return text + a->toString () + "\n" + ConsoleLine ('-') +
        b->toString () + "\n" + ConsoleLine ('-') +
        c->toString () + "\n" + ConsoleLine ('-') +
        d->toString () + "\n" + ConsoleLine ('-');
}

}   //< HMI
}   //< _Theater
