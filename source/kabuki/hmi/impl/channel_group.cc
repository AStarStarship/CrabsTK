/** Kabuki Theater
    @file    /.../Source/_HMI/RadioButton.h
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2017 Cale McCollough <https://calemccollough.github.io>

                            All right reserved (R).

        Licensed under the Apache License, Version 2.0 (the "License"); you may
        not use this file except in compliance with the License. You may obtain
        a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

        Unless required by applicable law or agreed to in writing, software
        distributed under the License is distributed on an "AS IS" BASIS,
        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
        See the License for the specific language governing permissions and
        limitations under the License.
*/

#include <_HMI/ChannelGroup.h>

namespace _HMI {
    
ChannelGroup::ChannelGroup ()
{
}

const byte* ChannelGroup::getChannelAddress (uint_t channel)
{
    if (channel > channels.size ()) return nullptr;
    return channels[channel];
}

void ChannelGroup::setChannelAddress (uint_t channel, const byte* address)
{
    if (address == nullptr) return;
    size_t l_size = channels.size ();
    if (channel > l_size) return;
    if (channel == l_size)
    {
        channels.push_back (address);
        return;
    }
    channels[channel] = address;
}

uint_t ChannelGroup::addChannel (const byte* address)
{
    if (address == nullptr) return ~0;
    channels.push_back (address);
    return channels.size () - 1;
}

bool ChannelGroup::removeChannel (uint_t index)
{
    if (index >= channels.size ()) return false;
    channels.erase (channels.begin () + index, channels.begin () + index + 1);
}
    
void ChannelGroup::print () const
{
    size_t l_size = channels.size ();
    printf ("ChannelGroup: size=%u\n", l_size);

    for (size_t i = 0; i < l_size; ++i)
    {
        printf ("%9u: ");
        const byte* ch = channels[i];
        char c;
        do
        {
            c = *ch;
            printf ("%u ", c);
            ++ch;
        } while (c != 0);
        _::printNL ();
    }
}

}   //< _HMI
