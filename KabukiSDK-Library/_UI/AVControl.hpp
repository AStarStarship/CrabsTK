/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/AVControl.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright (C) 2016 [Cale McCollough](calemccollough.github.io)

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
 
#pragma once

#include <FreeI2P.hpp>
#include <KabukiSDK-Config.hpp>
#include "Label.hpp"
#include "Event.hpp"

namespace _UI {

class _KabukiSDK AVControl, public Label, public Event
/** An audio-visual control.
    
*/
{
    public:

    /** A list of the different types of Control (s). */
    enum Types { 
        DMXControl = 0, 
        MIDIControl, 
        OSCControl, 
        DMXButton, 
        MIDIButton, 
        OSCButton, 
        MacroButton
    };

    static const char* MacroHeader;        //< const char* for printing multiple AVControls with other opclasses.
    static const int MacroHeaderLength;     //< The length of the MacroHeader

    AVControl (int newType, const char* &newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0,
        int newMaxValue = 1, int newWordSize = 1);
    /*< Default constructor. */

    AVControl (const AVControl& other);
    /*< Copy contructor. */

    virtual ~AVControl () {}
    /*< Virtual destructor. */

    int getType () const;
    /*< Gets what type of control this is. */

    const char* getTypeString () const;
    /*< Gets a const char* that says what type of AVControl this is. */

    int getChannel () const;
    /*< Gets the output channel of this control. */

    int getNumChannels () const;
    /*< Gets the number of channels. */

    int getWordSize () const;
    /*< Gets the number of bits of in a control word. */

    int getMinWordSize () const;
    /*< Gets the minimum word size. */

    int getMinWordValue () const;
    /*< Gets the minimum integer value of a word. */

    int getMaxWordSize () const;
    /*< Gets the maximum word size. */

    int getMaxWordValue () const;
    /*< Gets the maximum integer value of a word. */

    int getValue () const;
    /*< Gets the current value of this control. */

    int getLSBValue () const;
    /*< Gets the LSB of this value. */

    int getMSBValue () const;
    /*< Gets the MSB of this value. */

    int getInitValue () const;
    /*< Gets the default MIDI value of this control. */

    int getMinValue () const;
    /*< Gets the min value of this control. */

    int getMaxValue () const;
    /*< Gets the max value of this control. */
    
    void setLSBValue (int value);
    /*< Sets the LSB value to the value. */

    void setMSBValue (int value);
    /*< Sets the MSB value to the value. */

    void setChannel (int value);
    /*< Sets the output channel to value. */

    void setWordSize (int value);
    /*< Sets the control word size to the new value. */

    void setValue (int value);
    /*< Sets this control's value of the parameter to the value. */

    void setInitValue (int value);
    /*< Sets the initial value of a new control to the value. */

    virtual void setMinMaxValues (int newMin, int newMax);
    /*< Sets the min and max values.
    @pre    The word size must be set before calling this function! */

    virtual void setMinValue (int value);
    /*< Sets the min value to the value. */

    virtual void setMaxValue (int value);
    /*< Sets the max value to the value. */

    virtual void trigger () {}
    /*< Triggers the Control to fire. */

    virtual void toggle ();
    /*< Toggles between the min and the max values. */

    virtual int compare (const AVControl& other) const;
    /*< Compares this Control to the other.
    @return gets 0 if both Controls are the same.
    @return gets 1 if the controls are not identical. */

    virtual const char* getHeaderString () const;
    /*< Gets the header for toStringRow (). */

    virtual const char* ToStringRow () const;
    /*< Gets a column of the values without the labels. */

    virtual const char* print (I2P::Terminal& slot) const;
    /*< Prints this object to a terminal. */

    private:
    
    int type,                                      //< The type of Control. @see Types
        channel,                                   //< The output channel.
        numChannels,                               //< The number of channels.
        wordSize,                                  //< The word-size of a parameter value in bits.
        wordValue,                                 //< The maximum value of a word-sized parameter.
        minWordSize,                               //< The min size of a control word.
        maxWordSize,                               //< The max size of a control word.
        minWordValue,                              //< The min value of a parameter.
        maxWordValue,                              //< The max value of a parameter.
        value,                                     //< The parameter value.
        initValue,                                 //< The initial value.
        minValue,                                  //< The min value.
        maxValue;                                  //< The max value.

    void setType (int value);
    //< sets the type to the new value. */
};

}   //< namespace _UI
