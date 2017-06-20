/** The Chinese Room
    @version 0.x
    @file    /.../IEvent.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright(C) 2016 [Cale McCollough](calemccollough.github.io)

                        All right reserved(R).

    Licensed under the Apache License, Version 2.0(the "License"); you may
    not use this file except in compliance with the License. You may obtain
    a copy of the License at

                http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef CHINESEROOM_IEVENT_H
#define CHINESEROOM_IEVENT_H

#include "Config.h"

namespace _ {

class IEvent
/*< Interface with an event.
    
    @code
    #include <ChineseRoom/IEvent.h>
    struct Example: public IEvent
    {
        virtual void trigger()
        {
            
        }
    };
    @endcode
*/
{
    public:
    
    virtual void trigger() = 0;
    /*< Triggers the event. */

    virtual void print() const = 0;
    /*< Prints this event to the stdout. */
};

}       //< namespace _

#endif  //< CHINESEROOM_IEVENT_H
