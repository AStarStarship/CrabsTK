/** kabuki::tek
    @file    ~/source/kabuki/tek/impl/tek_mixer_page.cc
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

#include "../include/mixer_page.h"
#include "../include/mixer.h"
#include "../include/mixer_page.h"

namespace kabuki { namespace tek {


MixerPage::MixerPage  (Mixer *mixer, int initStartIndex):
    mixer  (mixer)
{
    if  (initStartIndex < 0)  // We currently don't have a way to check the upper bounds of the intiStartIndex.
        initStartIndex = 0;
    else
        startingIndex = initStartIndex;


    channel = new MixerPage *[8];  // Init that shit yo!
    for  (int i=0; i < 8; ++i)
        channel[i] = 0;
}

MixerPage::~MixerPage ()
{
    for  (int i=0; i < 8; ++i)
        delete channel[i];
    delete channel;
    delete mixer;
}

int MixerPage::startIndex ()
{
    return startingIndex;
}

void MixerPage::setStartIndex  (int newIndex)
{
    if  (newIndex < 0)
        return;
    if  (mixer != 0)
        if  (newIndex > mixer->numChannels ())
            return;

    startingIndex = newIndex; //< The channel index starts at 1.
}

const _::Operation* MixerPage::Star (char_t index, _::Expression* expr)
{
    static const _::Operation this_op = { "MixerPage", 
        _::NumOperations (0), _::FirstOperation ('a'),
        "tek::Mixer", 0
    };

    switch (index)
    {
        case '?': return &this_op;
        break;
    }
    
    return invalidIndex;
}

}       //< namespace tek
}       //< namespace kabuki
