/** Kabuki Starship
    @file    ~/Source/_Theater/Mixer/MixerPage.h
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

namespace _Theater { namespace Mixer {

class _KabukiTheater_ Mixer;
class _KabukiTheater_ MixerChannel;

/*! Class that represents a page of mixer channels on the Symmetry Live!
    A MixerPage stores 8 TemplatePages in an array. The user can switch through banks of pages on the mixer. The 
    mixer can either be sorted inassending track index order or the page banks can be customized to make buses.
*/
class _KabukiTheater_ MixerPage
{
    public:

    MixerPage (Mixer *theMixer = nullptr, int initStartIndex = 0);
    //< Constructor.

    ~MixerPage ();
    //< Destructor.

    int getStartIndex ();
    //< Gets the startingIndex of this MixerPage.

    void setStartIndex (int newIndex);
    //< Sets the startingIndex to the newIndex.

    void setChannel (int thisChannel, MixerChannel* theNewOne);
    //< Replaces thisChannel with theNewOne.

    MixerChannel *getChannel (int index);
    //< Gets a pointer to the channel[atThisIndex].
    
    void print ();
    /*< Prints this object to a terminal. */

    private:

    int startingIndex;                  	//< The startingIndex of this page this MixerPage.
    Mixer *mixer;                       	//< A pointer to the global _Theater::Mixer.
    std::vector<MixerChannel*> channels;  	//< An array of pointers to pointers to MixerChannel  (s).
};
}   //< namespace Mixer
}   //< namespace _Theater

