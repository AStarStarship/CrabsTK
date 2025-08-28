// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_CORE
#ifndef KABUKITOOLKIT_WHO_EMAILADDRESS
#define KABUKITOOLKIT_WHO_EMAILADDRESS
namespace _ {
/* An email address. */
class TAddressEmail {
 public:
  /* Default constructor. */
  TAddressEmail(const TString<>& address) {}

  /* Gets the address String. */
  TString<>& GetAddress() { return address_; }

  /* Attempts to set the address to the given AString. */
  void SetAddress(const TString<>& AString) {
    // address_ = AString;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    return o << "Foo";
  }

 private:
  TString<> address_;  //< The email address.
};
}       //< namespace _
#endif
#endif
