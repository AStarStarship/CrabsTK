// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABSTK_AV_EVENT_DECL
#define CRABSTK_AV_EVENT_DECL
#include <_Config.h>
#if SEAM >= CRABSTK_AV_1
namespace _ {

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
  timestamp_t timestamp;  //< Event timestamp in microseconds.
};

}  //< namespace _
#endif
#endif
