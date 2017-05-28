/** Kabuki Theater
    @file    /.../Source/_HMI/Controller.hpp
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

#include "HMIComponent.hpp"

namespace _HMI {

class _KabukiTheater_ Controller
/* A group of HMI Control objects.
*/
{
    public:

    enum
    {
        MaxTemplates = 1024,            //< The max amount of templates (Should there be one?).
        MaxTempo = 999,                 //< The max allowed tempo
        MinTicksPerBeat,                //< The min number of ticks per MIDI beat.
        MaxTicksPerBeat,                //< The max number of ticks per MIDI beat.
        MinNumModes = 2,                //< The min number of modes on a ISymmetric HUI.
        MinTicksPerBeat = 24,           //< The min number of MIDI quarter note subdivisions supported.
        MaxTicksPerBeat = 960,          //< The max number of MIDI quarter note subdivisions supported.

        MinTempo = 1,                   //< The min tempo allowed.
        MaxTempo = 960,                 //< The max tempo allowed.
        DefaultEncoderAcceleration = 1  //< The default encoder acceleration.
    };

    // Contrustor.
    Controller  (int initCommonChannel = 0, int initKeyboardChannel = 1, int initDrumsChannel = 7);
    
    Controller  (const Controller& o);
    //< Copy constructor.
    
    ~Controller ();
    //< Destructor.

    void updateMIDIClock ();
    //< Processes a MIDI beat sub-division.
    
    void processMIDIQuaterNote ();
    //< Processes a MIDI quarter note beat.

    int getTicksPerBeat ();
    //< gets the number of ticks per MIDI quarter note beat.
    
    void setTicksPerBeat  (int newNumTicksPerBeat);
    //< sets the ticksPerBeat to the newNumTicks.

    int getTempo ();
    //< gets the current tempo.
    
    void setTempo  (double newTempo);
    //< sets the tempo to the newTempo.
        
    void start ();
    //< Starts the MIDI engine.

    void stop ();
    //< Stops  the MIDI engine.
    
    void forward ();
    //< Processes a fast-forward button press.
    
    void backward ();
    //< Processes a rewind button press.

    double getDoublePressTime ();
    //< gets the double press time in seconds.
    
    int setDoublePressTime  (double newTime);
    /*< sets the double_press_time to the newTime in seconds.
        @return gets -1 if the newTime is to small, 1 if it is to 
            big, and 0 upon success */
    
    void print () const;
    /*< Prints this object to a stdout. */

    private:

    int commonChannel,          //< The common MIDI Channel  (0)
        keyboardChannel,        //< The keyboard MIDI out channel
        drumsChannel,           //< The drums MIDI out channel
        doublePressTime;        //< The max time period that constitutes a "double press", or "double click".
};

}   //< _HMI
}   //< _Theater
