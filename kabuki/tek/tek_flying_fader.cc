/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_FlyingFader.cc
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2017 Cale McCollough <calemccollough@gmail.com>;
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

#include "flying_fader.h"

using namespace _ {;
using namespace std;

namespace kabuki { namespace tek {

FlyingFader:: FlyingFader () {
}

void FlyingFader::Print () {
    cout << "\n| FlyingFader:0x";
    PrintPointer (this);

}
const Operation* FlyingFader::Star (char_t index, Expr* expr) {
    static const Operation This = { "FlyingFader",
        NumOperations (0), FirstOperation ('a'),
        "tek", 0 };
    switch (index) {
        case '?': return &This;
    }
    return nullptr;
}

FlyingFaderOp::FlyingFaderOp (FlyingFader* ff) :
    ff_ (ff) {
}

const Operation* FlyingFaderOp::Star (char_t index, Expr* expr) {
    return ff_->Star (index, expr);
}

}       //< namespace tek
}       //< namespace kabuki
