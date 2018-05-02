/** Kabuki Toolkit
    @file    ~/source/mixer_channel.h
    @author  Cale McCollough <calemccollough.github.io>
    @license Copyright (C) 2014-2017 Cale McCollough <calemccollough@gmail.com>;
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
#include <stdafx.h>
#if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KABUKI_TOOLKIT_HMI_MIXERCHANNEL
#define HEADER_FOR_KABUKI_TOOLKIT_HMI_MIXERCHANNEL

#include "config.h"

namespace kt { namespace hmi {

/** A mixer channel Operation. */
class KABUKI MixerChannel : public _::Op {
    NONCOPYABLE (MixerChannel)
    public:

    enum { 
        DefaultNumAuxSends = 3, //< Default number of aux sends.
    };

    /** Constructs a mixer channel with the given name. */
    MixerChannel  (const char* initName = nullptr);

    /** Destructor. */
    ~MixerChannel ();

    /** Gets th volume of this channel. */
    int GetVolume ();

    /** Gets th pan of of the channel. */
    int GetPan ();

    /** Gets if this channel is muted. */
    int IsMuted ();

    /** Gets if this channel is soloed. */
    int IsSoloed ();

    /** Sets the channel volume to the value. */
    void setVolume  (int value);

    /** Sets the channel pan to the value. */
    void SetPan  (int value);

    /** Either mutes or unmutes this channel. */
    void SetMute  (bool isMuted);

    /** Either solos or unsolos this channel. */
    void SetSolo  (bool isSoloed);

    /** Toggles the mute variable. */
    void ToggleMute ();

    /** Toggles the solo variable. */
    void ToggleSolo ();

    /** Gets thisAux level. */
    int  GetAux  (int thisAux);

    /** Sets thisAuxSend level. */
    void SetAux  (int thisAuxSend, int level);

    /** Deletes the aux send channel at thisIndex.
        @warning This function does not trim the auxSend array size for speed 
        considerations Inserts an aux send at thisIndex. */
    void DeleteAux  (int thisIndex);
    
    /** Script Operations. */
    const _::Op Star (uint index, _::Expression expr);

    private:

    const char* label_text_;    //< Name of this channel.
    int         vol_value_,     //< Volume value for this channel.
                pan_value_,     //< Pan value for this channel.
                mute_value_,    //< Mute value for this channel.
                solo_value_,    //< Solo value for this channel.
                num_aux_sends_; //< Number of aux sends.
    int*        aux_send_;      //< Array of aux send levels.
    
};      //< class MixerChannel
}       //< namespace hmi
}       //< namespace toolkit
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_TOOLKIT_HMI_MIXERCHANNEL
#endif  //< #if MAJOR_SEAM >= 2 && MINOR_SEAM >= 1
