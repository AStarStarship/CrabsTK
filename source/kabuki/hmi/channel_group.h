/** Kabuki Starship
    @file    ~/Source/_hmi/ChannelGroup.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved  (R).

        Licensed under the Apache License, Version 2.0  (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/
 
#pragma once

#include <KabukiTheater-Config.h>

namespace _hmi {
    
class _HMI_API ChannelGroup
/*< A group of */
{
    public:
    
    ChannelGroup ();
    /*< Constructs a group of channels. */

    const byte* getChannelAddress (uint_t channel);
    /*< Gets the I2P address of the channel. */

    void setChannelAddress (uint_t channel, const byte* address);
    /*< Sets the channel address I2P device. */

    uint_t addChannel (const byte* address);
    /*< Adds a new channel to the group. */

    bool removeChannel (uint_t index);
    /*< Removes the channel at the given index if it exists.
        @return Returns false if the index is out of bounds. */

    void print () const;
    /*< Prints this object to a stdout. */

    private:

    std::vector<const byte*> channels;      //< The list of I2P address for the channels.
};

}   //< _hmi
