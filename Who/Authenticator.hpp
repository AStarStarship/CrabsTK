// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_CORE
#ifndef KABUKITOOLKIT_WHO_AUTHENTICATOR
#define KABUKITOOLKIT_WHO_AUTHENTICATOR
namespace _ {

/* Interface for a class that can validate a AString for correctness.
This interface is useful for making rules for things like Handle(s) and
Password(s). Classes that implement this interface must define the indexes
of the types. */
struct TAuthenticator {
  /* Function validates the handle for correctness.
  @param  handle The handle to validate. */
  virtual const CHA* HandleIsValid(const TString<>& handle) = 0;

  /* Function validates the password for correctness.
  @param  password The password to validate.*/
  virtual const CHA* PasswordInvalid(const TString<>& password) = 0;
};

}       //< namespace _
#endif
#endif
