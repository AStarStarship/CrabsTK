// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#ifndef CRABSTK_TOUCH_CONTROLLERISYMMETRIC_DECL_DECL
#define CRABSTK_TOUCH_CONTROLLERISYMMETRIC_DECL_DECL
#include "Controller.hpp"
//#include "ButtonColumn.hpp"
#include "ControlMatrix.hpp"
#include "ControlSurface.hpp"
#include "Device.hpp"
#include "ButtonMacro.hpp"
#include "ParamSet.hpp"
namespace _ {

/* An Interactive Symmetrical Controller Human Machine Interface.
An Interactive Symmetrical (Isymmetric) Control Surface has a variable
amount of modes but, by nature, must have more than 2.

The Symmetry Live! has two constrol surface layers. When both layers are
in the same mode, both layer's are on the same template but both sides
have different. */
class ControllerIsymmetric : public Controller {
 public:
  enum {
    // Max amount of templates  (Should there be one?).
    cMaxWidgets = 0x7FFFFFFF,
    // Max number of mixer channels.
    cMaxMixerChannels = 1024,
    // Number of modes per ParamSet on the SL!
    cNumModes = 5,
    // Number of Button columns on the left and right of the screen.
    cNumButtonColumns = 3,
    // Number of modes per layer.
    cNumModes = 5,
    // Number of macro buttons per layer.
    cNumMacroButtons = 5,
    // Number of page buttons per layer.
    cNumPageButtons = 5,
    // Will change latter.
    cNumGlobalButtons = 4,
    // Number of knobs/buttons in an ControlGroup.
    cNumControlsPerGroup = 4,
    // Number of controls per layer that can.
    cNumControlsPerLayer = 8,
    // Default amount of time for a FPD click.
    cDefaultDoubleClickTime = 100,
    // Flag for ControlGroup is flipped.
    cFlippedControlGroup = 0,
    // Flag for pots control group.
    cPotsControlGroup = 1,
    // Flag for encoder control group.
    cEncoderControlGroup = 2,
    // Number of macro types.
    cNumMacroTypes = 2
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

    for (ISC i = 0; i < MaxTemplates; ++i) widgets_[i] = nullptr;
  }

  /* Destructor. */
  ~IsymmetricController() {}

  /* Initializes the MIDI devices. */
  void InitMidiDevices() {
    // numMIDIInDevices = midiInGetNumDevs ();
    // numMIDIOutDevices = midiOutGetNumDevs ();
  }

  /* Checks to see if thisWidget is currently loaded in to the SL! and if the
  states_ are the same. */
  ISC Compare(const TWidget& o) {
    // First, find template with same name
    TString<> label(o.Label ());
    for (ISC i = 0; i < device_count_; ++i) {
      if (widgets_[i]->Label ().Compare (label)) {
        return true;
      }
    }
    return false;
  }

  /* The event that happens when a macro button is pressed.
  @contract 0 <= index <= 4 */
  void PressMacro(ISC index);

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
  void SwitchMode (ParamSet* layer, ISC newMode) {}

  /* Determines if the press was a single click or a FPD click. */
  void PressModeButton(ParamSet* layer, ISC modeIndex) {
    // Nothing happens until you release the mode button.
    layer->PressModeButton ();
  }

