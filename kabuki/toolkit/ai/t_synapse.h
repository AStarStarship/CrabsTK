/* Kabuki Toolkit @version 0.x
@file    /kabuki/toolkit/ai/perceptron.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <cale@astartup.net>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>
#if SEAM >= KABUKI_TOOLKIT_AI_1
#ifndef KABUKI_TOOLKIT_AIML_SYNAPSE_T
#define KABUKI_TOOLKIT_AIML_SYNAPSE_T 1

namespace _ {

;
class TPerceptron;

/* A synapse between an axon and dendrite. */
template<typename Value>
class TSynapse {
    public:
      /* TSynapse value min. */

    static Value Min () { return -1.0f; }

    /* TSynapse value max. */
    static Value Max () { return 1.0f; }

    /* TSynapse bias. */
    static Value Bias () { return 0; }

    /* Min synapse noise. */
    static Value NoiseMin () { return -0.05; }

    /* Max synapse noise. */
    static Value NoiseMax () return 0.05f }

    /* Generates uniform random noise in the given range.
    static Value SynapseNoise (Value min_value = NoiseMin (),
                               Value max_value = NoiseMax ()) {
      static std::default_random_engine e;
      static std::uniform_real_distribution<Value> dis (min_value, max_value);
      return dis (e);
    } */

    /* Constructs a TSynapse with the given min/max noise range. */
    TSynapse (TPerceptron< Value>* rx)
      : rx_ (rx), weight_ (SynapseNoise ()) {}

    /* Constructs a TSynapse with the given weight. */
    TSynapse (TPerceptron< Value>* rx, Value weight)
      : rx_ (rx), weight_ (weight) {}

    /* Constructs a TSynapse with the given min/max noise range. */
    TSynapse (TPerceptron< Value>* rx, Value min_value,
             Value max_value)
      : rx_ (rx), weight_ (SynapseNoise (min_value, max_value)) {}

    /* Copy constructor shallow copies the rx_ perceptron. */
    TSynapse (const TSynapse& o)
      : weight_ (o.weight_), rx_ (o.rx_) {}

    /* Gets the weight (i.e. how far apart they are). */
    Value Weight () { return weight_; }

    /* Sets the weight. */
    void SetWeight (Value weight) { weight_ = weight; }

    /* Gets the rx terminal. */
    TPerceptron< Value>* GetRX () { return rx_; }

    /* Gets the rx terminal. */
    void SetRx (TPerceptron< Value>* rx) {
      DASSERT (rx);
      rx_ = rx;
    }



    private:

    Value weight_;            //< Weight of the synaptic terminal connection.
    TPerceptron< Value>* rx_; //< RX synaptic terminal.
};

}       //< namespace _
#endif
#endif
