/** Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/address.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough.github.io>;
             All right reserved (R). Licensed under the Apache License, Version
             2.0 (the "License"); you may not use this file except in
             compliance with the License. You may obtain a copy of the License
             [here](http://www.apache.org/licenses/LICENSE-2.0). Unless
             required by applicable law or agreed to in writing, software
             distributed under the License is distributed on an "AS IS" BASIS,
             WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
             implied. See the License for the specific language governing
             permissions and limitations under the License.
*/

#pragma once
#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_ADDRESS
#define HEADER_FOR_KT_ID_ADDRESS
#include "config.h"

namespace _ {

typedef enum { HomeAddress = 0, WordAddress, OtherAddress } AddressType;

/** Gets one of the addressTypeLables. */
inline const char* AddressTypeLabel(AddressType type);

/** A physical address.
    @todo Integrate with Google Maps API.
*/
class KABUKI Address {
 public:
  /** Default constructor. */
  Address();

  /** Creates an address with the given parameters. */
  Address(const char* street, const char* aCity, const char* aState,
          const char* aZip, const char* aCcountry, AddressType aType);

  /** Gets the street address. */
  const char* GetStreet();

  /** Sets the street address. */
  void SetStreet(const char* string);

  /** Gets the city. */
  const char* GetCity();

  /** Sets the city. */
  void SetCity(const char* string);

  /** Gets the State. */
  const char* GetState();

  /** Sets the state. */
  void SetState(const char* string);

  /** Gets the zip code. */
  const char* GetZip();

  /** Sets the zip code. */
  void SetZip(const char* string);

  /** Gets the country. */
  const char* GetCountry();

  /** Sets the country. */
  void SetCountry(const char* string);

  /** Gets the address type.
      The address type means street, PO box, etc.
  */
  AddressType GetType();

  /** Sets the address type. */
  void SetType(AddressType t);

  /** Prints this object to a Expression. */
  void Print(_::Log& log);

 private:
  char *street_,      //< The street address.
      *city_,         //< The city of address
      *state_,        //< The state of address
      *zip_,          //< The zip code of address
      *country_;      //< The country of address
  AddressType type_;  //< The type of address
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_ADDRESS
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
