/** Kabuki Theater
    @file    /.../Source/_HMI/IsymmetricController.hpp
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
 

#pragma once

#include <_HMI/Controller.hpp>

namespace _HMI {

class IsymmetricController: public Controller
/*< An Interactive Symmetrical Controller Human Machine Interface.
    An Interactive Symmetrical (Isymmetric) Control Surface has a variable amount of modes but, by nature, must have more than 2.

    The Symmetry Live! has two constrol surface layers. When both layers are in the same mode, both layer's
    are on the same template but both sides have different.
*/
{
    public:

    enum {
        kMaxWidgets             = 0x7FFFFFFF,   
        //< The maximum amount of templates  (Should there be one?).
        kMaxMixerChannels       = 1024,
        //< The max number of mixer channels.
        kNumModes               = 5,
        //< The number of modes per ParameterSet on the SL!
        kNumButtonColumns       = 3,
        //< The number of Button columns on the left and right of the screen.
        kNumModes               = 5,
        //< Number of modes per layer.
        kNumMacroButtons        = 5,
        //< Number of macro buttons per layer.
        kNumPageButtons         = 5,
        //< Number of page buttons per layer.
        kNumGlobalButtons       = 4,
        //< This will change latter.
        kNumControlsPerGroup    = 4,
        //< The number of knobs/buttons in an ControlGroup.
        kNumControlsPerLayer    = 8,
        //< The number of controls per layer that can.
        kDefaultDoubleClickTime = 100,
        //< this is the default amount of time for a double click.
        kFlippedControlGroup    = 0,
        //< Flag for ControlGroup is flipped.
        kPotsControlGroup       = 1,
        //< Flag for pots control group.
        kEncoderControlGroup    = 2,
        //< Flag for encoder cotnorl group.
        kNumMacroTypes          = 2
        //< The number of macro types.
    };
    
    IsymmetricController ();
    //< Constructs an IsymmetricController with no templates or devices loaded.
    
    ~IsymmetricController ();
    //< Destructor.
    
    void initMIDIDevices ();
    //< Initializes the MIDI devices.

    int Compare  (const Widget& other);
    //< Checks to see if thisWidget is currently loaded in to the SL! and if the states are the same.

    void pressMacro  (int thisIndex);
    /*< The event that happens when a macro button is pressed.
        @pre 0 <= thisIndex <= 4 */
        
    void pressMacro  (MacroControl* thisMacro);
    //< The event that happens when a macro button is pressed.

    void Flip ();
    //< Flips Layer A and Layer B.
    
    void Swap ();
    //< Swaps the encoders for the pots/slidders.

    void SwitchMode  (ParameterSet* layer, int newMode);
    //< Switches the mode on layer to the newMode.

    void PressModeButton  (ParameterSet* layer, int modeIndex);
    //< Determines if the press was a single click or a double click.
    
    void DepressModeButton  (ParameterSet* layer, int modeIndex);
    //< Stores when the last time a mode button was depressed.

    Widget* GetWidget  (int index);
    /*< Gets the template at thisIndex in templatesArray.
        @return if thisIndex < 0 or > num_Widgets */

    Widget* FindWidget  (const char* s);
    /*< Gets a pointer to thisWidget if thisTempalte is loaded.
         @return Gets 0 if thisWidget is loaded */

    bool AddWidget  (Widget& w);
    /*< Adds newWidget to the currently loaded templates
        @pre newWidget cannot be 0.
        @pre numWidgets must be <= MAX_TEMPLATES.
        @pre templatesArray cannot contain a temples with  newWidget's name.
    */

    bool ContainsWidget  (const char* thisName);
    /*< Checks to see if a template by thisName is loaded
        @return Gets true if so. 
    */

    int GetNumWidgets ();
    //< Gets the number of currently loaded templates

    int GetNumWidgets ();
    //< Gets the number of templates currently loaded.
    
    int GetEncoderAcceleration ();
    //< Gets how fast the encoders accelerate.

    int GetCommonChannel ();
    //< Gets the common MIDI Channel  (0).
    
    int GetKeyboardChannel ();
    //< Gets the keyboard MIDI out channel.
    
    int GetDrumsChannel ();
    //< Gets the drums MIDI out channel.

    int GetNumTicksPerBeat ();
    //< Gets the number of MIDI ticks per quarter note beat.
    
    int GetTickCount ();
    //< Gets the current index of the quarter note subdivisions.
    
    int GetTempo ();
    //< Gets the MIDI tempo.

    //void SyncToPort  (MIDI::Port * thisPort);
    //< Syncs the MIDI clock to thisPort.
    
    const _::Member* Op (byte index, Verifier* io);
    /*< Inter-process subroutines. */

    void Print() const;
    /*< Prints this object to a Terminal. */

    private:

    Widget* a,                     //< Layer A
        * b,                       //< Layer B
        * c,                       //< Layer C
        * d;                       //< Layer D

    ControlMatrix* topControlsA,   //< The top controls a-b.
        * topControlsB,            //< The top controls b-c.
        * bottomControlsA,         //< The bottom controls a-b.
        * bottomControlsB;         //< The bottom controls c-d.

    Widget* mode;                //< The current mode's template.
    std::vector<Widget*> modes,  //< The 5 different modes on the Symmetry Live!
        templatesArray;            /*< The currently loaded templates. */
    
    ButtonColumn* currentMacros,   //< The macros for the current mode.
        * mixerMacros,             //< The mixer macros.
        * auxMacros,               //< The aux send macros.
        * deviceMacros;            //< The device macros.

    int numWidgets,              //< The number of templates currently loaded.
        encoderAcceleration,       //< How fast the encoders accelerate.

        commonChannel,             //< The common MIDI Channel  (0).
        keyboardChannel,           //< The keyboard MIDI out channel.
        drumsChannel,              //< The drums MIDI out channel.

        numTicksPerBeat,           //< The number of MIDI ticks per quarter note beat.
        tickCount,                 //< The current index of the quarter note subdivisions.
        tempo,                     //< The MIDI tempo.
        swappedFlippedState;       //< Represents if the controls are swapped

    //std::vector<MIDIDevice> devices;       //< An array of all of the MIDI devices currently attached to this system.

    //int numMIDIInDev,            //< The number of MIDI input devices attached to the system.
    //     numMIDIOutDev;          //< The number of MIDI output devices attached to the system.

    Time doubleClickTime;          //< The vector of the double click time.

    IPAddress controlSurfaceIP;    //< The IPAddress of this control surface.

    void HoldSwap ();
    /*< Switches the mode or current page on Layer A and Layer B.
        @pre swappedFlippedState must be true */

    /** @todo
        - Write code to determine how many MIDI devices are connected to the computer and get their handles.
        - Determine what the fuck I was doing with the macro lists??? Should they have their own data struct?
     */
};

}   //< _HMI
}   //< _Theater
