/** Kabuki Theater
    @file    /.../Source/_Theater/Mixer/MixerChannel.hpp
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

#include <KabukiTheater-Config.hpp>

namespace _Theater { namespace Mixer {

class _KabukiTheater_ MixerChannel
{
    _Noncopyable_ (MixerChannel)

    public:

    enum { 
        DefaultNumAuxSends = 3,       //< The default number of aux sends.
    };

    MixerChannel  (const char* initName = nullptr);
    /*< Constructs a mixer channel with the given name. */

    ~MixerChannel ();
    /*< Destructor. */

    int getVolume ();
    /*< Gets th volume of this channel. */

    int getPan ();
    /*< Gets th pan of of the channel. */

    int isMuted ();
    /*< Gets if this channel is muted. */

    int isSoloed ();
    /*< Gets if this channel is soloed. */

    void setVolume  (int value);
    /*< Sets the channel volume to the value. */

    void setPan  (int value);
    /*< Sets the channel pan to the value. */

    void setMute  (bool isMuted);
    /*< Either mutes or unmutes this channel. */

    void setSolo  (bool isSoloed);
    /*< Either solos or unsolos this channel. */

    void toggleMute ();
    /*< Toggles the mute variable. */

    void toggleSolo ();
    /*< Toggles the solo variable. */

    int  getAux  (int thisAux);
    /*< Gets thisAux level. */

    void setAux  (int thisAuxSend, int level);
    /*< Sets thisAuxSend level. */

    void deleteAux  (int thisIndex);
    /*< Deletes the aux send channel at thisIndex.
        @warning This function does not trim the auxSend array size for speed 
        considerations Inserts an aux send at thisIndex. */

    private:

    const char* labelText;          //< The name of this channel.

    int volValue,                   //< The volume value for this channel.
        panValue,                   //< The pan value for this channel.
        muteValue,                  //< The mute value for this channel.
        soloValue,                  //< The solo value for this channel.
        numAuxSends;                //< The number of aux sends.

    int* auxSend;                   //< An array of aux send levels.
};

}   //< namespace Mixer
}   //< namespace _Theater
