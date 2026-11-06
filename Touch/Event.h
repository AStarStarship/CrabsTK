// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef _CrabsTKTouchEventHPP
#define _CrabsTKTouchEventHPP
#include <_Config.h>
#if _Seam >= CRABSTK_AV_1
namespace CTK {
namespace Touch {

/* A event with an ASCII TSS (Time Subsecond) timestamp. */
class Event {
 public:
  /* Default constructor. */
  Event();

  /* Virtual destructor. */
  virtual ~Event();

  /* gets the timestamp of the Event. */
  TSS GetTimestamp();

  /* Triggers the event. */
  virtual void Trigger() = 0;

  /* Prints this object to a AString. */
  template <typename Printer>
  virtual Printer& Print(Printer& o) const = 0;

 private:
  timestamp_t timestamp_;  //< Event timestamp in microseconds.
};

}  // namespace Touch
}  // namespace CTK
#endif  //< _Seam >= CRABSTK_AV_1
#endif  //< _CrabsTKTouchEventHPP
