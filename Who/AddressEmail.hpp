// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
#ifndef _CrabsTKWhoAddressEMailHPP
#define _CrabsTKWhoAddressEMailHPP
namespace CTK {
namespace Who {

/* An email address. */
class TAddressEmail {
 public:
  /* Default constructor. */
  TAddressEmail(const CHR* address) {}

  /* Gets the address String. */
  const CHR* GetAddress() { return address_; }

  /* Attempts to set the address to the given AString. */
  void SetAddress(const CHR* AString) {
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
}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoAddressEMailHPP
