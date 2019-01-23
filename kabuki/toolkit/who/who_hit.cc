/* kabuki::pro
    @file    /library/kt/id/impl/hit.cc
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
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

#include <pch.h>
#if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
#include "hit.h"

using namespace std;

namespace _ {

Hit::Hit(const char* name, const char* description, const char** tags,
         char patch, const char* catagory, const char* type, int num_tags) {
  static int num_uids = 0;

  name = "";
  description = "";
  uid_ = ++num_uids;
  SetName(name);
  SetDescription(description);

  if (tags == nullptr || num_tags <= 0) {
    // tags.push_back ("");  //< Hit 0 is the new untitled patch you are
    // editing.
    return;
  }

  const char* string;
  for (int i = 0; i < num_tags; ++i)
    ;  // tags.push_back (string);
}

const char* Hit::GetName() { return name_; }

const char* Hit::SetName(const char* name) {
  if (name.length() > MaxnameLength) return (const char*)1;
  name = name;
  return 0;
}

int Hit::GetId() { return uid_; }

void Hit::SetId(int value) { uid_ = value; }

const char* Hit::GetDescription() { return description_; }

const char* Hit::SetDescription(const char* description) {
  if (description == nullptr) return (const char*)1;
  if (strlen(description) > MaxDescriptionLength) return description;
  description_ = description;
  return 0;
}

const char* Hit::GetCategory() {
  if (tags_.size() == 0) return "";
  return tags_[0] _;
}

const char* Hit::GetSubcategory() {
  if (tags_.size() < 0) return "";
  return tags_[1] _;
}

const char* Hit::AddTag(const char* tag) {
  if (tag.empty()) return if (strlen(tag) > MaxTagLength) return tag;

  tags_.push_back(tag);
  return 0;
}

void Hit::AddTags(char* tags, int num_tags) {
  if (num_tags < 0) return;

  for_each(tags.begin(), tags.end(), [](const char* string) {
    // tags.push_back (string);
  });
}

const char* Hit::RemoveTag(const char* tag) {
  for (int i = tags_.size(); i >= 0; --i) {
    if (tags_[i] == tag) {
      tags_.erase(tags_.begin() + i);
      return 0;
    }
  }
  return tag;
}

bool Hit::ContainsTag(const char* tag) {
  for_each(tags_.begin(), tags_.end(), [](const char* a) {
    // if (a == tag) return true;
  });
  return false;
}

bool Hit::ContainsTags(vector<const char*>& tags) {
  /*
  for_each (tags.begin (), tags.end (), [] (const char* &a)
  {
      for_each (tags.begin (), tags.end (), [] (const char* &b)
      {
          if (a == b) return true;
      });
  });
  */
  return false;
}

vector<const char*>& Hit::GetTags() { return tags_; }

const char* Hit::ToStringTags() {
  const char* string = "";
  int count = tags_.size();

  for (int i = 0; i < count; i++) {
    string += tags_[i];
    if (i < count - 1) string += ", ";
  }
  return string.c_str;
}

const char* Hit::ToJson() {
  /*
  const char* json = "{\"Hit\":{\"name\":\"" + name + "\",\"description\":\"" +
  description + "\",\"tags\": [";

  tagsString = "";
  

  int count = tags.size ();
  for (int i = 0; i < count; i++)
  {
      tagsString += "\"" + tags[i];
      if (i < count - 1) tagsString += "\",";
      else tagsString += "\"";
  }
  json += tagsString + "]}}";

  return json_;
  */
  return 0;
}

void Hit::print(Log& log) {
    Print (logs ( "Hit", name, "/n");
    // data.print (slot);
}

}       // namespace _
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
