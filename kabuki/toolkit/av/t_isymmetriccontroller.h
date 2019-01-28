/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/av/t_t_IsymmetricController.h
@author  Cale McCollough <https://calemccollough.github.io>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AV_1
#ifndef KABUKI_TOOLKIT_AV_ISYMMETRICCONTROLLER
#define KABUKI_TOOLKIT_AV_ISYMMETRICCONTROLLER

#include "t_controller.h"
//#include "t_ButtonColumn.h"
#include "t_control_matrix.h"
#include "t_control_surface.h"
#include "t_device.h"
#include "t_macrobutton.h"
#include "t_paramset.h"

namespace _ {

/* An Interactive Symmetrical Controller Human Machine Interface.
An Interactive Symmetrical (Isymmetric) Control Surface has a variable
amount of modes but, by nature, must have more than 2.

The Symmetry Live! has two constrol surface layers. When both layers are
in the same mode, both layer's are on the same template but both sides
have different. */
class IsymmetricController : public Controller {
 public:
  enum {
   // Max amount of templates  (Should there be one?).
    kMaxWidgets = 0x7FFFFFFF,
   // Max number of mixer channels.
    kMaxMixerChannels = 1024,
   // Number of modes per ParamSet on the SL!
    kNumModes = 5,
   // Number of Button columns on the left and right of the screen.
    kNumButtonColumns = 3,
   // Number of modes per layer.
    kNumModes = 5,
   // Number of macro buttons per layer.
    kNumMacroButtons = 5,
   // Number of page buttons per layer.
    kNumPageButtons = 5,
   // Will change latter.
    kNumGlobalButtons = 4,
   // Number of knobs/buttons in an ControlGroup.
    kNumControlsPerGroup = 4,
   // Number of controls per layer that can.
    kNumControlsPerLayer = 8,
   // Default amount of time for a DBL click.
    kDefaultDoubleClickTime = 100,
   // Flag for ControlGroup is flipped.
    kFlippedControlGroup = 0,
   // Flag for pots control group.
    kPotsControlGroup = 1,
   // Flag for encoder control group.
    kEncoderControlGroup = 2,
   // Number of macro types.
    kNumMacroTypes = 2
  };

  /* Constructs an IsymmetricController with no templates or devices loaded. */
  IsymmetricController()
    : a_ (new ParamSet (ParamSet::A)),
    b_ (new ParamSet (ParamSet::B)),
    c_ (new ParamSet (ParamSet::C)),
    d_ (new ParamSet (ParamSet::D)),
    state_swap_flip_ (false),
    top_controls_a_ (BlankControlGroup),
    top_controls_b_ (BlankControlGroup),
    bottom_controls_a_ (BlankControlGroup),
    bottom_controls_b_ (BlankControlGroup),
    device_count_ (0),
    encoder_acceleration_ (DefaultEncoderAcceleration),
    channel_commone_ (0),
    channel_keyboard_ (1),
    drums_channel_ (7) {
    modes_[MixMode] = new Device ("Mixer");
    modes_[AuxMode] = new Device ("Aux Sends");
    modes_[HostMode] = new Device ("Live!");
    modes_[ModeA] = new Device ("A");
    modes_[ModeB] = new Device ("B");

    mode_ = modes_[MixMode];

    widgets_ = new Device *[MaxTemplates];

    for (SI4 i = 0; i < MaxTemplates; ++i) widgets_[i] = nullptr;
  }

  /* Destructor. */
  ~IsymmetricController() {}

  /* Initializes the MIDI devices. */
  void InitMidiDevices() {
    // numMIDIInDevices = midiInGetNumDevs ();
    // numMIDIOutDevices = midiOutGetNumDevs ();
  }

  /* Checks to see if thisWidget is currently loaded in to the SL! and if the
  states are the same. */
  SI4 Compare(const TWidget& o) {
    // First, find template with same name
    TStrand<> label(o.Label ());
    for (SI4 i = 0; i < device_count_; ++i) {
      if (widgets_[i]->Label ().Compare (label)) {
        return true;
      }
    }
    return false;
  }

  /* The event that happens when a macro button is pressed.
  @contract 0 <= index <= 4 */
  void PressMacro(SI4 index);

  /* The event that happens when a macro button is pressed. */
  void PressMacro(MacroControl* macro);

