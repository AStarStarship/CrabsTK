// Copyright AStarship <https://astarship.net>.
#pragma once
#include "_Config.h"
#if SEAM >= CRABSTK_WHO_CORE
#ifndef CRABSTK_WHO_ADDRESS_DECL_DECL
#define CRABSTK_WHO_ADDRESS_DECL_DECL
namespace _ {

typedef enum { HomeAddress = 0, WordAddress, OtherAddress } AddressType;

/* A physical address. */
class TAddress {
 public:
  /* Default constructor. */
  TAddress() {
  }

  /* Creates an address with the given parameters. */
  TAddress(const const CHR* street, const const CHR* aCity, const const CHR* aState,
          const const CHR* aZip, const const CHR* aCountry, AddressType aType)
    : street_ (StringClone (street)),
      city_ (StringClone (aCity)),
      state_ (StringClone (aState)),
      zip_ (StringClone (aZip)),
      country_ (StringClone (aCountry)),
      type_ (aType) {}

  /* Gets the street address. */
  const const CHR* GetStreet() { return street_; }

  /* Sets the street address. */
  void SetStreet(const const CHR* AString) {
    delete street_;
    street_ = StringClone (AString);
  }

  /* Gets the city. */
  const const CHR* GetCity() { return city_; }

  /* Sets the city. */
  void SetCity(const const CHR* AString) {
    delete city_;
    city_ = StringClone (AString);
  }

  /* Gets the State. */
  const const CHR* GetState() { return state_; }
  

  /* Sets the state. */
  void SetState(const const CHR* AString) {
    delete state_;
    state_ = StringClone (AString);
  }
  

  /* Gets the zip code. */
  const const CHR* GetZip() { return *zip_; }

  /* Sets the zip code. */
  void SetZip(const const CHR* AString) {
    delete zip_;
    zip_ = StringClone (AString);
  }

  /* Gets the country. */
  const const CHR* GetCountry() { return country_; }

  /* Sets the country. */
  void SetCountry(const const CHR* AString) {
    delete country_;
    country_ = StringClone (AString);
  }

  /* Gets the address type.
      The address type means street, PO box, etc.
  */
  AddressType GetType() { return type_; }

  /* Sets the address type. */
  void SetType(AddressType t) { type_ = t; }

  /* Prints this object to a Expr. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    o << "Address:"
      << "\n Street         : " << street_ << "\n City           : " << city_
      << "\n State/Province : " << state_ << "\n Zip/Postal Code: " << zip_
      << "\n Country        : " << country_ << "\n";
  }

 private:
  TString<> *street_, //< The street address.
      *city_,         //< The city of address
      *state_,        //< The state of address
      *zip_,          //< The zip code of address
      *country_;      //< The country of address
  AddressType type_;  //< The type of address

  static inline const CHA** AddressTypeLabels () {
    static const CHA* labels[] = { "Home", "Work", "Other" };
    return labels;
  }

  static inline const IUA* AddressTypeLabel (AddressType type) {
    if (type < 0 || type >= 3) return AddressTypeLabels ()[0];
    if (type < 0 || type > 2) return AddressTypeLabels ()[2];
    return AddressTypeLabels ()[type];
  }
};

}  //< namespace _
#endif
#endif
