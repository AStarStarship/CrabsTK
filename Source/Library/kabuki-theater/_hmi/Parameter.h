/** Kabuki Theater
    @file    /.../Source/_HMI/Parameter.hpp
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
 
#pragma once

#include "HMIComponent.hpp"

namespace _HMI {

template<typename T>
class _HMI_Devices_ Parameter : public HMIComponent
/** A parameter with upper and lower bounds mapped to an I2P device.
      
*/
{
    public:

    Parameter (T newType, const char* label = "", int channel = 0, T initValue = 0, T minValue = 0,
        T maxValue = 1);
    /*< Default constructor. */

    Parameter (const Parameter& o);
    /*< Copy contructor. */

    virtual ~Parameter () {}
    /*< Virtual destructor. */

    T getType () const;
    /*< Gets what type of control this is. */

    const char* getTypeString () const;
    /*< Gets a string that says what type of Parameter this is. */

    int getChannel () const;
    /*< Gets the output channel of this control. */

    int getNumChannels () const;
    /*< Gets the number of channels. */

    T getWordSize () const;
    /*< Gets the number of bits of in a control word. */

    T getMinWordSize () const;
    /*< Gets the minimum word size. */

    T getMinWordValue () const;
    /*< Gets the minimum integer value of a word. */

    T getMaxWordSize () const;
    /*< Gets the maximum word size. */

    T getMaxWordValue () const;
    /*< Gets the maximum integer value of a word. */

    T getValue () const;
    /*< Gets the current value of this control. */

    T getInitValue () const;
    /*< Gets the default MIDI value of this control. */

    T getMinValue () const;
    /*< Gets the min value of this control. */

    T getMaxValue () const;
    /*< Gets the max value of this control. */

    void setChannel (T value);
    /*< Sets the output channel to value. */

    void setWordSize (T value);
    /*< Sets the control word size to the new value. */

    void setValue (T value);
    /*< Sets this control's value of the parameter to the value. */

    void setInitValue (T value);
    /*< Sets the initial value of a new control to the value. */

    virtual void setMinMaxValues (T newMin, T newMax);
    /*< Sets the min and max values.
    @pre    The word size must be set before calling this function! */

    virtual void setMinValue (T value);
    /*< Sets the min value to the value. */

    virtual void setMaxValue (T value);
    /*< Sets the max value to the value. */

    virtual void toggle ();
    /*< Toggles between the min and the max values. */

    virtual int compare (const Parameter& p) const;
    /*< Compares this Parameter to the given Parameter.
    @return gets 0 if both Controls are the same.
    @return gets 1 if the controls are not identical. */

    virtual void printHeader () const;
    /*< Gets the header for toStringRow (). */

    virtual void printRow () const;
    /*< Gets a column of the values without the labels. */
    
    void print () const;
    /*< Prints this object to a stdout. */

    private:

    const byte* channel;    //< The output channel.
    T value,                //< The parameter value.
        initValue,          //< The initial value.
        minValue,           //< The min value.
        maxValue;           //< The max value.
};

template<typename T, typename U>
class ParameterPair
{
    public:

    ParameterPair (T& msb, T& lsb);
    /*< A pair of parameters to create a higher resolution parameter. */
    :   msb(msb),
        lsb(lsb)
    {

    }

    void setLSBValue (T value);
    /*< Sets the LSB value to the value. */

    T getLSBValue () const;
    /*< Gets the LSB of this value. */

    T getMSBValue () const;
    /*< Gets the MSB of this value. */

    void setMSBValue (T value);
    /*< Sets the MSB value to the value. */

    private:

    T& msb,
        &lsb;
};

}   //< _HMI

/*  wordValue,      //< The maximum value of a word-sized parameter.
    minWordSize,    //< The min size of a control word.
    maxWordSize,    //< The max size of a control word.
    minWordValue,   //< The min value of a parameter.
    maxWordValue,   //< The max value of a parameter.
*/
