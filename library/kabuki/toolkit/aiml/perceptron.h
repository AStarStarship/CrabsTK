
#pragma once
#include <stdafx.h>

#ifndef HEADER_FOR_PERCEPTRON
#define HEADER_FOR_PERCEPTRON

typedef float float_t;
typedef unsigned int uint;

static const float_t kSynapseMin,
                     kSynapseMax,
                     kSynapseBiase,
                     kSynapseNoiseMin,
                     kSynapseNoiseMax;

float_t SynapseNoise ();

/**  
class Axion_UI1 {
    public:

    Axion_UI1 (uint8_t& x);

    void Connect (uint8_t& x);

    void Connect (float_t& y);

    void Disconnect (float_t& y);

    void Update ();

    private:

    uint8_t   x_n_minus_1_;
    uint8_t&  x_;
    std::vector<float_t&> y_;
};

class Axion1D {
    public:

    Axion1D (float& value);
};*/

/** A Perceptron.

    A Perceptron needs to get it's input from either floating-point or 
    integers.

    @code
    x_1 ---\        _________     _________
        w_1 \       | _____ |     |   ___ |
    x_2 -----\      | \     |     |   |   |
        w_2   |-----|  \    |---->|   |   |----> Output Y
    ...      /      |  /    |  h  |   |   |
    x_n ----/       | /____ |     | __|0  |
        w_n         |_______|     |_______|
    @endcode
*/
class Perceptron {
    public:

    Perceptron ();

    void Connect (Perceptron* p);

    void Disconnect (Perceptron* p);

    float_t GetWeight ();

    void SetWeight (float_t weight);

    float_t GetValue ();

    void SetValue (float_t& value);

    void Update ();

    private:

    float_t y_;
    std::vector<Perceptron*> x_;

    private:

    float_t weight_;
    float_t x_n_minus_1_;
};

#endif  //< #fndef HEADER_FOR_PERCEPTRON
