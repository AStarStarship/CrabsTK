/* Kabuki Toolkit
    @file    /library/kt/id/server.h
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
#ifndef HEADER_FOR_KT_ID_UIDSERVER
#define HEADER_FOR_KT_ID_UIDSERVER
#include "config.h"

namespace _ {

/* A uid server that can save and load the uid from a file.
    Unique ids are indexed using only positive integers. 0 is hence used to
    flag when the server doesn't contain a uid. */
template <typename T = uid_t>
class UidServer {
 public:
  enum { kInvalidUid = 0 };

  /* Constructs object by setting next_id_ to init_uid and copying the
      filename string. */
  UidServer(T init_uid = 1, const char* filename = "uid")
      : next_uid_((init_uid == kInvalidUid) ? kInvalidUid + 1 : init_uid),
        filename_(StrandClone(filename)) {
    //                      +---E  +----<||
    // Nothing to do here ({:->)-|------|
    //                           +---E  +----<||
  }

  /* Peeks at the next uid with incrementing it. */
  T PeekNextUid() { return next_uid_; }

  /* Gets the next uid. */
  T GetNextUid() {
    if (next_uid_ == kInvalidUid) return kInvalidUid + 1;
    return next_uid_++;
  }

  const char* Save() {
    ofstream file;
    file.open(filename);
    file << uid_;
    file.close();
  }

  const char* Load() {
    ifstream file(fileanme);
    if (file.is_open()) {
      file << while (getline(file, line)) { cout << line << '\n'; }
      file.close();
    } else {
      cout << "Unable to open file";
    }

    return 0;
  }

 private:
  T next_uid_;            //< Next unique id.
  const char* filename_;  //< Filename to write uid too.
};                        //< class UidServer
}       // namespace _
#endif  //< HEADER_FOR_KT_ID_UIDSERVER
#endif  //< #if MAJOR_SEAM > 2 || MAJOR_SEAM == 2 && MINOR_SEAM >= 2
