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

#include "../include/flying_fader.h"

namespace kabuki { namespace tek {

FlyingFader:: FlyingFader ()
{
}

const _::Operation* FlyingFader::Star (char_t index, _::Expression* expr)
{
    static const _::Operation this_op = { "FlyingFader",
        _::NumMembers (0), _::FirstOperation ('A'),
        "tek::sensuators", 0 };
    switch (index)
    {
        case 0: return _::NumOperations (0);
    }
    
}

}       //< namespace tek
}       //< namespace kabuki