  /* Flips Layer A and Layer B. */
  void Flip() {
    /*
    if  (a->mode () == b->mode ()) // Then we want to flip the pages on the mode {
        Device *modeA;
        modeA = a->mode ();
        a->setMode  (b->mode ())
        b->setMode  (pageA)
        return;
    }

    ControlGroup *swap_pages_;

    if  (a->state_swap_flip_) { // then unswap pages
    {
        swap_pages_ = a->mode->pages_1_to_4_;
        a->mode->pages_1_to_4_ = a->mode->pages5to8;
        a->mode->pages5to8 = swap_pages_;
    }
    if  (b->state_swap_flip_) // then unswap pages
    {
        swap_pages_ = b->mode->pages_1_to_4_;
        b->mode->pages_1_to_4_ = b->mode->pages5to8;
        b->mode->pages5to8 = swap_pages_;
    }

    swap_pages_ = a->mode->pages5to8;
    a->mode->pages5to8 = b->mode->pages_1_to_4_;
    b->mode->pages_1_to_4_ = swap_pages_;

    if  (is_flipped_)
        is_flipped_ = false;
    else
        is_flipped_ = true;

  // updateLEDs  (sl);
  */
  }

  /* Swaps the encoders for the pots/slidders. */
  void Swap() {
    // This function swaps the pots and the encoders.

    ParamSet *temp = layer_a_;
    layer_a_ = layer_b_;
    layer_b_ = temp;

    Update ();
  }

  /* Switches the mode on layer to the newMode. */
  void SwitchMode (ParamSet* layer, SI4 newMode) {}

  /* Determines if the press was a single click or a DBL click. */
  void PressModeButton(ParamSet* layer, SI4 modeIndex) {
    // Nothing happens until you release the mode button.
    layer->PressModeButton ();
  }

  /* Stores when the last time a mode button was depressed. */
  void DepressModeButton(ParamSet* layer, SI4 modeIndex) {
    // this function switches layer's current mode

    TSS time = time (nullptr), delta_click;

    delta_click = time - layer->LastModePress ();

    if (delta_click > double_press_ticks_) {  // then swap the layer controls back
      holdSwap ();
    } else {
      SwitchMode (layer, mode_);
    }

    /*
    if  (mode == layer->mode->type ()) // same mode pressed
    {//
        if ((time  (nullptr) - layer->LastModePress ()) <= _doubleClickTime) { // Then its a DBL-click and the swap sticks.
            layer->modeBttnPressed = false;
        }
        else { // Then hold down button
            layer->modeBttnPressed = true;
            layer->lastSwapPress = time  (nullptr);
            swap  (sl, layer);
        }
    }
    else { // just change the mode
      if  (layer->layer == LAYER_A)
          a->mode = modes[mode];
      else// then its Layer B
          b->mode = modes[mode];
    }
    */
  }

  /* Gets the template at index in templatesArray.
   @return if index < 0 or > num_Widgets */
  TWidget* GetWidget(SI4 index) {
    if (index < 0) return widgets_[0];
    if (index > device_count_) return widgets_[device_count_ - 1];
    return widgets_[index - 1];
  }

  /* Gets a pointer to thisWidget if thisTempalte is loaded.
  @return Gets 0 if thisWidget is loaded */
  TWidget* FindWidget(const CH1* s) {
    for (SI4 i = 0; i < device_count_; ++i)
      if (StrandCompare (device, widgets_[i]->Label ()->getText ()))
        return widgets_[i];
    return nullptr;
  }

  /* Adds newWidget to the currently loaded templates
  @contract newWidget cannot be 0.
  @contract widget_count_ must be <= MAX_TEMPLATES.
  @contract templatesArray cannot contain a temples with  newWidget's name. */
  BOL AddWidget (TWidget& w) {
    // Check precondiditions
    if (w == nullptr || device_count_ == MaxTemplates) return false;
    CH1 *newDeviceName = w->Label ()->getText ();
    for (SI4 i = 0; i < device_count_; ++i) {
      if (StrandCompare (widgets_[i]->Label ()->getText (), newDeviceName) == 0) {
        return false;
      }
    }
    widgets_[device_count_] = w;
    ++device_count_;
    return true;
  }

  /* Checks to see if a template by thisName is loaded
  @return Gets true if so. */
  BOL ContainsWidget(const CH1* name) {
    for (SI4 i = 0; i < device_count_; ++i) {
      if (!StrandCompare (widgets_[i]->Label ()->getText (), name)) {
        return true;
      }
    }
    return false;
  }

  /* Gets the number of currently loaded templates. */
  SI4 WidgetCount();

  /* Gets the number of templates currently loaded. */
  SI4 WidgetCount();

  /* Gets how fast the encoders accelerate. */
  SI4 EncoderAcceleration();

  /* Gets the common MIDI Channel (0). */
  SI4 ChannelCommon() { return channel_commone_; }

