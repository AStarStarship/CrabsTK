/* Kabuki Toolkit @version 0.x
@link    https://github.com/kabuki-starship/kabuki-toolkit.git
@file    /kabuki/toolkit/who/t_entity.h
@author  Cale McCollough <calemccollough.github.io>
@license Copyright (C) 2014-2019 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_WHO_1
#ifndef KABUKI_TOOLKIT_WHO_ENTITY
#define KABUKI_TOOLKIT_WHO_ENTITY

#include "t_profile.h"

namespace _ {

/* A entity with contact information.
@todo Attach an image URL.
*/
class TEntity {
 public:
  /* A real-world entity. */
  TEntity() {}

  /* Gets the entity first name. */
  const TStrand<>& GetFirstName() { return first_name_; }

  /* Attempts to set the name to the one given string. */
  void SetFirstName(const TStrand<>& string) {
    delete first_name_;
    first_name_ = StrandClone (string);
  }

  /* Gets the entity last name. */
  const TStrand<>& GetLastName() { return last_name_; }

  /* Attempts to set the last name to the given string. */
  void SetLastName(const TStrand<>& string) {
    delete last_name_;
    last_name_ = StrandClone (string);
  }

  TStrand<>

  /* Returns true if this entity contains the search query. */
  BOL Query(const TStrand<>& query) {
    /*
    for_each (tags.begin (), tags.end (), [](CH1& string(
    {
        if (string == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](Address& a(
    {
        if (a == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](Entity& e(
    {
        if (e == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](Profile& p(
    {
        if (p == query) return 1;
    });

    for_each (tags.begin (), tags.end (), [](CH1& string(
    {
        if (string == query) return 1;
    });
    */
    return false;
  }

  /* Prints this object to a expression. */
  template<typename Printer>
  Printer& Print (Printer& o) {
    out << "Entity: ";
  }

 private:

  TStrand<>first_name_,  //< The entity's first name.
      last_name_,        //< Entity's last name or class.
      phone_number_,     //< Phone number of the entity.
      image_uri_;         //< A URI pointing to an image of the Entity.
  TLoom<> addresses_,     //< Address of the entity.
      email_addresses_,   //< Email address of the entity.
      tags_,              //< Metadata tags.
      profiles_,          //< Online profiles this entity is attached to.
      notes_;             //< An array of notes about the entity.
};

}       // namespace _
#endif  //< KABUKI_TOOLKIT_WHO_ENTITY
#endif  //< #if SEAM >= KABUKI_TOOLKIT_WHO_1
