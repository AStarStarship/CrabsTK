/** kabuki::pro
    @file    $kabuki-toolkit/library/kt/id/id_address.cc
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

#include <stdafx.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "address.h"

namespace _ {

const char** AddressTypeLabels() {
  static const char* labels[] = {"Home", "Work", "Other"};
  return labels;
}

const char* AddressTypeLabel(AddressType type) {
  if (type < 0 || type >= 3) return AddressTypeLabels()[0];
  if (type < 0 || type > 2) return AddressTypeLabels()[2];
  return AddressTypeLabels()[type];
}

Address::Address() {
  /// Nothing to do here! :-)
}

Address::Address(const char* street, const char* city, const char* state,
                 const char* zip, const char* country, AddressType type)
    : street_(_::StringClone(street)),
      city_(_::StringClone(city)),
      state_(_::StringClone(state)),
      zip_(_::StringClone(zip)),
      country_(_::StringClone(country)),
      type_(type) {}

const char* Address::GetStreet() { return street_; }

void Address::SetStreet(const char* string) {
  delete street_;
  street_ = _::StringClone(string);
}

const char* Address::GetCity() { return city_; }

void Address::SetCity(const char* string) {
  delete city_;
  city_ = _::StringClone(string);
}

const char* Address::GetState() { return state_; }

void Address::SetState(const char* string) {
  delete state_;
  state_ = _::StringClone(string);
}

const char* Address::GetZip() { return zip_; }

void Address::SetZip(const char* string) {
  delete zip_;
  zip_ = _::StringClone(string);
}

const char* Address::GetCountry() { return country_; }

void Address::SetCountry(const char* string) {
  delete country_;
  country_ = _::StringClone(string);
}

AddressType Address::GetType() { return type_; }

void Address::SetType(AddressType t) { type_ = t; }

void Address::Print(_::Log& log) {
  log << "Address:"
      << "\n Street         : " << street_ << "\n City           : " << city_
      << "\n State/Province : " << state_ << "\n Zip/Postal Code: " << zip_
      << "\n Country        : " << country_ << "\n";
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