  /* Gets the keyboard MIDI out channel. */
  SI4 ChannelKeyboard() { return channel_keyboard_; }

  /* Sets the channel_keyboard_ to the new value. */
  BOL SetChannelKeyboard (SI4 value) {
    if (value < 1 || value >= 16) return false;
    channel_keyboard_ = value;
    return true;
  }

  /* Gets the drums MIDI out channel. */
  SI4 GetChannelDrums() { return drums_channel_; }

  /* Sets the drums_channel_ to the new value. */
  BOL SetChannelDrums (SI4 value) {
    if (value < 1 || value >= 16) return false;
    drums_channel_ = value;
    return true;
  }

  /* Gets the number of MIDI ticks per quarter note beat. */
  SI4 TicksPerBeat () { return ticks_per_beat_; }

  void SetTicksPerBeat (SI4 newNumTicksPerBeat) {
    if (newNumTicksPerBeat <= MinTicksPerBeat ||
      newNumTicksPerBeat >= MaxTicksPerBeat)
      return;

    ticks_per_beat_ = newNumTicksPerBeat;
  }

  /* Gets the current index of the quarter note subdivisions. */
  SI4 TickCount () { return tick_count_; }

  /* Gets the MIDI tempo. */
  SI4 Tempo () {
    return tempo_;
  }
  
  /* Sets the tempo_ in beats-per-minute. */
  void SetTempo (DBL tempo) {
    if (tempo < 0 || tempo > tempo_max_) return;
    // uint32_t microsecondsPerSecond = 60000000/newTempo;
    tempo_ = tempo;
  }

  /* Syncs the MIDI clock to thisPort. */
  // void SyncToPort  (MIDI::Port * thisPort);

  void ProcessMIDIQuaterNote () {}

  void UpdateMIDIClock () {
    ++tick_count_;

    if (tick_count_ >= ticks_per_beat_) {
      ProcessMIDIQuaterNote ();
      tick_count_ = 0;
    }
  }

  /* Prints this object to a Printer. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << LineStrand ('-') <<
      "\nSymmetrical Control Surface:\n")
      << LineStrand ('-');
    a->Print (o);
    o << LineStrand ('-');
    b->Print (o);
    o << LineStrand ('-');
    c->Print (o);
    o << LineStrand ('-'); 
    d->Print (o);
    return o << LineStrand ('-');
  }

  /* Inter-process subroutines. */
  const Op* Star (CHW index, Expr* expr) {
    return nullptr;
  }

 private:
  TWidget *a_,                    //< Layer A
      *b_,                       //< Layer B
      *c_,                       //< Layer C
      *d_;                       //< Layer D
  TControlMatrix *top_controls_a_,  //< Top controls a-b.
      *top_controls_b_,            //< Top controls b-c.
      *bottom_controls_a_,         //< Bottom controls a-b.
      *bottom_controls_b_;         //< Bottom controls c-d.
  TWidget* mode_;                 //< Current mode's template.
  TArray<TWidget*> modes_,        //< 5 different modes on the Symmetry Live!
      widgets_;                   //< Widgets array.
  TButtonColumn *macro_,  //< Macros for the current mode.
      *macros_mixer_,             //< Mixer macros.
      *macros_aux_,               //< Aux send macros.
      *macros_device_;            //< Device macros.
  SI4 widget_count_,               //< Number of templates currently loaded.
      encoder_acceleration_,      //< How fast the encoders accelerate.
      channel_commone_,            //< Common MIDI Channel  (0).
      channel_keyboard_,          //< Keyboard MIDI out channel.
      drums_channel_,             //< Drums MIDI out channel.
      ticks_per_beat_,          //< Number of MIDI ticks per quarter note beat.
      tick_count_,            //< Current index of the quarter note subdivisions.
      tempo_,                //< MIDI tempo.
      state_swap_flip_;  //< Represents if the controls are swapped
  //_::Array<MIDIDevice> devices; //< array of all of the MIDI devices currently
  // attached to this system. SI4 numMIDIInDev,            //< Number of MIDI
  // input devices attached to the system.
  //     numMIDIOutDev;          //< Number of MIDI output devices attached to
  //     the system.
  UI4 double_press_ticks_;        //< TArray of the DBL click time.

  /* Switches the mode or current page on Layer A and Layer B.
  @contract state_swap_flip_ must be true */
  void HoldSwap() {
    if (!state_swap_flip_) return;
  }
};
}  // namespace _
#endif  //< KABUKI_TOOLKIT_AV_ISYMMETRICCONTROLLER
#endif  //< #if SEAM >= KABUKI_TOOLKIT_AV_1
