// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_TOUCH_MIDIWIDGET_DECL_DECL
#define CRABSTK_TOUCH_MIDIWIDGET_DECL_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_TOUCH_CORE
#include "Widget.hpp"
namespace _ {

class WidgetMidi : public TWidget {
 public:
  enum {
    kChannelCommon = 0,         //< Channel for outputting to all channels.
    kPressDoubleTicksMin = 1,   //< ASCII TSS ticks offset min.
    kPressDoubleTicksMax = 1000 //<  ASCII TSS ticks offset min.
  };

  WidgetMidi(IUA common_channel, IUA keyboard_channel, IUA drums_channel)
    : common_channel_ (common_channel_),
      channel_keyboard_ (channel_keyboard_),
      drums_channel_ (drums_channel_),
      double_press_ticks_ (500) {}

  WidgetMidi(const WidgetMidi& o)
    : common_channel_ (o.common_channel_),
      channel_keyboard_ (o.channel_keyboard_),
      drums_channel_ (o.drums_channel_) {}

  virtual ~WidgetMidi () {}

  IUA GetKeyboardChannel() { return channel_keyboard_; }

  void SetKeyboardChannel(IUA channel) {
    if (channel == 0)
      channel_keyboard_ = 1;
    else if (channel >= 16)
      channel_keyboard_ = 16;
    else
      channel_keyboard_ = channel;
  }

  ISA GetDrumsChannel() { return drums_channel_; }

  void SetDrumsChannel(IUA channel);

  IUC GetDoublePressTime() { return double_press_ticks_; }

  ISC SetDoublePressTime(IUC new_time) {
    if (newTime < minDoubleClickTime) return -1;

    if (newTime > maxDoubleClickTime) return 1;

    double_press_ticks_ = newTime;
    return 0;
  }

  template<typename Printer>
  Printer& Print(Printer& o) {
    o << "\nWidgetMidi:";
  }

private:
  ISA common_channel_,  //< Channel 
    channel_keyboard_,
    drums_channel_;
  IUC double_press_ticks_;
};
}  //< namespace _

#endif
#endif
