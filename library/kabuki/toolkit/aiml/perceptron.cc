

#pragma once
#include <stdafx.h>

#include "perceptron.h"

float_t kSynapseMax  = 1.0;
float_t kSynapseMin  = -1.0;
float_t kSynapseBias  = 0.0;
float_t kSynapseNoiseMin = -0.05;
float_t kSynapseNoiseMax = 0.05;

Axion_UI1::Axion_UI1 (uint8_t& x) :
    x_ (x) {
    // Nothing to do here!
}

void Axion_UI1::Connect (uint8_t& x) {
    x_ = x;
}

void Axion_UI1::Connect (float_t& y) {
    y_.push_back (y);
}

void Axion_UI1::Disconnect (float_t& y) {
    for (int i = y_.size () - 1; i > 0; --i) {
        if (y_[i] == y) {
            y_.erase (y_.begin () + i);
        }
    }
}

void Axion_UI1::Update () {
    uint8_t x = x_;
    if (x == x_n_minus_1_) return; //< No change.
    
}

float_t SynapseNoise () {
    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis (kSynapseNoiseMin, kSynapseNoiseMax); // rage 0 - 1
    return dis (e);
}

Axion1D::Axion1D (float& value) :
    x_n_minus_1_ (value) {
    // Nothing to do here!
}

float Axion1D::GetWeight () {
    return weight_;
}

void Axion1D::SetWeight (float weight) {
    weight_ = weight;
}

float Axion1D::GetValue () {
    return x_n_minus_1_;
}

void Axion1D::SetValue (float& value) {
    x_n_minus_1_ = value;
}

Perceptron::Perceptron () :
    y_ (kSynapseMin) {
    // Nothing to do here! ({:->)-+=<
}

void Perceptron::Connect (Axion1D* synapse) {
    x_.push_back (synapse);
}

void Perceptron::Disconnect (Axion1D* synapse) {
    for (int i = x_.size () - 1; i > 0; --i) {
        if (&x_[i] == synapse) {
            x_.erase (x_.begin () + i);
        }
    }
}

void Perceptron::Update () {
    int n = x_.size ();
    if (n == 0) {
        y_ = 0.0;
    }
    for (--n; n > 0; --n) {
         
    }
}
