// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if _Seam >= _CrabsTKWhoCore
#ifndef _CrabsTKWhoAuthenticatorHH
#define _CrabsTKWhoAuthenticatorHH
namespace CTK {
namespace Who {

/* Interface for a class that can validate a CHA* string for correctness.
This interface is useful for making rules for things like Handle(s) and
Password(s). Classes that implement this interface must define the indexes
of the types.
Strings are plain nil-terminated CHA buffers (no C++ std library). */
struct TAuthenticator {
  /* Function validates the handle for correctness.
  @param  handle The handle to validate. */
  virtual const CHA* HandleIsValid(const CHA* handle) = 0;

  /* Function validates the password for correctness.
  @param  password The password to validate.*/
  virtual const CHA* PasswordInvalid(const CHA* password) = 0;
};

}  // namespace Who
}  // namespace CTK
#endif  //< _Seam >= _CrabsTKWhoCore
#endif  //< _CrabsTKWhoAuthenticatorHH
