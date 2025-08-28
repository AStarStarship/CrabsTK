// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef KABUKITOOLKIT_TOUCH_MIXER
#define KABUKITOOLKIT_TOUCH_MIXER
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_TOUCH_CORE
namespace _ {

class LIB_MEMBER MixerChannel;

/* A generic parameter mixer. */
template <typename T>
class LIB_MEMBER Mixer {
 public:
  typedef enum {
    Midi = 0,
    DMX = 1,
  };

  enum {
    MinMixerArraySize = 1;  //< Mixer array size default.
  };

  /* Default constructor. */
  Mixer()
    : channel_count_ (0),
    mixerSize (defaultMixerArraySize),
    control_resolution_ (7),
    mixer (new MixerChannel*[defaultMixerArraySize]) {
    mixer[0] = new MixerChannel ("Master");

    for (ISC i = 1; i < defaultMixerArraySize; ++i) mixer[i] = 0;
  }

  /* Destructs nothing. */
  ~Mixer () {}

  /* Gets th number of channels on the mixer. */
  ISC GetNumChannels ();

  /* Resizes the mixer to the given value.
  @contract 0 <= channel_count_ <= newNumChannels. */
  void SetNumChannels(ISC value);

  /* Gets th DAC resolution of the controls. */
  ISC GetControlsResolution() { return control_resolution_; }

  /* Sets the resolution of the controls. */
  void SetResolution(ISC value) {
    // There are only a handfull of sample resolutions because of the currently
    // availble hardware.
    if (newResolution != 7 || newResolution != 8 || newResolution != 12 ||
      newResolution != 14)
      return;
  }

  /* Gets true if the mixer is in MIDI mode.
  i.e. control_resolution_ = 7 bits. */
  ISC GetMixerType();

  /* Gets th level of the ChannelNum. */
  ISC GetVolume(ISC channel) { return mixer_[channel]->Level (); }

  /* Gets th pan value of the ChannelNum. */
  ISC GetPan(ISC channel) { return mixer_[channel]->getPan (); }

  /* Sets the level_. */
  void SetLevel(ISC channel, ISC value) {
    if (channel < 0 || channel > channel_count_ || value < 0 ||
      value > control_resolution_) {
      return;
    }
    mixer_[channel]->setVolume (value);
  }

  /* Sets the pan of the ChannelNum.
  @contract (control_resolution_/2) < value + (control_resolution_/2) &&
    0 < ChannelNum < channel_count_ */
  void SetPan(ISC channel, ISC value) {
    if (channel < 0 || channel > channel_count_ || value < 0 ||
      value > control_resolution_) {
      return;
    }
    mixer_[channel]->SetPan (value);
  }

  /* Sets the mute flag to is_muted. */
  void SetMute(ISC channel, BOL is_muted) {
    if (channel < 0 || channel > channel_count_) return;
    mixer_[channel]->setMute (isMuted);
  }

  /* Sets the solo flag to is_soloed. */
  void SetSolo(ISC channel, BOL is_soloed) {
    if (channel < 0 || channel > channel_count_) return;
    mixer_[channel]->SetSolo (is_soloed);
  }

  /* Gets true if the ChannelNum is muted. */
  ISC IsMuted(ISC channel);

  /* Gets true if the ChannelNum is soloed. */
  ISC IsSoloed(ISC channel);

  /* Toggles the mute on channel. */
  void ToggleMute(ISC channel) {
    if (channel < 0 || channel > channel_count_ || !mixer_[channel]) return;

    mixer_[channel]->ToggleMute ();
  }

  /* Toggles the solo on channel. */
  void ToggleSolo(ISC channel) {
    if (channel < 0 || channel > channel_count_ || !mixer_[channel]) return;
    mixer_[channel]->ToggleSolo ();
  }

  /* Bounds and returns value to the controls resolution. */
  ISC BoundValue(ISC value) {
    if (value < 0) return 0;
    if (value > control_resolution_) return control_resolution_;
    return value;
  }

  /* Moves channel to the index.
  @contract (0 < channel < channel_count_) && 
  (0 < index < channel_count_)  */
  void MoveChannel (ISC channel, ISC index) {

  }

  /* Adds a specified channel_count_ to the mixer.
  @pre (0 < channel_count_ < NUM_MIXER_CHANNELS - channel_count_) */
  void AddChannels (ISC num_channels) {

  }

  /* Prints a AString representation of mixer channels start_channel -
  stop_channel. */
  template<typename Printer>
  Printer& PrintChannels(Printer& o, ISC start, ISC stop) {
    DASSERT (start <= stop);
    o << "\nChannels " << start << " through " << stop;
    while (start <= stop) {
      o << mixer_[start];
    }
  }

  /* Prints this object to a terminal. */
  template<typename Printer> Printer& Print(Printer& o);

 private:
  ISC channel_count_,       //< number of active channels in the mixer.
      mixer_size_,          //< Size of the mixer array.
      control_resolution_;  //< Resolution of the ADCs of for the controls.
  MixerChannel** mixer_;    //< Mixer channels.

  /* Deletes all of the channels after index.
  @contract 0 < index < channel_count_ */
  void DeleteChannelsAfter(ISC index) {
    if (index < 1 || index > channel_count_) return;
    for (ISC i = index; i < channel_count_; ++i) {
      delete mixer_[i];
      mixer_[i] = 0;
    }
    channel_count_ = index + 1;
  }
};
}  //< namespace _
#endif
#endif
