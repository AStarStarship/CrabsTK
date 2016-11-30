/** Kabuki Software Development Kit
    @file    /.../KabukiSDK/_UI/AVControl.hpp
    @author  Cale McCollough <cale.mccollough@gmail.com>
    @license Copyright  (C) 2016 [Cale McCollough](calemccollough.github.io)

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

    /** A list of the different types of Control  (s). */
    enum Types { 
        DMXControl = 0, 
        MIDIControl, 
        OSCControl, 
        DMXButton, 
        MIDIButton, 
        OSCButton, 
        MacroButton
    } ;

    static const char* MacroHeader;        //< const char* for printing multiple AVControls with other opclasses.
    static const int MacroHeaderLength;     //< The length of the MacroHeader

    AVControl  (int newType, const char* &newLabel = "", int newChannel = 0, int initValue = 0, int newMinValue = 0,
        int newMaxValue = 1, int newWordSize = 1);
    /*< Default constructor. */

    AVControl  (const AVControl& other);
    /*< Copy contructor. */

    virtual ~AVControl () {}
    /*< Virtual destructor. */

    int getType () const;
    /*< gets what type of control this is. */

    const char* getTypeString () const;
    /*< gets a const char* that says what type of AVControl this is. */

    int getChannel () const;
    /*< gets the output channel of this control. */

    int getNumChannels () const;
    /*< gets the number of channels. */

    int getWordSize () const;
    /*< gets the number of bits of in a control word. */

    int getMinWordSize () const;
    /*< gets the minimum word size. */

    int getMinWordValue () const;
    /*< gets the minimum integer value of a word. */

    int getMaxWordSize () const;
    /*< gets the maximum word size. */

    int getMaxWordValue () const;
    /*< gets the maximum integer value of a word. */

    int getValue () const;
    /*< gets the current value of this control. */

    int getLSBValue () const;
    /*< gets the LSB of this value. */

    int getMSBValue () const;
    /*< gets the MSB of this value. */

    int getInitValue () const;
    /*< gets the default MIDI value of this control. */

    int getMinValue () const;
    /*< gets the min value of this control. */

    int getMaxValue () const;
    /*< gets the max value of this control. */
    
    void setLSBValue  (int value);
    /*< sets the LSB value to the value. */

    void setMSBValue  (int value);
    /*< sets the MSB value to the value. */

    void setChannel  (int value);
    /*< sets the output channel to value. */

    void setWordSize  (int value);
    /*< sets the control word size to the new value. */

    void setValue  (int value);
    /*< sets this control's value of the parameter to the value. */

    void setInitValue  (int value);
    /*< sets the initial value of a new control to the value. */

    virtual void setMinMaxValues  (int newMin, int newMax);
    /*< sets the min and max values.
    @pre    The word size must be set before calling this function! */

    virtual void setMinValue  (int value);
    /*< sets the min value to the value. */

    virtual void setMaxValue  (int value);
    /*< sets the max value to the value. */

    virtual void trigger () {}
    /*< Triggers the Control to fire. */

    virtual void toggle ();
    /*< Toggles between the min and the max values. */

    virtual int compare  (const AVControl& other) const;
    /*< Compares this Control to the other.
    @return gets 0 if both Controls are the same.
    @return gets 1 if the controls are not identical. */

    virtual const char* getHeaderString () const;
    /*< gets the header for toStringRow (). */

    virtual const char* ToStringRow () const;
    /*< gets a column of the values without the labels. */

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

    void setType  (int value);
    //< sets the type to the new value. */
};

}   //< namespace _UI
