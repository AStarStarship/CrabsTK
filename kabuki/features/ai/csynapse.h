/** Kabuki Toolkit @version 0.x
@file    /kabuki/f2sdk/ai/perceptron.h
@author  Cale McCollough <cale.mccollough@gmail.com>
@license Copyright (C) 2014-19 Cale McCollough <calemccollough.github.io>;
All right reserved (R). Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License.
You may obtain a copy of the License at www.apache.org/licenses/LICENSE-2.0.
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License. */

#pragma once
#include <pch.h>

#if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1

#ifndef HEADER_FOR_KABUKI_TOOLKIT_AIML_SYNAPSE
#define HEADER_FOR_KABUKI_TOOLKIT_AIML_SYNAPSE

typedef float        float_t;
typedef unsigned SI4 uint;

namespace _ {

static const float_t kSynapseMin,      //< Synapse value min.
                     kSynapseMax,      //< Synapse value max.
                     kSynapseBias,     //< Synapse bias.
                     kSynapseNoiseMin, //< Min synapse noise.
                     kSynapseNoiseMax; //< Max synapse noise.

/** Generates uniform random noise in the given range. */
float_t SynapseNoise (float_t min_value = kSynapseNoiseMin,
                      float_t max_value = kSynapseNoiseMax);
class Perceptron;

/** A synapse between an axon and dendrite. */
class Synapse {
    public:

    /** Constructs a Synapse with the given min/max noise range. */
    Synapse (Perceptron* rx);

    /** Constructs a Synapse with the given weight. */
    Synapse (Perceptron* rx, float_t weight);

    /** Constructs a Synapse with the given min/max noise range. */
    Synapse (Perceptron* rx, float_t min_value,
             float_t max_value);

    /** Copy constructor shallow copies the rx_ perceptron. */
    Synapse (const Synapse& other);

    /** Gets the weight (i.e. how far apart they are). */
    float_t GetWeight ();

    /** Sets the weight. */
    void SetWeight (float_t weight);

    /** Gets the rx terminal. */
    Perceptron* GetRx ();

    /** Gets the rx terminal. */
    void SetRx (Perceptron* rx);

    private:

    float_t     weight_; //< Weight of the synaptic terminal connection.
    Perceptron* rx_;     //< rx synaptic terminal.
};

}       //< namespace _
#endif  //< #ifndef HEADER_FOR_KABUKI_TOOLKIT_AIML_SYNAPSE
#endif  //< #if MAJOR_SEAM >= 5 && MINOR_SEAM >= 1
