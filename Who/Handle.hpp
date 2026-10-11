// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= CRABSTK_WHO_CORE
#ifndef CRABSTK_WHO_HANDLE_DECL_DECL
#define CRABSTK_WHO_HANDLE_DECL_DECL
#include "Authenticator.hpp"
namespace _ {

/* A handle like a CHA or IUD.
*/
class THandle {
 public:
  enum {
    kValidation = 1,        //< Validation type.
    kDefaultMinLength = 3,  //< Min length of a Handle range.
    kMaxLength = 255        //< Max length of a Handle range.
  };

  static const CHA* Default ();

  /* Constructor creates a standard CHA. */
  THandle(TAuthenticator* authenticator, const TString<>& key,
         ISC min_length = kDefaultMinLength, ISC max_length = kMaxLength)
    : auth_ (authenticator), type_ (kValidation) {
    if (min_length >= max_length) {
      ISC temp = min_length;
      min_length = max_length;
      max_length = temp;
    }
    key = key;
    if (min_length < kDefaultMinLength)
      min_length = kDefaultMinLength;
    else if (min_length > kMaxLength)
      min_length = kDefaultMinLength;
    key_ = StringClone (key);
  }

  /* Gets a reference to the handle AString. */
  const TString<>& GetKey() { return key_; }

  /* Gets true if this password is value. */
  BOL SetKey(const TString<>& key) {
    size_t length = StringLength (key);
    if (length < kDefaultMinLength || length > kMaxLength) return false;

    if (!IsValid (key)) return false;
    delete key_;
    key_ = StringClone (key);
    return true;
  }

  /* Returns true if the handle is valid. */
  BOL IsValid(const TString<>& key) {
    size_t length = StringLength (key);
    if (length < kDefaultMinLength || length > kMaxLength) return false;
    return true;
  }

  /* Returns true if this Handle is identical to the given Handle. */
  BOL Equals(const THandle& h) { return StringEquals (key_, h.key_); }

  /* Returns true if this Handle is identical to the given Handle. */
  BOL Equals(const TString<>& handle) { return StringEquals (key_, handle); }

  /* Prints this object to the log. */
  template<typename Printer> Printer& Print (Printer& o) { return txt << "\nHandle: " << key_; }

 private:
  TString<>& key_;        //< Unique AString key.
  TAuthenticator* auth_;  //< Handle authenticator.
  ISC type_;              //< The validation type.

};  //< class Handle

}   //< namespace _
#endif
#endif
