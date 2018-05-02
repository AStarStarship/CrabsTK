/** kabuki::tek
#include <tek_led_matrix.h>
    @file    ~/source/kabuki/tek/impl/tek_rotary_encoder_bank.cc
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

#include "rotary_knob_bank.h"

using namespace _;

namespace kabuki { namespace tek {

RotaryKnobBank::RotaryKnobBank (ch_t num_knobs, ch_t start_channel,
                                ch_t num_banks, const byte* rows,
                                const byte* columns) :
    num_channels_ (num_knobs),
    start_channel_ (start_channel),
    num_banks_ (num_banks),
    num_buttons_ (0),
    current_bank_ (0),
    current_channel_ (start_channel),
    wiring_config_ (0),
    rows_ (rows),
    columns_ (columns),
    buttons_ (0),
    lights_ (0) {
}

ch_t RotaryKnobBank::GetNumChannels () {
    return num_channels_ * num_banks_;
}

void RotaryKnobBank::SwitchBank (ch_t value) {
    if (value >= num_banks_) return;

    current_bank_ = value;
    current_channel_ = start_channel_ + num_channels_ * value;
}

void RotaryKnobBank::CycleBank () {
    current_channel_ += num_channels_;
    if (current_channel_ > start_channel_ + num_channels_ * num_banks_)
        current_channel_ = start_channel_;
}

void RotaryKnobBank::Update (int16_t* channels_,
                                       int16_t* min_values_,
                                       int16_t* max_values_,
                                       byte* spi_out_bytes_,
                                       int16_t row_,
                                       int8_t count_) {
    //for  (uint16_t i = 0; i < num_channels_; ++i)
    //   UpdateChannel  (i, channels, minValues, maxValues, spiOutBytes, row,
    //                   count, wiring_config_);
}

void RotaryKnobBank::UpdateChannel (uint16_t index,
                                   int16_t* channels,
                                   int16_t* min_values,
                                   int16_t* max_values,
                                   byte* spi_out_bytes,
                                   uint16_t row,
                                   byte count) {
    if (channels == nullptr)
        return;
    if (min_values == nullptr)
        return;
    if (max_values == nullptr)
        return;
    // When we enter this algorithm, we will be stepping through the rows 1-8.

    static const int8_t kLedIndexToRowMap[3][16] = {
        { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, -1 },
        //< LEDRingConfig::NoButtons
        { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0, 7 },
        //< LEDRingConfig::ButtonsOnTopRows
        { 0, 1, 2, 3, 4, 5, 6, 7, 7, 6, 5, 4, 3, 2, 1, 0 } };
        //< LEDRingConfig::ButtonsOnBottomRows

    int16_t value      = channels[index],
            min_value  = min_values[index],
            max_value  = max_values[index],
            ring_index = (value - min_value) % ((max_value - min_value) / 15),
            row_index = kLedIndexToRowMap[wiring_config_][ring_index];

    if (row != row_index)
        return;

    //uint16_t columnNumber =  (index - current_channel_) >> 2;
    //< >> 2 to divide by 2

    //byte columnBit = columns[columnNumber],
    //    mask = 1 <<  (columnBit & BitNumberMask);

}

const Operation* RotaryKnobBank::Star (char_t index, Expression* expr) {
    static const Operation This = {
        "RotaryEncoderBank", NumOperations (0), FirstOperation ('a'),
        "tek", 0 };

    switch (index) {
        case '?': return &This;
    }

    return nullptr;
}

}       //< namespace tek
}       //< namespace kabuki
