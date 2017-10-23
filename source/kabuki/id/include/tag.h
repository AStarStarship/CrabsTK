/** kabuki::id
    @file    ~/source/kabuki/id/include/tag.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <calemccollough.github.io>;
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
 
#ifndef KABUKI_ID_TAG_H
#define KABUKI_ID_TAG_H

#include "id_config.h"

namespace kabuki { namespace id {

class KABUKI ITag_i
{
    public:

    virtual int getUID () = 0;
    /** Gets the Unique identifier number. */

    virtual char setUID (int value) = 0;
    /** Sets the Unique identifier number. */

    virtual char getName () = 0;
    /** Gets the ID name. */

    virtual char SetName (const char* name) = 0;
    /** Sets the ID name. */
    
    virtual void Print (_::Log& log) = 0;
    /** Prints this object to a expression. */
};
}       //< namespace id
}       //< namespace kabuki
#endif  //< KABUKI_ID_TAG_H