  /* Stores when the last time a mode button was depressed. */
  void DepressModeButton(ParamSet* layer, ISC modeIndex) {
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
        if ((time  (nullptr) - layer->LastModePress ()) <= _doubleClickTime) { // Then its a FPD-click and the swap sticks.
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
  TWidget* GetWidget(ISC index) {
    if (index < 0) return widgets_[0];
    if (index > device_count_) return widgets_[device_count_ - 1];
    return widgets_[index - 1];
  }

  /* Gets a pointer to thisWidget if thisTempalte is loaded.
  @return Gets 0 if thisWidget is loaded */
  TWidget* FindWidget(const CHA* s) {
    for (ISC i = 0; i < device_count_; ++i)
      if (StringCompare (device, widgets_[i]->Label ()->Text ()))
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
    CHA *newDeviceName = w->Label ()->Text ();
    for (ISC i = 0; i < device_count_; ++i) {
      if (StringCompare (widgets_[i]->Label ()->Text (), newDeviceName) == 0) {
        return false;
      }
    }
    widgets_[device_count_] = w;
    ++device_count_;
    return true;
  }

  /* Checks to see if a template by thisName is loaded
  @return Gets true if so. */
  BOL ContainsWidget(const CHA* name) {
    for (ISC i = 0; i < device_count_; ++i) {
      if (!StringCompare (widgets_[i]->Label ()->Text (), name)) {
        return true;
      }
    }
    return false;
  }

  /* Gets the number of currently loaded templates. */
  ISC WidgetCount();

  /* Gets the number of templates currently loaded. */
  ISC WidgetCount();

  /* Gets how fast the encoders accelerate. */
  ISC EncoderAcceleration();

  /* Gets the common MIDI Channel (0). */
  ISC ChannelCommon() { return channel_commone_; }

  /* Gets the keyboard MIDI out channel. */
  ISC ChannelKeyboard() { return channel_keyboard_; }

  /* Sets the channel_keyboard_ to the new value. */
  BOL SetChannelKeyboard (ISC value) {
    if (value < 1 || value >= 16) return false;
    channel_keyboard_ = value;
    return true;
  }

  /* Gets the drums MIDI out channel. */
  ISC GetChannelDrums() { return drums_channel_; }

  /* Sets the drums_channel_ to the new value. */
  BOL SetChannelDrums (ISC value) {
    if (value < 1 || value >= 16) return false;
    drums_channel_ = value;
    return true;
  }

  /* Gets the number of MIDI ticks per quarter note beat. */
  ISC TicksPerBeat () { return ticks_per_beat_; }

  void SetTicksPerBeat (ISC newNumTicksPerBeat) {
    if (newNumTicksPerBeat <= MinTicksPerBeat ||
      newNumTicksPerBeat >= MaxTicksPerBeat)
      return;

    ticks_per_beat_ = newNumTicksPerBeat;
  }

  /* Gets the current index of the quarter note subdivisions. */
  ISC TickCount () { return tick_count_; }

  /* Gets the MIDI tempo. */
  ISC Tempo () {
    return tempo_;
  }
  
  /* Sets the tempo_ in beats-per-minute. */
  void SetTempo (FPD tempo) {
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
    o << LineString ('-') <<
      "\nSymmetrical Control Surface:\n")
      << LineString ('-');
    a->Print (o);
    o << LineString ('-');
    b->Print (o);
    o << LineString ('-');
    c->Print (o);
    o << LineString ('-'); 
    d->Print (o);
    return o << LineString ('-');
  }

  /* Inter-process subroutines. */
  const Op* Star (CHW index, Expr* expr) {
    return nullptr;
  }

 private:
  TWidget *a_,                     //< Layer A
      *b_,                         //< Layer B
      *c_,                         //< Layer C
      *d_;                         //< Layer D
  TControlMatrix *top_controls_a_, //< Top controls a-b.
      *top_controls_b_,            //< Top controls b-c.
      *bottom_controls_a_,         //< Bottom controls a-b.
      *bottom_controls_b_;         //< Bottom controls c-d.
  TWidget* mode_;                  //< Current mode's template.
  TArray<TWidget*> modes_,         //< 5 different modes on the Symmetry Live!
      widgets_;                    //< Widgets array.
  TButtonColumn *macro_,           //< Macros for the current mode.
      *macros_mixer_,              //< Mixer macros.
      *macros_aux_,                //< Aux send macros.
      *macros_device_;             //< Device macros.
  ISC widget_count_,               //< Number of templates currently loaded.
      encoder_acceleration_,       //< How fast the encoders accelerate.
      channel_commone_,            //< Common MIDI Channel  (0).
      channel_keyboard_,           //< Keyboard MIDI out channel.
      drums_channel_,              //< Drums MIDI out channel.
      ticks_per_beat_,      //< Number of MIDI ticks per quarter note beat.
      tick_count_,          //< Current index of the quarter note subdivisions.
      tempo_,                      //< MIDI tempo.
      state_swap_flip_;            //< Represents if the controls are swapped
  //_::Array<MIDIDevice> devices;  //< array of all of the MIDI devices currently
  // attached to this system. ISC numMIDIInDev,  //< Number of MIDI
  // input devices attached to the system.
  //     numMIDIOutDev;            //< Number of MIDI output devices attached to
  //     the system.
  IUC double_press_ticks_;         //< TArray of the FPD click time.

  /* Switches the mode or current page on Layer A and Layer B.
  @contract state_swap_flip_ must be true */
  void HoldSwap() {
    if (!state_swap_flip_) return;
  }
};
}  //< namespace _
#endif
#endif
