/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/features/hmi/hmi_/IsymmetricController.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= SEAM_00_03_00_00__00
#ifndef HEADER_FOR_KT_HMI_ISYMMETRICCONTROLLER
#define HEADER_FOR_KT_HMI_ISYMMETRICCONTROLLER

#include "Controller.h>

namespace _ {

/* An Interactive Symmetrical Controller Human Machine Interface.
    An Interactive Symmetrical (Isymmetric) Control Surface has a variable
    amount of modes but, by nature, must have more than 2.

    The Symmetry Live! has two constrol surface layers. When both layers are
    in the same mode, both layer's are on the same template but both sides
    have different.
*/
class IsymmetricController : public Controller {
 public:
  enum {
    kMaxWidgets = 0x7FFFFFFF,
    //< Max amount of templates  (Should there be one?).
    kMaxMixerChannels = 1024,
    //< Max number of mixer channels.
    kNumModes = 5,
    //< Number of modes per ParameterSet on the SL!
    kNumButtonColumns = 3,
    //< Number of Button columns on the left and right of the screen.
    kNumModes = 5,
    //< Number of modes per layer.
    kNumMacroButtons = 5,
    //< Number of macro buttons per layer.
    kNumPageButtons = 5,
    //< Number of page buttons per layer.
    kNumGlobalButtons = 4,
    //< Will change latter.
    kNumControlsPerGroup = 4,
    //< Number of knobs/buttons in an ControlGroup.
    kNumControlsPerLayer = 8,
    //< Number of controls per layer that can.
    kDefaultDoubleClickTime = 100,
    //< Default amount of time for a double click.
    kFlippedControlGroup = 0,
    //< Flag for ControlGroup is flipped.
    kPotsControlGroup = 1,
    //< Flag for pots control group.
    kEncoderControlGroup = 2,
    //< Flag for encoder control group.
    kNumMacroTypes = 2
    //< Number of macro types.
  };

  /* Constructs an IsymmetricController with no templates or devices loaded. */
  IsymmetricController();

  /* Destructor. */
  ~IsymmetricController();

  /* Initializes the MIDI devices. */
  void InitMidiDevices();

  /* Checks to see if thisWidget is currently loaded in to the SL! and if the
   * states are the same. */
  int Compare(const Widget& other);

  /* The event that happens when a macro button is pressed.
      @pre 0 <= index <= 4 */
  void PressMacro(int index);

  /* The event that happens when a macro button is pressed. */
  void PressMacro(MacroControl* thisMacro);

  /* Flips Layer A and Layer B. */
  void Flip();

  /* Swaps the encoders for the pots/slidders. */
  void Swap();

  /* Switches the mode on layer to the newMode. */
  void SwitchMode(ParameterSet* layer, int newMode);

  /* Determines if the press was a single click or a double click. */
  void PressModeButton(ParameterSet* layer, int modeIndex);

  /* Stores when the last time a mode button was depressed. */
  void DepressModeButton(ParameterSet* layer, int modeIndex);

  /* Gets the template at index in templatesArray.
      @return if index < 0 or > num_Widgets */
  Widget* GetWidget(int index);

  /* Gets a pointer to thisWidget if thisTempalte is loaded.
       @return Gets 0 if thisWidget is loaded */
  Widget* FindWidget(const char* s);

  /* Adds newWidget to the currently loaded templates
      @pre newWidget cannot be 0.
      @pre numWidgets must be <= MAX_TEMPLATES.
      @pre templatesArray cannot contain a temples with  newWidget's name.
  */
  bool AddWidget(Widget& w);

  /* Checks to see if a template by thisName is loaded
      @return Gets true if so.
  */
  bool ContainsWidget(const char* thisName);

  /* Gets the number of currently loaded templates. */
  int GetNumWidgets();

  /* Gets the number of templates currently loaded. */
  int GetNumWidgets();

  /* Gets how fast the encoders accelerate. */
  int GetEncoderAcceleration();

  /* Gets the common MIDI Channel (0). */
  int GetCommonChannel();

  /* Gets the keyboard MIDI out channel. */
  int GetKeyboardChannel();

  /* Gets the drums MIDI out channel. */
  int GetDrumsChannel();

  /* Gets the number of MIDI ticks per quarter note beat. */
  int GetNumTicksPerBeat();

  /* Gets the current index of the quarter note subdivisions. */
  int GetTickCount();

  /* Gets the MIDI tempo. */
  int GetTempo();

  /* Syncs the MIDI clock to thisPort. */
  // void SyncToPort  (MIDI::Port * thisPort);

  /* Inter-process subroutines. */
  const _::Op* Star(wchar_t index, _::Expression* expr);

  /* Prints this object to a Expression. */
  void Print() const;

 private:
  Widget *a,                    //< Layer A
      *b,                       //< Layer B
      *c,                       //< Layer C
      *d;                       //< Layer D
  ControlMatrix *topControlsA,  //< Top controls a-b.
      *topControlsB,            //< Top controls b-c.
      *bottomControlsA,         //< Bottom controls a-b.
      *bottomControlsB;         //< Bottom controls c-d.
  Widget* mode;                 //< Current mode's template.
  _::Array<Widget*> modes,      //< 5 different modes on the Symmetry Live!
      templatesArray;           //< Currently loaded templates.
  ButtonColumn *currentMacros,  //< Macros for the current mode.
      *mixerMacros,             //< Mixer macros.
      *auxMacros,               //< Aux send macros.
      *deviceMacros;            //< Device macros.
  int numWidgets,               //< Number of templates currently loaded.
      encoderAcceleration,      //< How fast the encoders accelerate.
      commonChannel,            //< Common MIDI Channel  (0).
      keyboardChannel,          //< Keyboard MIDI out channel.
      drumsChannel,             //< Drums MIDI out channel.
      numTicksPerBeat,          //< Number of MIDI ticks per quarter note beat.
      tickCount,            //< Current index of the quarter note subdivisions.
      tempo,                //< MIDI tempo.
      swappedFlippedState;  //< Represents if the controls are swapped
  //_::Array<MIDIDevice> devices; //< array of all of the MIDI devices currently
  // attached to this system. int numMIDIInDev,            //< Number of MIDI
  // input devices attached to the system.
  //     numMIDIOutDev;          //< Number of MIDI output devices attached to
  //     the system.
  Time doubleClickTime;        //< vector of the double click time.
  IPAddress controlSurfaceIP;  //< IPAddress of this control surface.

  /* Switches the mode or current page on Layer A and Layer B.
      @pre swappedFlippedState must be true */
  void HoldSwap();

  /* @todo
      - Write code to determine how many MIDI devices are connected to the
     computer and get their handles.
      - Determine what the fuck I was doing with the macro lists??? Should they
     have their own data struct?
   */
};  //< class IsymmetricController
}  // namespace _
#endif  //< HEADER_FOR_KT_HMI_ISYMMETRICCONTROLLER
#endif  //< #if SEAM >= SEAM_00_03_00_00__00
