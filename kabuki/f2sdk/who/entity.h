/* Kabuki Toolkit
    @file    $kabuki-toolkit/library/kt/id/entity.h
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
#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#ifndef HEADER_FOR_KT_ID_ENTITY
#define HEADER_FOR_KT_ID_ENTITY

#include "profile.h"

namespace _ {

/* A entity with contact information.
    @todo Attach a compressed image link.
*/
class API Entity {
 public:
  /* A real-world entity with . */
  Entity();

  /* Gets the entity first name. */
  const char* GetFirstName();

  /* Attempts to set the name to the one given string. */
  void SetFirstName(const char* string);

  /* Gets the entity last name. */
  const char* GetLastName();

  /* Attempts to set the last name to the given string. */
  void SetLastName(const char* string);

  /* Returns true if this entity contains the search query. */
  bool Query(const char* query);

  /* Prints this object to a expression. */
  void Print(Log& log);

 private:
  char *first_name_,             //< The entity's first name.
      *last_name_,               //< Entity's last name or class.
      *phone_number_;            //< Phone number of the entity.
  data::StringArray addresses_,  //< Address of the entity.
      email_addresses_,          //< Email address of the entity.
      tags_,                     //< Metadata tags.
      profiles_,                 //< Online profiles this entity is attached to.
      notes_;                    //< An array of notes about the entity.
};

}       // namespace _
#endif  //< HEADER_FOR_KT_ID_ENTITY
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
