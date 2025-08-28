// Copyright AStarship <https://astarship.net>.
#pragma once
#include <_Config.h>
#if SEAM >= KABUKITOOLKIT_WHO_CORE
#ifndef KABUKITOOLKIT_WHO_ADDRESS
#define KABUKITOOLKIT_WHO_ADDRESS
namespace _ {

typedef enum { HomeAddress = 0, WordAddress, OtherAddress } AddressType;

/* A physical address. */
class TAddress {
 public:
  /* Default constructor. */
  TAddress() {
  }

  /* Creates an address with the given parameters. */
  TAddress(const TString<>& street, const TString<>& aCity, const TString<>& aState,
          const TString<>& aZip, const TString<>& aCcountry, AddressType aType)
    : street_ (StringClone (street)),
      city_ (StringClone (city)),
      state_ (StringClone (state)),
      zip_ (StringClone (zip)),
      country_ (StringClone (country)),
      type_ (type) {}

  /* Gets the street address. */
  const TString<>& GetStreet() { return street_; }

  /* Sets the street address. */
  void SetStreet(const TString<>& AString) {
    delete street_;
    street_ = StringClone (AString);
  }

  /* Gets the city. */
  const TString<>& GetCity() { return city_; }

  /* Sets the city. */
  void SetCity(const TString<>& AString) {
    delete city_;
    city_ = StringClone (AString);
  }

  /* Gets the State. */
  const TString<>& GetState() { return state_; }
  

  /* Sets the state. */
  void SetState(const TString<>& AString) {
    delete state_;
    state_ = StringClone (AString);
  }
  

  /* Gets the zip code. */
  const TString<>& GetZip();

  /* Sets the zip code. */
  void SetZip(const TString<>& AString) {
    delete zip_;
    zip_ = StringClone (AString);
  }

  /* Gets the country. */
  const TString<>& GetCountry() { return country_; }

  /* Sets the country. */
  void SetCountry(const TString<>& AString) {
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
    out << "Address:"
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
