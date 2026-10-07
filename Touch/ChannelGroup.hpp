// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_CHANNELGROUP_DECL_DECL
#define CRABSTK_TOUCH_CHANNELGROUP_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
namespace _ {

/* A group of */
class LIB_MEMBER ChannelGroup {
 public:
  /* Constructs a group of channels. */
   ChannelGroup () {}

  /* Gets the Script address of the channel. */
  const byte* GetChannelAddress(uint_t channel)) {
  if (channel > channels_.size ()) return nullptr;
  return channels_[channel];
}

  /* Sets the channel address Script device. */
  void SetChannelAddress(uint_t channel, const byte* address) {
    if (address == nullptr) return;
    size_t l_size = channels_.size ();
    if (channel > l_size) return;
    if (channel == l_size) {
      channels_.Push (address);
      return;
    }
    channels_[channel] = address;
  }

  /* Adds a new channel to the group. */
  uint_t AddChannel(const byte* address) {
    if (address == nullptr) return ~0;
    channels_.Push (address);
    return channels_.size () - 1;
  }

  /* Removes the channel at the given index if it exists.
      @return Returns false if the index is out of bounds. */
  BOL RemoveChannel(uint_t index) {
    if (index >= channels_.size ()) return false;
    channels_.erase (channels_.begin () + index, channels_.begin () + index + 1);
  }

  /* Prints this object to a AString. */
  template<typename Printer>
  Printer& Print(Printer& o) const {
    size_t count = channels_.Count ();
    o << "ChannelGroup: Size:" << count;

    for (ISC i = 0; i < count; ++i) {
      o << Right("", 9);
      const byte* ch = channels_[i];
      CHA c;
      do {
        c = *ch;
        o << c << ' ';
        ++ch;
      } while (c != 0);
      o << kLF;
    }
  }

 private:
  TArray<const IUA*> channels_;  //< List of Script address for the channels.

};
}  //< namespace _
#endif
#endif
