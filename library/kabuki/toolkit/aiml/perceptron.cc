

#pragma once
#include <stdafx.h>

#include "perceptron.h"

float_t kSynapseMax  = 1.0;
float_t kSynapseMin  = -1.0;
float_t kSynapseBias  = 0.0;
float_t kSynapseNoiseMin = -0.05;
float_t kSynapseNoiseMax = 0.05;

float_t SynapseNoise () {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis (kSynapseNoiseMin, kSynapseNoiseMax); // rage 0 - 1
    return dis (e);
}

Perceptron::Perceptron () :
    y_ (kSynapseMin) {
    // Nothing to do here! ({:->)-+=<
}

void Perceptron::Connect (Perceptron* synapse) {
    x_.push_back (synapse);
}

void Perceptron::Disconnect (Perceptron* synapse) {
    for (int i = x_.size () - 1; i > 0; --i) {
        if (&x_[i] == synapse) {
            x_.erase (x_.begin () + i);
        }
    }
}

float_t Perceptron::GetValue () {
    return x_n_minus_1_;
}

void Perceptron::SetValue (float_t& value) {
    x_n_minus_1_ = value;
}

float_t Perceptron::GetWeight () {
    return weight_;
}

void Perceptron::SetWeight (float_t weight) {
    weight_ = weight;
}

void Perceptron::Update () {
    int n = x_.size ();
    if (n == 0) {
        y_ = 0.0;
    }
    for (--n; n > 0; --n) {
         
    }
}
