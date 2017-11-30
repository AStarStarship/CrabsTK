/** Kabuki Toolkit
    @file    ~/source/kabuki/hmi/parameter.h
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
 
#ifndef HEADER_FOR_KABUKI_HMI_PARAMETER
#define HEADER_FOR_KABUKI_HMI_PARAMETER

#include "hmi_component.h"

namespace kabuki { namespace hmi {

/** A parameter with upper and lower bounds mapped to an Script device.
      
*/
template<typename T>
class KABUKI Parameter : public HmiComponent {
    public:

    typedef enum {
        MidiControl = 0,
    };

    /** Default constructor. */
    Parameter (T newType, const char* label = "", int channel = 0, 
               T init_value = 0, T min_value = 0, T max_value_ = 1);

    /** Copy constructor. */
    Parameter (const Parameter& o);

    /** Virtual destructor. */
    virtual ~Parameter () {}

    /** Gets what type of control this is. */
    T GetType () const;

    /** Gets a string that says what type of Parameter this is. */
    const char* GetTypeString () const;

    /** Gets the output channel of this control. */
    int GetChannel () const;

    /** Gets the number of channels. */
    int GetNumChannels () const;

    /** Gets the number of bits of in a control word. */
    T GetWordSize () const;

    /** Gets the minimum word size. */
    T GetMinWordSize () const;

    /** Gets the minimum integer value of a word. */
    T GetMinWordValue () const;

    /** Gets the maximum word size. */
    T GetMaxWordSize () const;

    /** Gets the maximum integer value of a word. */
    T getMaxWordValue () const;

    /** Gets the current value of this control. */
    T GetValue () const;

    /** Gets the default MIDI value of this control. */
    T GetInitValue () const;

    /** Gets the min value of this control. */
    T GetMinValue () const;

    /** Gets the max value of this control. */
    T GetMaxValue () const;

    /** Sets the output channel to value. */
    void SetChannel (T value);

    /** Sets the control word size to the new value. */
    void SetWordSize (T value);

    /** Sets this control's value of the parameter to the value. */
    void SetValue (T value);

    /** Sets the initial value of a new control to the value. */
    void SetInitValue (T value);

    /** Sets the min and max values.
        @pre The word size must be set before calling this function! */
    virtual void SetMinMaxValues (T newMin, T newMax);

    /** Sets the min value to the value. */
    virtual void SetMinValue (T value);

    /** Sets the max value to the value. */
    virtual void SetMaxValue (T value);

    /** Toggles between the min and the max values. */
    virtual void Toggle ();

    /** Compares this Parameter to the given Parameter.
        @return gets 0 if both Controls are the same.
        @return gets 1 if the controls are not identical. */
    virtual int Compare (const Parameter& p) const;

    /** Gets the header for toStringRow (). */
    virtual void PrintHeader () const;

    /** Gets a column of the values without the labels. */
    virtual void PrintRow () const;
    
    /** Prints this object to a stdout. */
    void Print () const;

    private:

    const byte* channel_;    //< Output channel.
    T           value,      //< Parameter value.
                init_value_, //< Initial value.
                min_value_,  //< Min value.
                max_value_;  //< Max value.
};

template<typename T, typename U>
class ParameterPair {
    public:

    /** A pair of parameters to create a higher resolution parameter. */
    ParameterPair (T& msb, T& lsb);
    :   msb(msb),
        lsb(lsb)
    {

    }

    /** Sets the LSB value to the value. */
    void SetLsbValue (T value);

    /** Gets the LSB of this value. */
    T GetLsbalue () const;

    /** Gets the MSB of this value. */
    T GetMsbValue () const;

    /** Sets the MSB value to the value. */
    void SetMsbValue (T value);

    private:

    T& msb,
     & lsb;
};      //< ParameterPair
}       //< namespace hmi
}       //< namespace kabuki
#endif  //< HEADER_FOR_KABUKI_HMI_PARAMETER


/*  wordValue,      //< Max value of a word-sized parameter.
    minWordSize,    //< Min size of a control word.
    maxWordSize,    //< Max size of a control word.
    minWordValue,   //< Min value of a parameter.
    maxWordValue,   //< Max value of a parameter.
*/
